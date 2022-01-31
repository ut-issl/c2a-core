#pragma section REPRO
/**
* @file
* @brief  AOBC の Driver
*/
#include "./aobc.h"
#include "./aobc_command_definitions.h"
#include "./aobc_telemetry_definitions.h"
#include "./aobc_telemetry_buffer.h"
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/Library/endian_memcpy.h>
#include <string.h>

// ヘッダーフッター
static const uint8_t AOBC_stx_[DS_ISSLFMT_STX_SIZE] = {DS_ISSLFMT_STX_1ST_BYTE,
                                                       DS_ISSLFMT_STX_2ND_BYTE};
static const uint8_t AOBC_etx_[DS_ISSLFMT_ETX_SIZE] = {DS_ISSLFMT_ETX_1ST_BYTE,
                                                       DS_ISSLFMT_ETX_2ND_BYTE};

#define AOBC_STREAM_TLM_CMD   (0)   //!< テレコマで使うストリーム

static DS_ERR_CODE AOBC_load_driver_super_init_settings_(DriverSuper* p_super);
static DS_ERR_CODE AOBC_analyze_rec_data_(DS_StreamConfig* p_stream_config,
                                          void* p_driver);

static uint8_t AOBC_tx_frame_[DS_ISSLFMT_COMMON_HEADER_SIZE +
                              CTCP_MAX_LEN +
                              DS_ISSLFMT_COMMON_FOOTER_SIZE];


int AOBC_init(AOBC_Driver* aobc_driver, uint8_t ch)
{
  DS_ERR_CODE ret;

  memset(aobc_driver, 0x00, sizeof(*aobc_driver));
  AOBC_init_tlm_buffer(aobc_driver);

  aobc_driver->driver.uart_config.ch = ch;
  aobc_driver->driver.uart_config.baudrate = 115200;
  aobc_driver->driver.uart_config.parity_settings = PARITY_SETTINGS_NONE;
  aobc_driver->driver.uart_config.data_length = UART_DATA_LENGTH_8BIT;
  aobc_driver->driver.uart_config.stop_bit = UART_STOP_BIT_1BIT;

  ret = DS_init(&(aobc_driver->driver.super),
                &(aobc_driver->driver.uart_config),
                AOBC_load_driver_super_init_settings_);
  if (ret != DS_ERR_CODE_OK) return 1;
  return 0;
}


static DS_ERR_CODE AOBC_load_driver_super_init_settings_(DriverSuper* p_super)
{
  DS_StreamConfig* p_stream_config;

  p_super->interface = UART;

  // streamは0のみ
  p_stream_config = &(p_super->stream_config[AOBC_STREAM_TLM_CMD]);
  DSSC_enable(p_stream_config);

  // 送信はする
  DSSC_set_tx_frame(p_stream_config, AOBC_tx_frame_);  // 送る直前に中身を memcpy する
  DSSC_set_tx_frame_size(p_stream_config, 0);          // 送る直前に値をセットする

  // 定期的な受信はする
  DSSC_set_rx_header(p_stream_config, AOBC_stx_, DS_ISSLFMT_STX_SIZE);
  DSSC_set_rx_footer(p_stream_config, AOBC_etx_, DS_ISSLFMT_ETX_SIZE);
  DSSC_set_rx_frame_size(p_stream_config, -1);    // 可変
  DSSC_set_rx_framelength_pos(p_stream_config, DS_ISSLFMT_STX_SIZE);
  DSSC_set_rx_framelength_type_size(p_stream_config, 2);
  DSSC_set_rx_framelength_offset(p_stream_config,
                                 DS_ISSLFMT_COMMON_HEADER_SIZE + DS_ISSLFMT_COMMON_FOOTER_SIZE);
  DSSC_set_data_analyzer(p_stream_config, AOBC_analyze_rec_data_);

  // 定期 TLM の監視機能の有効化しない → ので設定上書きなし

  return DS_ERR_CODE_OK;
}


