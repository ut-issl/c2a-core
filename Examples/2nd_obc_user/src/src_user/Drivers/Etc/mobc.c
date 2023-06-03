#pragma section REPRO
/**
 * @file
 * @brief MOBC の Driver
 */

// FIXME: DS 側の TCP が整理されたら， TCP 関連を撲滅し， CTCP に統一する

#include "./mobc.h"
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/Drivers/Protocol/eb90_frame_for_driver_super.h>
#include <src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.h>
#include <string.h>

#define MOBC_STREAM_TLM_CMD   (0)   //!< テレコマで使うストリーム

static uint8_t MOBC_tx_frame_[EB90_FRAME_HEADER_SIZE +
                              CTCP_MAX_LEN +
                              EB90_FRAME_FOOTER_SIZE];

static DS_ERR_CODE MOBC_load_driver_super_init_settings_(DriverSuper* p_super);
static DS_ERR_CODE MOBC_analyze_rec_data_(DS_StreamConfig* p_stream_config,
                                          void* p_driver);


DS_INIT_ERR_CODE MOBC_init(MOBC_Driver* mobc_driver, uint8_t ch, DS_StreamRecBuffer* rx_buffer)
{
  DS_ERR_CODE ret;

  memset(mobc_driver, 0x00, sizeof(MOBC_Driver));

  mobc_driver->driver.uart_config.ch = ch;
  mobc_driver->driver.uart_config.baudrate = 115200;
  mobc_driver->driver.uart_config.parity_settings = PARITY_SETTINGS_NONE;
  mobc_driver->driver.uart_config.data_length = UART_DATA_LENGTH_8BIT;
  mobc_driver->driver.uart_config.stop_bit = UART_STOP_BIT_1BIT;

  ret = DS_init(&(mobc_driver->driver.super),
                &(mobc_driver->driver.uart_config),
                rx_buffer,
                MOBC_load_driver_super_init_settings_);
  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;
  return DS_INIT_OK;
}


static DS_ERR_CODE MOBC_load_driver_super_init_settings_(DriverSuper* p_super)
{
  DS_StreamConfig* p_stream_config;

  p_super->interface = UART;

  // stream は 0 のみ
  p_stream_config = &(p_super->stream_config[MOBC_STREAM_TLM_CMD]);

  CTCP_init_dssc(p_stream_config, MOBC_tx_frame_, sizeof(MOBC_tx_frame_), MOBC_analyze_rec_data_);

  // 定期 TLM の監視機能の有効化しない → ので設定上書きなし

  DSSC_enable(p_stream_config);

  return DS_ERR_CODE_OK;
}


DS_REC_ERR_CODE MOBC_rec(MOBC_Driver* mobc_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* p_stream_config;

  ret = DS_receive(&(mobc_driver->driver.super));

  if (ret != DS_ERR_CODE_OK) return DS_REC_DS_RECEIVE_ERR;

  p_stream_config = &(mobc_driver->driver.super.stream_config[MOBC_STREAM_TLM_CMD]);
  if (DSSC_get_rec_status(p_stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME) return DS_REC_OK;    // 受信せず（TODO: 詳細なエラー処理は一旦しない）

  ret = DS_analyze_rec_data(&(mobc_driver->driver.super), MOBC_STREAM_TLM_CMD, mobc_driver);

  if (ret != DS_ERR_CODE_OK) return DS_REC_ANALYZE_ERR;

  return DS_REC_OK;
}


static DS_ERR_CODE MOBC_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver)
{
  MOBC_Driver* mobc_driver = (MOBC_Driver*)p_driver;
  CommonCmdPacket packet;   // FIXME: これは static にする？
                            //        static のほうがコンパイル時にアドレスが確定して安全． Out of stack space を回避できる
                            //        一方でメモリ使用量は増える．
  DS_ERR_CODE ret =  CCP_get_ccp_from_dssc(p_stream_config, &packet);
  if (ret != DS_ERR_CODE_OK)
  {
    mobc_driver->info.comm.rx_err_code = MOBC_RX_ERR_CODE_INVALID_PACKET;
    return ret;
  }

  mobc_driver->info.comm.rx_err_code = MOBC_RX_ERR_CODE_OK;

  if (!EB90_FRAME_is_valid_crc_of_dssc(p_stream_config))
  {
    mobc_driver->info.comm.rx_err_code = MOBC_RX_ERR_CODE_CRC_ERR;
    return DS_ERR_CODE_ERR;
  }

  // MOBC からのコマンドは以下のパターン
  //  APID:
  //      APID_AOBC_CMD
  //  CCP_EXEC_TYPE:
  //      CCP_EXEC_TYPE_GS    <- GS から MOBC のキューに入らず直接転送されたもの
  //      CCP_EXEC_TYPE_TL0   <- GS から MOBC のキューに入らず直接転送されたもの
  //      CCP_EXEC_TYPE_MC    <- GS から MOBC のキューに入らず直接転送されたもの
  //      CCP_EXEC_TYPE_RT    <- これが GS → MOBC との違いで， MOBC の TLC/BC キューに溜まって実行されたもの
  // CCP_DEST_TYPE:
  //      CCP_DEST_TYPE_TO_ME (CCP_DEST_TYPE_TO_APID, CCP_DEST_TYPE_TO_AOBC の可能性はなくはないが， ME に上書きされているはず)

  // FIXME: ここで返り値が NG だった場合，なにを return するかは議論の余地あり
  //        通信的には OK なので， OK を返すのでいいという認識
  // FIXME: CTCP 大工事が終わったら，返り値をちゃんと見るようにする
  mobc_driver->info.c2a.ph_ack = PH_analyze_cmd_packet(&packet);

  return DS_ERR_CODE_OK;
}


DS_CMD_ERR_CODE MOBC_send(MOBC_Driver* mobc_driver, const CommonTlmPacket* packet)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* p_stream_config;

  p_stream_config = &(mobc_driver->driver.super.stream_config[MOBC_STREAM_TLM_CMD]);

  // tx_frameの設定
  CTP_set_tx_frame_to_dssc(p_stream_config, packet);

  ret = DS_send_general_cmd(&(mobc_driver->driver.super), MOBC_STREAM_TLM_CMD);

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
