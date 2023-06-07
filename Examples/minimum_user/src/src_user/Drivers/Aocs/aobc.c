#pragma section REPRO
/**
 * @file
 * @brief AOBC の Driver
 */
#include "./aobc.h"
#include "./aobc_command_definitions.h"
#include "./aobc_telemetry_definitions.h"
#include "./aobc_telemetry_buffer.h"
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_core/Drivers/Protocol/eb90_frame_for_driver_super.h>
#include <src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.h>
#include <string.h>

#define AOBC_STREAM_TLM_CMD   (0)   //!< テレコマで使うストリーム

static uint8_t AOBC_tx_frame_[EB90_FRAME_HEADER_SIZE +
                              CTCP_MAX_LEN +
                              EB90_FRAME_FOOTER_SIZE];

static DS_ERR_CODE AOBC_load_driver_super_init_settings_(DriverSuper* p_super);
static DS_ERR_CODE AOBC_analyze_rec_data_(DS_StreamConfig* p_stream_config,
                                          void* p_driver);

DS_INIT_ERR_CODE AOBC_init(AOBC_Driver* aobc_driver, uint8_t ch, DS_StreamRecBuffer* rx_buffer)
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
                rx_buffer,
                AOBC_load_driver_super_init_settings_);
  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;
  return DS_INIT_OK;
}


static DS_ERR_CODE AOBC_load_driver_super_init_settings_(DriverSuper* p_super)
{
  DS_StreamConfig* p_stream_config;

  p_super->interface = UART;

  // stream は 0 のみ
  p_stream_config = &(p_super->stream_config[AOBC_STREAM_TLM_CMD]);

  CTCP_init_dssc(p_stream_config, AOBC_tx_frame_, sizeof(AOBC_tx_frame_), AOBC_analyze_rec_data_);

  // 定期 TLM の監視機能の有効化しない → ので設定上書きなし

  DSSC_enable(p_stream_config);

  return DS_ERR_CODE_OK;
}


DS_REC_ERR_CODE AOBC_rec(AOBC_Driver* aobc_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* p_stream_config;

  ret = DS_receive(&(aobc_driver->driver.super));

  if (ret != DS_ERR_CODE_OK) return DS_REC_DS_RECEIVE_ERR;

  p_stream_config = &(aobc_driver->driver.super.stream_config[AOBC_STREAM_TLM_CMD]);
  if (DSSC_get_rec_status(p_stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME) return DS_REC_OK;    // 受信せず（[TODO] 詳細なエラー処理は一旦しない）

  ret = DS_analyze_rec_data(&(aobc_driver->driver.super), AOBC_STREAM_TLM_CMD, aobc_driver);

  if (ret != DS_ERR_CODE_OK) return DS_REC_ANALYZE_ERR;

  return DS_REC_OK;
}


static DS_ERR_CODE AOBC_analyze_rec_data_(DS_StreamConfig* p_stream_config,
                                          void* p_driver)
{
  AOBC_Driver* aobc_driver = (AOBC_Driver*)p_driver;

  aobc_driver->info.comm.rx_err_code = AOBC_RX_ERR_CODE_OK;

  if (!EB90_FRAME_is_valid_crc_of_dssc(p_stream_config))
  {
    aobc_driver->info.comm.rx_err_code = AOBC_RX_ERR_CODE_CRC_ERR;
    return DS_ERR_CODE_ERR;
  }

  return AOBC_buffer_tlm_packet(p_stream_config, aobc_driver);
}


// TODO: DS protocol 改修にともなって古くなったので治す
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

  p_stream_config = &(aobc_driver->driver.super.stream_config[AOBC_STREAM_TLM_CMD]);

  // tx_frameの設定
  CCP_set_tx_frame_to_dssc(p_stream_config, packet);

  cmd_code = (AOBC_CMD_CODE)CCP_get_id(packet);

  // [TODO] ここではコマンドが実際に存在するか，ということはフィルタしない！（でいいよね？）
  // 必要があれば，AOBC 側で弾くべき．
  switch (cmd_code)
  {
  case AOBC_Cmd_CODE_GENERATE_TLM:            // FALLTHROUGH
  case AOBC_Cmd_CODE_TG_GENERATE_TLM:         // FALLTHROUGH
  case AOBC_Cmd_CODE_TG_GENERATE_HP_TLM:      // FALLTHROUGH
  case AOBC_Cmd_CODE_TG_GENERATE_RT_TLM:      // FALLTHROUGH
  case AOBC_Cmd_CODE_TG_GENERATE_ST_TLM:      // FALLTHROUGH
  case AOBC_Cmd_CODE_TG_FORWARD_TLM:          // FALLTHROUGH
  case AOBC_Cmd_CODE_TG_FORWARD_AS_HP_TLM:    // FALLTHROUGH
  case AOBC_Cmd_CODE_TG_FORWARD_AS_RT_TLM:    // FALLTHROUGH
  case AOBC_Cmd_CODE_TG_FORWARD_AS_ST_TLM:
    ret = DS_send_req_tlm_cmd(&(aobc_driver->driver.super), AOBC_STREAM_TLM_CMD);
    break;
  default:
    ret = DS_send_general_cmd(&(aobc_driver->driver.super), AOBC_STREAM_TLM_CMD);
    break;
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
