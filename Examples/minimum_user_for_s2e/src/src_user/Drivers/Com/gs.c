#pragma section REPRO
/**
 * @file
 * @brief Ground station との通信の Driver
 */

#include "gs.h"

#include <string.h>
#include "../../IfWrapper/ccsds_user.h"
#include "../../TlmCmd/Ccsds/TCFrame.h"
#include <src_core/Drivers/Super/driver_super.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/TlmCmd/Ccsds/space_packet_typedef.h>
#include "../../Library/stdint.h"

#define GS_RX_HEADER_SIZE (2)
#define GS_RX_FRAME_SIZE  (-1) // 可変長
#define GS_RX_FRAMELENGTH_TYPE_SIZE (2)
#define GS_TX_stream (0) // どれでも良いがとりあえず 0 で

#define GS_RX_HEADER_NUM (3)

#if GS_RX_HEADER_NUM > DS_STREAM_MAX
  #error GS RX HEADER NUM TOO MANY
#endif

// header
static const uint8_t GS_rx_header_[GS_RX_HEADER_NUM][GS_RX_HEADER_SIZE] = {{0x03, 0x5C}, {0x23, 0x5C}, {0x33, 0x5C} }; // FIXME: 直す
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

int GS_init(GS_Driver* gs_driver, uint8_t uart_ch)
{
  DS_ERR_CODE ret_uart, ret_ccsds;
  int i;

  memset(gs_driver, 0x00, sizeof(GS_Driver));

  GS_validate_init();

  gs_driver->driver_ccsds.ccsds_config.bitrate = 115200;
  gs_driver->driver_ccsds.ccsds_config.ch = 0;

  gs_driver->driver_uart.uart_config.ch = uart_ch;
  gs_driver->driver_uart.uart_config.baudrate = 115200;
  gs_driver->driver_uart.uart_config.parity_settings = PARITY_SETTINGS_NONE;
  gs_driver->driver_uart.uart_config.data_length = UART_DATA_LENGTH_8BIT;
  gs_driver->driver_uart.uart_config.stop_bit = UART_STOP_BIT_1BIT;

  ret_ccsds = DS_init(&gs_driver->driver_ccsds.super, &gs_driver->driver_ccsds.ccsds_config, GS_load_ccsds_driver_super_init_settings_);
  ret_uart  = DS_init(&gs_driver->driver_uart.super, &gs_driver->driver_uart.uart_config, GS_load_uart_driver_super_init_settings_);
  if (ret_ccsds != DS_ERR_CODE_OK || ret_uart != DS_ERR_CODE_OK) return 1;
  gs_driver->latest_info = &gs_driver->info[GS_PORT_TYPE_CCSDS];
  gs_driver->tlm_tx_port_type = GS_PORT_TYPE_CCSDS;
  gs_driver->is_ccsds_tx_valid = 0;

#ifdef SILS_FW
  gs_driver->is_ccsds_tx_valid = 1; // SILS でこれが最初から ON になってないと何も降りてこない
#endif

  for (i = 0; i < GS_PORT_TYPE_NUM; ++i)
  {
    gs_driver->info[i].rec_status = DS_ERR_CODE_OK;
    gs_driver->info[i].last_rec_tcf_type = GS_TCF_TYPE_ENUM_UNKNOWN;
    gs_driver->info[i].ad_rec_status = DS_ERR_CODE_OK;
    gs_driver->info[i].bd_rec_status = DS_ERR_CODE_OK;
    gs_driver->info[i].bd_rec_status = DS_ERR_CODE_OK;
    gs_driver->info[i].tc_frame_validate_status = GS_VALIDATE_ERR_OK;
    gs_driver->info[i].ret_from_if_rx = 0;
    gs_driver->info[i].last_rec_time = 0;
    gs_driver->info[i].cmd_ack = PH_ACK_SUCCESS;

    gs_driver->info[i].send_cycle = 0;
    gs_driver->info[i].vcid = VCDU_VCID_UNKNOWN;
    gs_driver->info[i].vcdu_counter = 0;
  }

  for (i = 0; i < CCSDS_SELECT_NUM; ++i)
  {
    gs_driver->ccsds_info.uip_stat[i] = 0;
  }
  gs_driver->ccsds_info.buffer_num = 8;

  return 0;
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
    DSSC_set_rx_frame_size(p_stream_config, GS_RX_FRAME_SIZE); // 可変長
    DSSC_set_rx_framelength_pos(p_stream_config, GS_RX_HEADER_SIZE);
    DSSC_set_rx_framelength_type_size(p_stream_config, GS_RX_FRAMELENGTH_TYPE_SIZE);
    DSSC_set_rx_framelength_offset(p_stream_config, 1); // TCF の framelength は 0 起算
    DSSC_set_data_analyzer(p_stream_config, GS_analyze_rec_data_);
  }
}

