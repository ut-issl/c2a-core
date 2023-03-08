#pragma section REPRO
/**
 * @file
 * @brief Ground station との通信の Driver
 */

#include "gs.h"

#include <string.h>
#include <stdint.h>

#include "../../IfWrapper/ccsds_user.h"
#include "../../TlmCmd/Ccsds/tc_transfer_frame.h"
#include <src_core/Drivers/Super/driver_super.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/TlmCmd/Ccsds/space_packet_typedef.h>


#define GS_RX_HEADER_SIZE (2)
#define GS_RX_FRAMELENGTH_TYPE_SIZE (2)
#define GS_TX_STREAM (0) // どれでも良いがとりあえず 0 で

#if GS_RX_HEADER_NUM > DS_STREAM_MAX
#error GS RX HEADER NUM TOO MANY
#endif

// それぞれ AD, BD, BC
static uint8_t GS_rx_header_[GS_RX_HEADER_NUM][GS_RX_HEADER_SIZE];
static uint8_t GS_tx_frame_[VCDU_LEN];

/**
 * @brief CCSDS 側 Driver の DS 上での初期化設定
 * @param[in] p_super: DriverSuper
 * @return DS_ERR_CODE
 */
static DS_ERR_CODE GS_load_ccsds_driver_super_init_settings_(DriverSuper* p_super);

/**
 * @brief UART 側 Driver の DS 上での初期化設定
 * @param[in] p_super: DriverSuper
 * @return DS_ERR_CODE
 */
static DS_ERR_CODE GS_load_uart_driver_super_init_settings_(DriverSuper* p_super);

/**
 * @brief 上記初期化の共通部分をまとめた関数
 * @param[in] p_super: DriverSuper
 */
static void GS_load_default_driver_super_init_settings_(DriverSuper* p_super);

/**
 * @brief 地上からの受信データ解析関数
 * @param[in] p_stream_config: DS_StreamConfig
 * @param[in] p_driver: GS_Driver
 * @param[in]
 */
static DS_ERR_CODE GS_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver);

