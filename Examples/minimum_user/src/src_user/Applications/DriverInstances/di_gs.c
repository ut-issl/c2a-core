#pragma section REPRO
/**
 * @file
 * @brief GS Driver のインスタンス化
 */
#include "di_gs.h"
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include <src_core/Library/result.h>
#include "../../Drivers/Com/gs_validate.h"
#include "../../Settings/port_config.h"
#include "../../Settings/DriverSuper/driver_buffer_define.h"

static RESULT DI_GS_init_(void);

// 以下 init と update の定義
static void DI_GS_cmd_packet_handler_app_init_(void);
static void DI_GS_cmd_packet_handler_app_(void);

static void DI_GS_rt_tlm_packet_handler_app_init_(void);
static void DI_GS_rt_tlm_packet_handler_app_(void);
static void DI_GS_rp_tlm_packet_handler_app_init_(void);
static void DI_GS_rp_tlm_packet_handler_app_(void);

static void DI_GS_set_t2m_flush_interval_(cycle_t flush_interval, DI_GS_TlmPacketHandler* gs_tlm_packet_handler);

static GS_Driver gs_driver_;
const GS_Driver* const gs_driver = &gs_driver_;

static DI_GS_TlmPacketHandler DI_GS_rt_tlm_packet_handler_;
const DI_GS_TlmPacketHandler* const DI_GS_rt_tlm_packet_handler = &DI_GS_rt_tlm_packet_handler_;
static DI_GS_TlmPacketHandler DI_GS_rp_tlm_packet_handler_;
const DI_GS_TlmPacketHandler* const DI_GS_rp_tlm_packet_handler = &DI_GS_rp_tlm_packet_handler_;

// バッファ
static DS_StreamRecBuffer DI_GS_ccsds_rx_buffer_[GS_RX_HEADER_NUM];
static uint8_t DI_GS_ccsds_rx_buffer_allocation_[GS_RX_HEADER_NUM][DS_STREAM_REC_BUFFER_SIZE_DEFAULT];
static DS_StreamRecBuffer DI_GS_uart_rx_buffer_[GS_RX_HEADER_NUM];
static uint8_t DI_GS_uart_rx_buffer_allocation_[GS_RX_HEADER_NUM][DS_STREAM_REC_BUFFER_SIZE_DEFAULT];


static RESULT DI_GS_init_(void)
{
  int stream;
  DS_INIT_ERR_CODE ret;
  DS_StreamRecBuffer* ccsds_rx_buffers[DS_STREAM_MAX];
  DS_StreamRecBuffer* uart_rx_buffers[DS_STREAM_MAX];
  DS_nullify_stream_rec_buffers(ccsds_rx_buffers);
  DS_nullify_stream_rec_buffers(uart_rx_buffers);

  // GS_RX_HEADER_NUM > DS_STREAM_MAX のアサーションは gs.c でやっているのでここではしない
  for (stream = 0; stream < GS_RX_HEADER_NUM; ++stream)
  {
    DS_ERR_CODE ret1;
    DS_ERR_CODE ret2;
    ret1 = DS_init_stream_rec_buffer(&DI_GS_ccsds_rx_buffer_[stream],
                                     DI_GS_ccsds_rx_buffer_allocation_[stream],
                                     sizeof(DI_GS_ccsds_rx_buffer_allocation_[stream]));
    ret2 = DS_init_stream_rec_buffer(&DI_GS_uart_rx_buffer_[stream],
                                     DI_GS_uart_rx_buffer_allocation_[stream],
                                     sizeof(DI_GS_uart_rx_buffer_allocation_[stream]));
    if (ret1 != DS_ERR_CODE_OK || ret2 != DS_ERR_CODE_OK)
    {
      Printf("GS buffer init Failed ! %d, %d \n", ret1, ret2);
      return RESULT_ERR;
    }
    ccsds_rx_buffers[stream] = &DI_GS_ccsds_rx_buffer_[stream];
    uart_rx_buffers[stream]  = &DI_GS_uart_rx_buffer_[stream];
  }

  ret = GS_init(&gs_driver_, PORT_CH_RS422_MOBC_EXT, ccsds_rx_buffers, uart_rx_buffers);

  if (ret != DS_INIT_OK)
  {
    Printf("!! GS Init Error %d !!\n", ret);
    return RESULT_ERR;
  }

  return RESULT_OK;
}

AppInfo DI_GS_cmd_packet_handler_app(void)
{
  return AI_create_app_info("GS_CMD", DI_GS_cmd_packet_handler_app_init_, DI_GS_cmd_packet_handler_app_);
}

AppInfo DI_GS_rt_tlm_packet_handler_app(void)
{
  return AI_create_app_info("GS_RT_TLM", DI_GS_rt_tlm_packet_handler_app_init_, DI_GS_rt_tlm_packet_handler_app_);
}

AppInfo DI_GS_rp_tlm_packet_handler_app(void)
{
  return AI_create_app_info("GS_RP_TLM", DI_GS_rp_tlm_packet_handler_app_init_, DI_GS_rp_tlm_packet_handler_app_);
}