int GS_rec_tcf(GS_Driver* gs_driver)
{
  int i, stream;

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
    gs_driver->info[i].rec_status = DS_receive(ds);
    gs_driver->info[i].ret_from_if_rx = ds->config.rec_status_.ret_from_if_rx;

    if (gs_driver->info[i].rec_status != DS_ERR_CODE_OK) continue;

    for (stream = 0; stream < GS_RX_HEADER_NUM; ++stream)
    {
      DS_StreamConfig* p_stream_config;

      p_stream_config = &ds->stream_config[stream];
      if (DSSC_get_rec_status(p_stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME) continue;

      gs_driver->info[i].rec_status = DS_analyze_rec_data(ds, (uint8_t)stream, gs_driver);

      gs_driver->info[i].last_rec_tcf_type = (GS_TCF_TYPE_ENUM)stream;
      switch (gs_driver->info[i].last_rec_tcf_type)
      {
      case GS_TCF_TYPE_ENUM_AD_CMD:
        gs_driver->info[i].ad_rec_status = gs_driver->info[i].rec_status;
        break;

      case GS_TCF_TYPE_ENUM_BC_CMD:
        gs_driver->info[i].bc_rec_status = gs_driver->info[i].rec_status;
        break;

      case GS_TCF_TYPE_ENUM_BD_CMD:
        gs_driver->info[i].bd_rec_status = gs_driver->info[i].rec_status;
        break;

      default:
        break;
      }
    }
  }

  return 0;
}

static DS_ERR_CODE GS_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver)
{
  const uint8_t* gs_rx_data = DSSC_get_rx_frame(p_stream_config);
  const TCFrame* tc_frame = TCF_convert_raw_byte(gs_rx_data);
  GS_Driver* gs_driver = (GS_Driver*)p_driver;
  GS_PORT_TYPE driver_index;
  const TCSegment* tc_segment;
  const CmdSpacePacket* cmd_space_packet;

  // アドレス計算で CCSDS か UART か判別
  if ((uint32_t)p_stream_config < (uint32_t)&gs_driver->driver_uart)
  {
    driver_index = GS_PORT_TYPE_CCSDS;
  }
  else
  {
    driver_index = GS_PORT_TYPE_UART;
  }

  gs_driver->info[driver_index].tc_frame_validate_status = GS_validate_tc_frame(tc_frame);
  if (gs_driver->info[driver_index].tc_frame_validate_status != GS_VALIDATE_ERR_OK)
  {
    return DS_ERR_CODE_ERR;
  }

  tc_segment = TCF_get_tc_segment(tc_frame);
  cmd_space_packet = TCS_get_command_space_packet(tc_segment);
  gs_driver->info[driver_index].last_dest_type = CSP_get_dest_type(cmd_space_packet);
  gs_driver->info[driver_index].cmd_ack = PH_analyze_cmd_packet(cmd_space_packet);  // 受信コマンドパケット解析
  gs_driver->info[driver_index].last_rec_time = TMGR_get_master_total_cycle();

  return DS_ERR_CODE_OK;
}

DS_CMD_ERR_CODE GS_send_vcdu(GS_Driver* gs_driver, const VCDU* vcdu)
{
  int i;
  DS_ERR_CODE ret_ccsds = DS_ERR_CODE_OK;
  DS_ERR_CODE ret_uart  = DS_ERR_CODE_OK;
  size_t vcdu_size = sizeof(VCDU);

  // パディングが無ければ元を GS_tx_frame_ にコピーさせる (444Byte) のコピーが無駄
  if (vcdu_size == VCDU_LEN)
  {
    DSSC_set_tx_frame(&gs_driver->driver_ccsds.super.stream_config[GS_TX_stream], (uint8_t*)vcdu);
    DSSC_set_tx_frame(&gs_driver->driver_uart.super.stream_config[GS_TX_stream], (uint8_t*)vcdu);
  }
  else
  {
    VCDU_generate_byte_stream(vcdu, GS_tx_frame_); // 送信元にセット 消したいなぁ...
    DSSC_set_tx_frame(&gs_driver->driver_ccsds.super.stream_config[GS_TX_stream], GS_tx_frame_);
    DSSC_set_tx_frame(&gs_driver->driver_uart.super.stream_config[GS_TX_stream], GS_tx_frame_);
  }

  for (i = 0; i < GS_PORT_TYPE_NUM; ++i)
  {
    if (i == GS_PORT_TYPE_CCSDS)
    {
      // CCSDS 無効, 又は バッファー空きが無いで下の処理は端折る
      // FIXME: 一杯だった時の処理
      gs_driver->ccsds_info.buffer_num = CCSDS_get_buffer_num();
      if (!gs_driver->is_ccsds_tx_valid || !gs_driver->ccsds_info.buffer_num) continue;
    }

    gs_driver->info[i].send_cycle = TMGR_get_master_total_cycle();
    gs_driver->info[i].vcid = VCDU_get_vcid(vcdu);
    gs_driver->info[i].vcdu_counter = VCDU_get_vcdu_counter(vcdu);

    // DS 側の名称が cmd なだけで送信しているのは TLM
    if (i == GS_PORT_TYPE_CCSDS)
    {
      ret_ccsds = DS_send_general_cmd(&gs_driver->driver_ccsds.super, GS_TX_stream);
    }
    else
    {
      ret_uart  = DS_send_general_cmd(&gs_driver->driver_uart.super,  GS_TX_stream);
    }
  }

  if (ret_ccsds != DS_ERR_CODE_OK || ret_uart != DS_ERR_CODE_OK)
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }

  return DS_CMD_OK;
}

#pragma section