DS_INIT_ERR_CODE GS_init(GS_Driver* gs_driver,
                         uint8_t uart_ch,
                         DS_StreamRecBuffer* ccsds_rx_buffers[DS_STREAM_MAX],
                         DS_StreamRecBuffer* uart_rx_buffers[DS_STREAM_MAX])
{
  DS_ERR_CODE ret_uart, ret_ccsds;
  int i;
  int stream;

  memset(gs_driver, 0x00, sizeof(GS_Driver));

  GS_validate_init();

  gs_driver->driver_ccsds.ccsds_config.bitrate = 115200;
  gs_driver->driver_ccsds.ccsds_config.ch = 0;

  gs_driver->driver_uart.uart_config.ch = uart_ch;
  gs_driver->driver_uart.uart_config.baudrate = 115200;
  gs_driver->driver_uart.uart_config.parity_settings = PARITY_SETTINGS_NONE;
  gs_driver->driver_uart.uart_config.data_length = UART_DATA_LENGTH_8BIT;
  gs_driver->driver_uart.uart_config.stop_bit = UART_STOP_BIT_1BIT;

  memset(GS_rx_header_, 0x00, sizeof(GS_rx_header_));
  GS_rx_header_[0][0] |= (uint8_t)((TCTF_TYPE_AD & 0x0f) << 4);
  GS_rx_header_[1][0] |= (uint8_t)((TCTF_TYPE_BD & 0x0f) << 4);
  GS_rx_header_[2][0] |= (uint8_t)((TCTF_TYPE_BC & 0x0f) << 4);
  for (stream = 0; stream < GS_RX_HEADER_NUM; ++stream)
  {
    GS_rx_header_[stream][0] |= (uint8_t)((TCTF_SCID_SAMPLE_SATELLITE & 0x3ff) >> 8);
    GS_rx_header_[stream][1] |= (uint8_t)(TCTF_SCID_SAMPLE_SATELLITE & 0xff);
  }

  ret_ccsds = DS_init_streams(&gs_driver->driver_ccsds.super,
                              &gs_driver->driver_ccsds.ccsds_config,
                              ccsds_rx_buffers,
                              GS_load_ccsds_driver_super_init_settings_);
  ret_uart  = DS_init_streams(&gs_driver->driver_uart.super,
                              &gs_driver->driver_uart.uart_config,
                              uart_rx_buffers,
                              GS_load_uart_driver_super_init_settings_);
  if (ret_ccsds != DS_ERR_CODE_OK || ret_uart != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;
  gs_driver->latest_info = &gs_driver->info[GS_PORT_TYPE_CCSDS];
  gs_driver->tlm_tx_port_type = GS_PORT_TYPE_CCSDS;

  for (i = 0; i < GS_PORT_TYPE_NUM; ++i)
  {
    gs_driver->info[i].rx.rec_status = DS_ERR_CODE_OK;
    gs_driver->info[i].rx.ret_from_if_rx = 0;
    gs_driver->info[i].rx.last_dest_type = CCP_DEST_TYPE_TO_UNKOWN;
    gs_driver->info[i].rx.last_rec_time = 0;
    gs_driver->info[i].rx.cmd_ack = PH_ACK_SUCCESS;

    gs_driver->info[i].rx.tctf.last_rec_tctf_type = TCTF_TYPE_UNKNOWN;
    gs_driver->info[i].rx.tctf.ad_rec_status = GS_VALIDATE_ERR_OK;
    gs_driver->info[i].rx.tctf.bd_rec_status = GS_VALIDATE_ERR_OK;
    gs_driver->info[i].rx.tctf.bd_rec_status = GS_VALIDATE_ERR_OK;

    gs_driver->info[i].tx.send_cycle = 0;
    gs_driver->info[i].tx.vcid = VCDU_VCID_UNKNOWN;
    gs_driver->info[i].tx.vcdu_counter = 0;
  }

  gs_driver->ccsds_info.buffer_num = 8;
  gs_driver->driver_uart.is_tlm_on = 1;

  return DS_INIT_OK;
}

static DS_ERR_CODE GS_load_ccsds_driver_super_init_settings_(DriverSuper* p_super)
{
  p_super->interface = CCSDS;
  GS_load_default_driver_super_init_settings_(p_super);

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE GS_load_uart_driver_super_init_settings_(DriverSuper* p_super)
{
  p_super->interface = UART;
  GS_load_default_driver_super_init_settings_(p_super);

  return DS_ERR_CODE_OK;
}

static void GS_load_default_driver_super_init_settings_(DriverSuper* p_super)
{
  DS_StreamConfig* p_stream_config;
  int stream;

  for (stream = 0; stream < GS_RX_HEADER_NUM; ++stream)
  {
    p_stream_config = &(p_super->stream_config[stream]);
    DSSC_enable(p_stream_config);
    DSSC_enable_strict_frame_search(p_stream_config);   // 複数ストリームがあり，かつ論理的な受信漏れを0にするため．

    DSSC_set_tx_frame(p_stream_config, GS_tx_frame_); // 全 stream, uart, ccsds に関わらず共通
    DSSC_set_tx_frame_size(p_stream_config, VCDU_LEN); // VCDU を送信

    DSSC_set_rx_header(p_stream_config, GS_rx_header_[stream], GS_RX_HEADER_SIZE);
    DSSC_set_rx_frame_size(p_stream_config, -1); // 可変長
    DSSC_set_rx_framelength_pos(p_stream_config, GS_RX_HEADER_SIZE);
    DSSC_set_rx_framelength_type_size(p_stream_config, GS_RX_FRAMELENGTH_TYPE_SIZE);
    DSSC_set_rx_framelength_offset(p_stream_config, 1); // TCTF の framelength は 0 起算
    DSSC_set_data_analyzer(p_stream_config, GS_analyze_rec_data_);
  }
}

DS_REC_ERR_CODE GS_rec_tctf(GS_Driver* gs_driver)
{
  uint8_t i, stream;

  for (i = 0; i < GS_PORT_TYPE_NUM; ++i)
  {
    DriverSuper* ds;

    if (i == GS_PORT_TYPE_CCSDS)
    {
      ds = &gs_driver->driver_ccsds.super;
    }
    else
    {
      ds = &gs_driver->driver_uart.super;
    }

    // TODO: これはエラー情報をきちんと把握したいので，アノマリ発行を入れる
    gs_driver->info[i].rx.rec_status = DS_receive(ds);
    gs_driver->info[i].rx.ret_from_if_rx = DSC_get_rec_status(ds)->ret_from_if_rx;

    if (gs_driver->info[i].rx.rec_status != DS_ERR_CODE_OK) continue;

    for (stream = 0; stream < GS_RX_HEADER_NUM; ++stream)
    {
      DS_StreamConfig* p_stream_config;

      p_stream_config = &ds->stream_config[stream];
      if (DSSC_get_rec_status(p_stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME) continue;

      gs_driver->info[i].rx.rec_status = DS_analyze_rec_data(ds, stream, gs_driver);
    }
  }

  // TODO: 常に OK を返すのでいいのか検討
  return DS_REC_OK;
}

static DS_ERR_CODE GS_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver)
{
  const uint8_t* gs_rx_data = DSSC_get_rx_frame(p_stream_config);
  const TcTransferFrame* tctf = TCTF_convert_from_bytes_to_tctf(gs_rx_data);
  GS_Driver* gs_driver = (GS_Driver*)p_driver;
  GS_PORT_TYPE driver_index;
  GS_VALIDATE_ERR tctf_validate_status;
  const TcSegment* tcs;
  const CmdSpacePacket* csp;

  // アドレス計算で CCSDS か UART か判別
  if ((uint32_t)p_stream_config < (uint32_t)&gs_driver->driver_uart)
  {
    driver_index = GS_PORT_TYPE_CCSDS;
  }
  else
  {
    driver_index = GS_PORT_TYPE_UART;
  }

  tctf_validate_status = GS_validate_tctf(tctf);

  gs_driver->info[driver_index].rx.tctf.last_rec_tctf_type = TCTF_get_type(tctf);
  switch (gs_driver->info[driver_index].rx.tctf.last_rec_tctf_type)
  {
  case TCTF_TYPE_AD:
    gs_driver->info[driver_index].rx.tctf.ad_rec_status = tctf_validate_status;
    break;

  case TCTF_TYPE_BC:
    gs_driver->info[driver_index].rx.tctf.bc_rec_status = tctf_validate_status;
    break;

  case TCTF_TYPE_BD:
    gs_driver->info[driver_index].rx.tctf.bd_rec_status = tctf_validate_status;
    break;

  default:
    // not reached
    break;
  }

  if (tctf_validate_status != GS_VALIDATE_ERR_OK) return DS_ERR_CODE_ERR;

  tcs = TCTF_get_tc_segment(tctf);
  csp = TCS_get_command_space_packet(tcs);
  gs_driver->info[driver_index].rx.last_dest_type = CSP_get_dest_type(csp);
  gs_driver->info[driver_index].rx.last_rec_time = TMGR_get_master_total_cycle();
  gs_driver->info[driver_index].rx.cmd_ack = PH_analyze_cmd_packet(csp);  // 受信コマンドパケット解析

  return DS_ERR_CODE_OK;
}

DS_CMD_ERR_CODE GS_send_vcdu(GS_Driver* gs_driver, const VCDU* vcdu)
{
  DS_ERR_CODE ret_ccsds = DS_ERR_CODE_OK;
  DS_ERR_CODE ret_uart  = DS_ERR_CODE_OK;
  size_t vcdu_size = sizeof(VCDU);

  // パディングが無ければ元を GS_tx_frame_ にコピーさせる (444Byte) のコピーが無駄
  if (vcdu_size == VCDU_LEN)
  {
    DSSC_set_tx_frame(&gs_driver->driver_ccsds.super.stream_config[GS_TX_STREAM], (uint8_t*)vcdu);
    DSSC_set_tx_frame(&gs_driver->driver_uart.super.stream_config[GS_TX_STREAM], (uint8_t*)vcdu);
  }
  else
  {
    VCDU_generate_byte_stream(vcdu, GS_tx_frame_); // 送信元にセット 消したいなぁ...
    DSSC_set_tx_frame(&gs_driver->driver_ccsds.super.stream_config[GS_TX_STREAM], GS_tx_frame_);
    DSSC_set_tx_frame(&gs_driver->driver_uart.super.stream_config[GS_TX_STREAM], GS_tx_frame_);
  }

  // CCSDS
  gs_driver->ccsds_info.buffer_num = CCSDS_get_buffer_num();
  if (gs_driver->ccsds_info.buffer_num)
  {
    gs_driver->info[GS_PORT_TYPE_CCSDS].tx.send_cycle = TMGR_get_master_total_cycle();
    gs_driver->info[GS_PORT_TYPE_CCSDS].tx.vcid = VCDU_get_vcid(vcdu);
    gs_driver->info[GS_PORT_TYPE_CCSDS].tx.vcdu_counter = VCDU_get_vcdu_counter(vcdu);

    // DS 側の名称が cmd なだけで送信しているのは TLM
    ret_ccsds = DS_send_general_cmd(&gs_driver->driver_ccsds.super, GS_TX_STREAM);
  }

  // UART
  if (gs_driver->driver_uart.is_tlm_on)
  {
    gs_driver->info[GS_PORT_TYPE_UART].tx.send_cycle = TMGR_get_master_total_cycle();
    gs_driver->info[GS_PORT_TYPE_UART].tx.vcid = VCDU_get_vcid(vcdu);
    gs_driver->info[GS_PORT_TYPE_UART].tx.vcdu_counter = VCDU_get_vcdu_counter(vcdu);

    // DS 側の名称が cmd なだけで送信しているのは TLM
    ret_uart  = DS_send_general_cmd(&gs_driver->driver_uart.super,  GS_TX_STREAM);
  }

  if (ret_ccsds != DS_ERR_CODE_OK || ret_uart != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  return DS_CMD_OK;
}

#pragma section
