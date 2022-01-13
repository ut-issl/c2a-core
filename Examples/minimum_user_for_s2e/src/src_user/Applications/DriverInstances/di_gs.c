#pragma section REPRO
/**
 * @file
 * @brief GS Driver のインスタンス化
 */

#include "di_gs.h"

#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/Library/print.h>
#include "../../Drivers/Com/gs_validate.h"
#include "../../Settings/port_config.h"

static GS_Driver gs_driver_;
const GS_Driver* const gs_driver = &gs_driver_;

static DI_GS_TlmPacketHandler DI_GS_ms_tlm_packet_handler_; // mission
const DI_GS_TlmPacketHandler* const DI_GS_ms_tlm_packet_handler = &DI_GS_ms_tlm_packet_handler_;
static DI_GS_TlmPacketHandler DI_GS_rp_tlm_packet_handler_; // replay tlm
const DI_GS_TlmPacketHandler* const DI_GS_rp_tlm_packet_handler = &DI_GS_rp_tlm_packet_handler_;

// 以下 init と update の定義
static void DI_GS_cmd_packet_handler_init_(void);
static void DI_GS_cmd_packet_handler_(void);

static void DI_GS_mst_packet_handler_init_(void);
static void DI_GS_mst_packet_handler_(void);
static void DI_GS_rpt_packet_handler_init_(void);
static void DI_GS_rpt_packet_handler_(void);

static void DI_GS_set_t2m_flush_interval_(cycle_t flush_interval, DI_GS_TlmPacketHandler* gs_tlm_packet_handler);

AppInfo DI_GS_cmd_packet_handler(void)
{
  return AI_create_app_info("GS_CMD", DI_GS_cmd_packet_handler_init_, DI_GS_cmd_packet_handler_);
}

AppInfo DI_GS_mst_packet_handler(void)
{
  return AI_create_app_info("GS_MST", DI_GS_mst_packet_handler_init_, DI_GS_mst_packet_handler_);
}

AppInfo DI_GS_rpt_packet_handler(void)
{
  return AI_create_app_info("GS_RPT", DI_GS_rpt_packet_handler_init_, DI_GS_rpt_packet_handler_);
}

static void DI_GS_cmd_packet_handler_init_(void)
{
  int ret = GS_init(&gs_driver_, PORT_CH_RS422_MOBC_EXT);

  if (ret != 0)
  {
    Printf("!! GS Init Error %d !!\n", ret);
  }
}

static void DI_GS_cmd_packet_handler_(void)
{
  uint8_t select;

  GS_rec_tcf(&gs_driver_);

  // Tlm更新
  for (select = 0; select < CCSDS_SELECT_NUM; ++select)
  {
    CCSDS_read_sequence((uint32_t)select, &gs_driver_.ccsds_info.uip_stat[select]);
  }
  // TODO: エラー処理
}

static void DI_GS_mst_packet_handler_init_(void)
{
  T2M_initialize(&DI_GS_ms_tlm_packet_handler_.tc_packet_to_m_pdu);
}

static void DI_GS_mst_packet_handler_(void)
{
  int i;

  // 本当なら max(今の FIFO の空き, 残り時間で実行可能な数) とかしたい
  for (i = 0; i < CCSDS_FIFO_SIZE; ++i)
  {
    T2M_ACK ack = T2M_form_m_pdu(&DI_GS_ms_tlm_packet_handler_.tc_packet_to_m_pdu, &PH_ms_tlm_list, &DI_GS_ms_tlm_packet_handler_.vcdu.m_pdu);
    if (ack != T2M_SUCCESS) return;

    // Realtime VCDU カウンタの設定
    VCDU_setup_realtime_vcdu_hdr(&DI_GS_ms_tlm_packet_handler_.vcdu, DI_GS_ms_tlm_packet_handler_.vcdu_counter);
    DI_GS_ms_tlm_packet_handler_.vcdu_counter = VCDU_calc_next_counter(DI_GS_ms_tlm_packet_handler_.vcdu_counter);

    // CLCW の設定
    // CMD の VCID と TLM の VCID は独立で関係がない
    // TLM の VCID 種別（Realtime, Replay)によらず CLCW を設定して良い
    // CLCW が対応する CMD の VCID は CLCW の内部で指定される
    VCDU_set_clcw(&DI_GS_ms_tlm_packet_handler_.vcdu, GS_form_clcw());

    // 完成した VCDU を MS VCDU として送出
    GS_send_vcdu(&gs_driver_, &DI_GS_ms_tlm_packet_handler_.vcdu);
  }
}

static void DI_GS_rpt_packet_handler_init_(void)
{
  T2M_initialize(&DI_GS_rp_tlm_packet_handler_.tc_packet_to_m_pdu);
}