static void DI_GS_cmd_packet_handler_app_init_(void)
{
  DI_GS_init_();
}

static void DI_GS_cmd_packet_handler_app_(void)
{
  GS_rec_tctf(&gs_driver_);
  // TODO: エラー処理
}

static void DI_GS_rt_tlm_packet_handler_app_init_(void)
{
  T2M_initialize(&DI_GS_rt_tlm_packet_handler_.tc_packet_to_m_pdu);
}

static void DI_GS_rt_tlm_packet_handler_app_(void)
{
  int i;

  // 本当なら max(今の FIFO の空き, 残り時間で実行可能な数) とかしたい
  for (i = 0; i < CCSDS_FIFO_SIZE; ++i)
  {
    T2M_ACK ack = T2M_form_m_pdu(&DI_GS_rt_tlm_packet_handler_.tc_packet_to_m_pdu,
                                 &PH_rt_tlm_list,
                                 &DI_GS_rt_tlm_packet_handler_.vcdu.m_pdu);
    if (ack != T2M_SUCCESS) return;

    // Realtime VCDU カウンタの設定
    VCDU_setup_realtime_vcdu_hdr(&DI_GS_rt_tlm_packet_handler_.vcdu, DI_GS_rt_tlm_packet_handler_.vcdu_counter);
    DI_GS_rt_tlm_packet_handler_.vcdu_counter = VCDU_calc_next_counter(DI_GS_rt_tlm_packet_handler_.vcdu_counter);

    // CLCW の設定
    // CMD の VCID と TLM の VCID は独立で関係がない
    // TLM の VCID 種別（Realtime, Replay)によらず CLCW を設定して良い
    // CLCW が対応する CMD の VCID は CLCW の内部で指定される
    VCDU_set_clcw(&DI_GS_rt_tlm_packet_handler_.vcdu, GS_form_clcw());

    // 完成した VCDU を RT VCDU として送出
    GS_send_vcdu(&gs_driver_, &DI_GS_rt_tlm_packet_handler_.vcdu);
  }
}

static void DI_GS_rp_tlm_packet_handler_app_init_(void)
{
  T2M_initialize(&DI_GS_rp_tlm_packet_handler_.tc_packet_to_m_pdu);
}

static void DI_GS_rp_tlm_packet_handler_app_(void)
{
  int i;

  // 本当なら max(今の FIFO の空き, 残り時間で実行可能な数) とかしたい
  for (i = 0; i < CCSDS_FIFO_SIZE; ++i)
  {
    T2M_ACK ack = T2M_form_m_pdu(&DI_GS_rp_tlm_packet_handler_.tc_packet_to_m_pdu,
                                 &PH_rp_tlm_list,
                                 &DI_GS_rp_tlm_packet_handler_.vcdu.m_pdu);
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

CCP_CmdRet Cmd_DI_GS_DRIVER_RESET(const CommonCmdPacket* packet)
{
  (void)packet;
  if (DI_GS_init_() != RESULT_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_GS_SET_RT_FLUSH_INTERVAL(const CommonCmdPacket* packet)
{
  cycle_t flush_interval;
  ENDIAN_memcpy(&flush_interval, CCP_get_param_head(packet), sizeof(cycle_t));

  DI_GS_set_t2m_flush_interval_(flush_interval, &DI_GS_rt_tlm_packet_handler_);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_GS_SET_RP_FLUSH_INTERVAL(const CommonCmdPacket* packet)
{
  cycle_t flush_interval;
  ENDIAN_memcpy(&flush_interval, CCP_get_param_head(packet), sizeof(cycle_t));

  DI_GS_set_t2m_flush_interval_(flush_interval, &DI_GS_rp_tlm_packet_handler_);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_GS_SET_FARM_PW(const CommonCmdPacket* packet)
{
  uint8_t pw = CCP_get_param_head(packet)[0];
  if (pw < 1 || pw > 127) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  GS_set_farm_pw(pw);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_GS_SET_INFO(const CommonCmdPacket* packet)
{
  uint8_t which = CCP_get_param_head(packet)[0];
  if (which >= GS_PORT_TYPE_NUM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  gs_driver_.latest_info = &gs_driver_.info[(GS_PORT_TYPE)which];
  gs_driver_.tlm_tx_port_type = (GS_PORT_TYPE)which;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_GS_CCSDS_GET_BUFFER(const CommonCmdPacket* packet)
{
  (void)packet;
  gs_driver_.ccsds_info.buffer_num = CCSDS_get_buffer_num();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_GS_CCSDS_SET_RATE(const CommonCmdPacket* packet)
{
  uint32_t ui_rate = (uint32_t)CCP_get_param_head(packet)[0];
  if (ui_rate == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  CCSDS_set_rate(ui_rate, &gs_driver_.driver_ccsds.ccsds_config);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_GS_UART_TLM_ON(const CommonCmdPacket* packet)
{
  (void)packet;
  gs_driver_.driver_uart.is_tlm_on = 1;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_GS_UART_TLM_OFF(const CommonCmdPacket* packet)
{
  (void)packet;
  gs_driver_.driver_uart.is_tlm_on = 0;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
