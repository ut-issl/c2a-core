#pragma section REPRO
/**
 * @file
 * @brief  テレメトリバッファー（テレメ中継）
 * @note   このコードは自動生成されています！
 */

#include "./aobc_telemetry_definitions.h"
#include "./aobc_telemetry_buffer.h"
#include <string.h> // for memcpy

static AOBC_Buffer aobc_buffer_;
const AOBC_Buffer* const aobc_buffer = &aobc_buffer_;

static DS_ERR_CODE AOBC_analyze_tlm_aobc_aobc_(DS_StreamConfig* p_stream_config, AOBC_Driver* aobc_driver);
static DS_ERR_CODE AOBC_analyze_tlm_aobc_hk_(DS_StreamConfig* p_stream_config, AOBC_Driver* aobc_driver);

void AOBC_buffer_init(void)
{
  aobc_buffer_.aobc_aobc.size = 0;
  aobc_buffer_.aobc_hk.size = 0;
}

DS_ERR_CODE AOBC_buffer_tlm_contents(DS_StreamConfig* p_stream_config, AOBC_Driver* aobc_driver)
{
  uint8_t tlm_id  = DS_C2AFMT_get_tlm_id(p_stream_config);

  switch (tlm_id)
  {
  case AOBC_Tlm_CODE_AOBC_AOBC:
    return AOBC_analyze_tlm_aobc_aobc_(p_stream_config, aobc_driver);
  case AOBC_Tlm_CODE_AOBC_HK:
    return AOBC_analyze_tlm_aobc_hk_(p_stream_config, aobc_driver);
  default:
    aobc_driver->info.comm.rx_err_code = AOBC_RX_ERR_CODE_TLM_NOT_FOUND;
    return DS_ERR_CODE_ERR;
  }
}