static void DI_GS_rpt_packet_handler_(void)
{
  int i;

  // 本当なら max(今の FIFO の空き, 残り時間で実行可能な数) とかしたい
  for (i = 0; i < CCSDS_FIFO_SIZE; ++i)
  {
    T2M_ACK ack = T2M_form_m_pdu(&DI_GS_rp_tlm_packet_handler_.tc_packet_to_m_pdu, &PH_rp_tlm_list, &DI_GS_rp_tlm_packet_handler_.vcdu.m_pdu);
    if (ack != T2M_SUCCESS) return;

    // Replay VCDU カウンタの設定
    VCDU_setup_replay_vcdu_hdr(&DI_GS_rp_tlm_packet_handler_.vcdu, DI_GS_rp_tlm_packet_handler_.vcdu_counter);
    DI_GS_rp_tlm_packet_handler_.vcdu_counter = VCDU_calc_next_counter(DI_GS_rp_tlm_packet_handler_.vcdu_counter);

    // CLCW の設定
    // CMD の VCIDと TLM の VCID は独立で関係がない
    // TLM の VCID 種別（Realtime, Replay) によらず CLCW を設定して良い
    // CLCW が対応する CMD の VCID は CLCW の内部で指定される
    VCDU_set_clcw(&DI_GS_rp_tlm_packet_handler_.vcdu, GS_form_clcw());

    // 完成した VCDU を RP VCDU として送出
    GS_send_vcdu(&gs_driver_, &DI_GS_rp_tlm_packet_handler_.vcdu);
  }
}

static void DI_GS_set_t2m_flush_interval_(cycle_t flush_interval, DI_GS_TlmPacketHandler* gs_tlm_packet_handler)
{
  gs_tlm_packet_handler->tc_packet_to_m_pdu.flush_interval = flush_interval;
}

CCP_EXEC_STS Cmd_DI_GS_CCSDS_TX_START(const CTCP* packet)
{
  (void)packet;
  gs_driver_.is_ccsds_tx_valid = 1;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_CCSDS_TX_STOP(const CTCP* packet)
{
  (void)packet;
  gs_driver_.is_ccsds_tx_valid = 0;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_DRIVER_RESET(const CTCP* packet)
{
  (void)packet;
  if (GS_init(&gs_driver_, PORT_CH_RS422_MOBC_EXT)) return CCP_EXEC_ILLEGAL_CONTEXT;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_SET_MS_FLUSH_INTERVAL(const CTCP* packet)
{
  cycle_t flush_interval;
  endian_memcpy(&flush_interval, CCP_get_param_head(packet), sizeof(cycle_t));

  DI_GS_set_t2m_flush_interval_(flush_interval, &DI_GS_ms_tlm_packet_handler_);

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_SET_RP_FLUSH_INTERVAL(const CTCP* packet)
{
  cycle_t flush_interval;
  endian_memcpy(&flush_interval, CCP_get_param_head(packet), sizeof(cycle_t));

  DI_GS_set_t2m_flush_interval_(flush_interval, &DI_GS_rp_tlm_packet_handler_);

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_SET_FARM_PW(const CTCP* packet)
{
  uint8_t pw = CCP_get_param_head(packet)[0];
  if (pw < 1 || pw > 127) return CCP_EXEC_ILLEGAL_PARAMETER;
  GS_set_farm_pw(pw);

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_SET_INFO(const CTCP* packet)
{
  uint8_t which = CCP_get_param_head(packet)[0];
  if (which >= GS_PORT_TYPE_NUM) return CCP_EXEC_ILLEGAL_PARAMETER;
  gs_driver_.latest_info = &gs_driver_.info[(GS_PORT_TYPE)which];
  gs_driver_.tlm_tx_port_type = (GS_PORT_TYPE)which;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_CCSDS_READ_SEQUENCE(const CTCP* packet)
{
  uint32_t select;
  (void)packet;
  for (select = 0; select < CCSDS_SELECT_NUM; ++select)
  {
    CCSDS_read_sequence(select, &gs_driver_.ccsds_info.uip_stat[select]);
  }

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_CCSDS_GET_BUFFER(const CTCP* packet)
{
  (void)packet;
  gs_driver_.ccsds_info.buffer_num = CCSDS_get_buffer_num();

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_GS_CCSDS_SET_RATE(const CTCP* packet)
{
  uint32_t ui_rate = (uint32_t)CCP_get_param_head(packet)[0];
  if (ui_rate == 0) return CCP_EXEC_ILLEGAL_PARAMETER;

  CCSDS_set_rate(ui_rate, &gs_driver_.driver_ccsds.ccsds_config);

  return CCP_EXEC_SUCCESS;
}

#pragma section