int AOBC_rec(AOBC_Driver* aobc_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* p_stream_config;

  ret = DS_receive(&(aobc_driver->driver.super));

  if (ret != DS_ERR_CODE_OK) return 1;

  p_stream_config = &(aobc_driver->driver.super.stream_config[AOBC_STREAM_TLM_CMD]);
  if (DSSC_get_rec_status(p_stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME) return 0;    // 受信せず（[TODO] 詳細なエラー処理は一旦しない）

  ret = DS_analyze_rec_data(&(aobc_driver->driver.super), AOBC_STREAM_TLM_CMD, aobc_driver);

  if (ret != DS_ERR_CODE_OK) return 1;

  return 0;
}


static DS_ERR_CODE AOBC_analyze_rec_data_(DS_StreamConfig* p_stream_config,
                                                    void* p_driver)
{
  AOBC_Driver* aobc_driver = (AOBC_Driver*)p_driver;

  aobc_driver->info.comm.rx_err_code = AOBC_RX_ERR_CODE_OK;

  // [TODO] ここに CRC チェックをいれる
  // AOBC_RX_ERR_CODE_CRC_ERR を入れる

  return AOBC_buffer_tlm_packet(p_stream_config, aobc_driver);
}


#if 0
// 非C2A系列はこのように書く
static DS_ERR_CODE AOBC_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver)
{
  AOBC_Driver* aobc_driver = (AOBC_Driver*)p_driver;
  uint32_t     tlm_ver = DS_ISSLFMT_get_tlm_version(p_stream_config);
  uint32_t     tlm_id  = DS_ISSLFMT_get_tlm_id(tlm_ver, p_stream_config);

  aobc_driver->info.comm.rx_err_code = AOBC_RX_ERR_CODE_OK;

  // [TODO] ここ自動生成したい...
  switch (tlm_id)
  {
  case AOBC_Tlm_CODE_HK:
    return AOBC_analyze_tlm_hk_(p_stream_config, aobc_driver);
  default:
    aobc_driver->info.comm.rx_err_code = AOBC_RX_ERR_CODE_TLM_NOT_FOUND;
    return DS_ERR_CODE_OK;
  }
}
#endif


DS_CMD_ERR_CODE AOBC_send_cmd(AOBC_Driver* aobc_driver, const CommonCmdPacket* packet)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* p_stream_config;
  AOBC_CMD_CODE cmd_code;
  uint16_t      ctcp_len;
  uint16_t      crc;
  size_t        pos;
  size_t        size;

  p_stream_config = &(aobc_driver->driver.super.stream_config[AOBC_STREAM_TLM_CMD]);

  // tx_frameの設定
  ctcp_len = CCP_get_packet_len(packet);
  DSSC_set_tx_frame_size(p_stream_config,
                         (uint16_t)(ctcp_len + DS_ISSLFMT_COMMON_HEADER_SIZE + DS_ISSLFMT_COMMON_FOOTER_SIZE));

  pos  = 0;
  size = DS_ISSLFMT_STX_SIZE;
  memcpy(&(AOBC_tx_frame_[pos]), AOBC_stx_, size);
  pos += size;
  size = DS_ISSLFMT_LEN_SIZE;
  endian_memcpy(&(AOBC_tx_frame_[pos]), &ctcp_len, size);       // ここはエンディアンを気にする！
  pos += size;
  size = (size_t)ctcp_len;
  memcpy(&(AOBC_tx_frame_[pos]), packet->packet, size);
  pos += size;

  crc = DS_ISSLFMT_calc_crc(AOBC_tx_frame_, pos);

  size = DS_ISSLFMT_CRC_SIZE;
  endian_memcpy(&(AOBC_tx_frame_[pos]), &crc, size);       // ここはエンディアンを気にする！
  pos += size;
  size = DS_ISSLFMT_ETX_SIZE;
  memcpy(&(AOBC_tx_frame_[pos]), AOBC_etx_, size);

  cmd_code = (AOBC_CMD_CODE)CCP_get_id(packet);

  // [TODO] ここではコマンドが実際に存在するか，ということはフィルタしない！（でいいよね？）
  // 必要があれば，AOBC 側で弾くべき．
  if (cmd_code == AOBC_Cmd_CODE_GENERATE_TLM)
  {
    ret = DS_send_req_tlm_cmd(&(aobc_driver->driver.super), AOBC_STREAM_TLM_CMD);
  }
  else
  {
    ret = DS_send_general_cmd(&(aobc_driver->driver.super), AOBC_STREAM_TLM_CMD);
  }

  if (ret == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    // TODO: エラー処理？
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}

#pragma section