static DS_ERR_CODE AOBC_analyze_tlm_aobc_aobc_(DS_StreamConfig* p_stream_config, AOBC_Driver* aobc_driver)
{
  uint32_t tlm_len = DS_ISSLFMT_get_tlm_length(p_stream_config);
  const uint8_t* f = DSSC_get_rx_frame(p_stream_config) + DS_ISSLFMT_COMMON_HEADER_SIZE;
  uint32_t contents_len = tlm_len - DS_C2AFMT_TCP_TLM_SECONDARY_HEADER_SIZE - 1;      // FIXME: CCSDSは1起算？
  const uint8_t* contents_pos = f + DS_C2AFMT_TCP_TLM_PRIMARY_HEADER_SIZE + DS_C2AFMT_TCP_TLM_SECONDARY_HEADER_SIZE;
  int8_t temp_i8 = 0;
  int16_t temp_i16 = 0;
  int32_t temp_i32 = 0;
  uint8_t temp_u8 = 0;
  uint16_t temp_u16 = 0;
  uint32_t temp_u32 = 0;
  float temp_f = 0.0f;
  double temp_d = 0.0;

  // GSへのテレメ中継のためのバッファーへのコピー
  if (contents_len > AOBC_TELEMETRY_BUFFE_SIZE) return DS_ERR_CODE_ERR;
  memcpy(aobc_buffer_.aobc_aobc.buffer, contents_pos, (size_t)contents_len);
  aobc_buffer_.aobc_aobc.size = (int)contents_len;

  // MOBC内部でテレメデータへアクセスしやすいようにするための構造体へのパース
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 7;
  temp_u16 &= 0x7;
  aobc_driver->tlm_data.aobc_aobc.ph.ver = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 4;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_aobc.ph.type = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 3;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_aobc.ph.sh_flag = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 2;
  temp_u16 &= 0x7ff;
  aobc_driver->tlm_data.aobc_aobc.ph.apid = temp_u16;
  endian_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 7;
  temp_u16 &= 0x3;
  aobc_driver->tlm_data.aobc_aobc.ph.seq_flag = temp_u16;
  endian_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 5;
  temp_u16 &= 0x3fff;
  aobc_driver->tlm_data.aobc_aobc.ph.seq_count = temp_u16;
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.ph.packet_len), &(f[4]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.ver), &(f[6]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.ti), &(f[7]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.tlm_id), &(f[11]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.global_time), &(f[12]), 8);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.on_board_subnet_time), &(f[20]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.dest_flags), &(f[24]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.dr_partition), &(f[25]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tm_mode_time), &(f[26]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tm_utl_unixtime_epoch), &(f[30]), 8);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tm_cycles_per_sec_fix_ratio), &(f[38]), 8);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.mm_sts), &(f[46]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.mm_opsmode), &(f[47]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.mm_opsmode_prev), &(f[48]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp_current_id), &(f[49]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp_cmd_last_err_time_master), &(f[50]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp_cmd_last_err_time_step), &(f[54]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp_cmd_last_err_id), &(f[55]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp_cmd_last_err_sts), &(f[57]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_ret_from_if_rx), &(f[61]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tcf_last_recv_ack), &(f[65]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tcf_validate_status), &(f[66]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tcf_last_recv_time), &(f[67]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tcf_farm_pw), &(f[71]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tcp_last_recv_ack), &(f[72]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlm_tx_port_type), &(f[73]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_counter), &(f[74]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_exec_time), &(f[78]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_exec_id), &(f[82]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_exec_sts), &(f[84]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_err_time), &(f[88]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_err_id), &(f[92]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_err_sts), &(f[94]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_err_counter), &(f[98]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_counter), &(f[102]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_exec_time), &(f[106]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_exec_id), &(f[110]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_exec_sts), &(f[112]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_err_time), &(f[116]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_err_id), &(f[120]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_err_sts), &(f[122]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_err_counter), &(f[126]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_counter), &(f[130]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_queued), &(f[134]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_last_exec_time), &(f[135]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_last_exec_id), &(f[139]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_last_exec_sts), &(f[141]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_last_err_time), &(f[145]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_last_err_id), &(f[149]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_last_err_sts), &(f[151]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_err_counter), &(f[155]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_soe_flag), &(f[159]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_cmd_lockout_flag), &(f[160]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_next_time), &(f[161]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl0_next_id), &(f[165]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_counter), &(f[167]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_queued), &(f[171]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_last_exec_time), &(f[172]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_last_exec_id), &(f[176]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_last_exec_sts), &(f[178]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_last_err_time), &(f[182]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_last_err_id), &(f[186]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_last_err_sts), &(f[188]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_err_counter), &(f[192]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_soe_flag), &(f[196]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_cmd_lockout_flag), &(f[197]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_next_time), &(f[198]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl1_next_id), &(f[202]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_counter), &(f[204]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_queued), &(f[208]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_last_exec_time), &(f[209]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_last_exec_id), &(f[213]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_last_exec_sts), &(f[215]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_last_err_time), &(f[219]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_last_err_id), &(f[223]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_last_err_sts), &(f[225]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_err_counter), &(f[229]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_soe_flag), &(f[233]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_cmd_lockout_flag), &(f[234]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_next_time), &(f[235]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tl2_next_id), &(f[239]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_blk_ptr), &(f[241]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_cmd_ptr), &(f[242]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_regd_time), &(f[243]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_regd_id), &(f[247]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.ms_buffer_available), &(f[249]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.ms_t2m_flush_interval), &(f[250]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rp_t2m_flush_interval), &(f[254]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.ms_tlm_counter), &(f[258]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.ms_tlm_queued), &(f[262]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.st_tlm_counter), &(f[263]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.st_tlm_queued), &(f[267]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rp_tlm_counter), &(f[268]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rp_tlm_queued), &(f[272]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tci_tx_cycle), &(f[273]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tci_tx_bitrate), &(f[277]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tci_vcid), &(f[281]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tci_vcdu_counter), &(f[282]), 4);
  // TODO: ビットフィールドをつかっている系は，様々なパターンがあり得るので，今後，バグが出ないか注視する

  // ワーニング回避
  (void)temp_i8;
  (void)temp_i16;
  (void)temp_i32;
  (void)temp_u8;
  (void)temp_u16;
  (void)temp_u32;
  (void)temp_f;
  (void)temp_d;

  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE AOBC_analyze_tlm_aobc_hk_(DS_StreamConfig* p_stream_config, AOBC_Driver* aobc_driver)
{
  uint32_t tlm_len = DS_ISSLFMT_get_tlm_length(p_stream_config);
  const uint8_t* f = DSSC_get_rx_frame(p_stream_config) + DS_ISSLFMT_COMMON_HEADER_SIZE;
  uint32_t contents_len = tlm_len - DS_C2AFMT_TCP_TLM_SECONDARY_HEADER_SIZE - 1;      // FIXME: CCSDSは1起算？
  const uint8_t* contents_pos = f + DS_C2AFMT_TCP_TLM_PRIMARY_HEADER_SIZE + DS_C2AFMT_TCP_TLM_SECONDARY_HEADER_SIZE;
  int8_t temp_i8 = 0;
  int16_t temp_i16 = 0;
  int32_t temp_i32 = 0;
  uint8_t temp_u8 = 0;
  uint16_t temp_u16 = 0;
  uint32_t temp_u32 = 0;
  float temp_f = 0.0f;
  double temp_d = 0.0;

  // GSへのテレメ中継のためのバッファーへのコピー
  if (contents_len > AOBC_TELEMETRY_BUFFE_SIZE) return DS_ERR_CODE_ERR;
  memcpy(aobc_buffer_.aobc_hk.buffer, contents_pos, (size_t)contents_len);
  aobc_buffer_.aobc_hk.size = (int)contents_len;

  // MOBC内部でテレメデータへアクセスしやすいようにするための構造体へのパース
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 7;
  temp_u16 &= 0x7;
  aobc_driver->tlm_data.aobc_hk.ph.ver = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 4;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.ph.type = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 3;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.ph.sh_flag = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 2;
  temp_u16 &= 0x7ff;
  aobc_driver->tlm_data.aobc_hk.ph.apid = temp_u16;
  endian_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 7;
  temp_u16 &= 0x3;
  aobc_driver->tlm_data.aobc_hk.ph.seq_flag = temp_u16;
  endian_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 5;
  temp_u16 &= 0x3fff;
  aobc_driver->tlm_data.aobc_hk.ph.seq_count = temp_u16;
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.ph.packet_len), &(f[4]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.ver), &(f[6]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.ti), &(f[7]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.tlm_id), &(f[11]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.global_time), &(f[12]), 8);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.on_board_subnet_time), &(f[20]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.dest_flags), &(f[24]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.dr_partition), &(f[25]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tm_mode_time), &(f[26]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tm_unixtime_at_ti0), &(f[30]), 8);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_mm_opsmode), &(f[38]), 1);
  endian_memcpy(&temp_u8, &(f[39]), 1);
  temp_u8 >>= 7;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_mm_sts = temp_u8;
  endian_memcpy(&temp_u8, &(f[39]), 1);
  temp_u8 >>= 6;
  temp_u8 &= 0x7f;
  aobc_driver->tlm_data.aobc_hk.obc_mm_opsmode_prev = temp_u8;
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tdsp_current_id), &(f[40]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tcf_last_recv_ack), &(f[42]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tcf_last_recv_time), &(f[43]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tcp_last_recv_ack), &(f[47]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_counter), &(f[48]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_exec_time), &(f[52]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_exec_id), &(f[56]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_exec_sts), &(f[58]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_err_time), &(f[59]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_err_id), &(f[63]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_err_sts), &(f[65]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_counter), &(f[66]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_queued), &(f[70]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_last_exec_time), &(f[71]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_last_exec_id), &(f[75]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_last_exec_sts), &(f[77]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_last_err_time), &(f[78]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_last_err_id), &(f[82]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_last_err_sts), &(f[84]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_next_time), &(f[85]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl0_next_id), &(f[89]), 2);
  endian_memcpy(&temp_u8, &(f[91]), 1);
  temp_u8 >>= 7;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_soe_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[91]), 1);
  temp_u8 >>= 6;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tl0_cmd_lockout_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[91]), 1);
  temp_u8 >>= 5;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tl1_cmd_soe_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[91]), 1);
  temp_u8 >>= 4;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tl1_cmd_lockout_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[91]), 1);
  temp_u8 >>= 3;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tl2_cmd_soe_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[91]), 1);
  temp_u8 >>= 2;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tl2_cmd_lockout_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[91]), 1);
  temp_u8 >>= 1;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.wdt.is_enable = temp_u8;
  endian_memcpy(&temp_u8, &(f[91]), 1);
  temp_u8 >>= 0;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.wdt.is_clear_enable = temp_u8;
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_bct_blk_ptr), &(f[92]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_bct_cmd_ptr), &(f[94]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_bct_regd_time), &(f[95]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_bct_regd_id), &(f[99]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counter_total), &(f[101]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_high), &(f[105]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_middle), &(f[107]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_low), &(f[109]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_el), &(f[111]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_eh), &(f[113]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.group), &(f[115]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.local), &(f[116]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.err_level), &(f[120]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.time.total_cycle), &(f[121]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.time.step), &(f[125]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.note), &(f[126]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.group), &(f[128]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.local), &(f[129]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.time.total_cycle), &(f[133]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.time.step), &(f[137]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.note), &(f[138]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.eh.log_table.respond_counter), &(f[140]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.anomaly_mobc_al_counter), &(f[144]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.anomaly_mobc_al_latest_time_master), &(f[148]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.anomaly_mobc_al_latest_time_step), &(f[152]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.anomaly_mobc_al_latest_code_group), &(f[153]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.anomaly_mobc_al_latest_code_local), &(f[154]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.anomaly_mobc_al_latest_run_length), &(f[158]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.anomaly_mobc_al_header), &(f[159]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl1_cmd_counter), &(f[160]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl2_cmd_counter), &(f[164]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl1_cmd_queued), &(f[168]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tl2_cmd_queued), &(f[169]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_ah_action_counter), &(f[170]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_ah_latest_id), &(f[171]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_ah_respond_at), &(f[172]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_err_counter), &(f[176]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs0.cmd_code), &(f[177]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs0.status), &(f[179]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs0.exec_counter), &(f[180]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs0.last_exec_sts), &(f[182]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs0.last_exec_time.total_cycle), &(f[183]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs1.cmd_code), &(f[187]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs1.status), &(f[189]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs1.exec_counter), &(f[190]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs1.last_exec_sts), &(f[192]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dcu.exec_logs1.last_exec_time.total_cycle), &(f[193]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.git_rev_core), &(f[197]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.git_rev_user), &(f[201]), 4);
  // TODO: ビットフィールドをつかっている系は，様々なパターンがあり得るので，今後，バグが出ないか注視する

  // ワーニング回避
  (void)temp_i8;
  (void)temp_i16;
  (void)temp_i32;
  (void)temp_u8;
  (void)temp_u16;
  (void)temp_u32;
  (void)temp_f;
  (void)temp_d;

  return DS_ERR_CODE_OK;
}

#pragma section
