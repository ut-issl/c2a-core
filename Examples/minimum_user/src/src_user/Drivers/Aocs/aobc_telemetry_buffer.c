#pragma section REPRO
/**
 * @file
 * @brief  テレメトリバッファー（テレメ中継）
 * @note   このコードは自動生成されています！
 */
#include "./aobc_telemetry_definitions.h"
#include "./aobc_telemetry_buffer.h"
#include "./aobc.h"
#include <string.h>

static DS_ERR_CODE AOBC_analyze_tlm_aobc_aobc_(const CommonTlmPacket* packet, AOBC_TLM_CODE tlm_id, AOBC_Driver* aobc_driver);
static DS_ERR_CODE AOBC_analyze_tlm_aobc_hk_(const CommonTlmPacket* packet, AOBC_TLM_CODE tlm_id, AOBC_Driver* aobc_driver);

static CommonTlmPacket AOBC_ctp_;

void AOBC_init_tlm_buffer(AOBC_Driver* aobc_driver)
{
  // packet などは，上位の driver の初期化で driver もろとも memset 0x00 されていると期待して，ここではしない
  int i = 0;
  for (i = 0; i < AOBC_MAX_TLM_NUM; ++i)
  {
    aobc_driver->tlm_buffer.tlm[i].is_null_packet = 1;
  }
}

DS_ERR_CODE AOBC_buffer_tlm_packet(DS_StreamConfig* p_stream_config, AOBC_Driver* aobc_driver)
{
  AOBC_TLM_CODE tlm_id;
  DS_ERR_CODE ret;

  ret = DS_C2AFMT_get_ctp(p_stream_config, &AOBC_ctp_);
  if (ret != DS_ERR_CODE_OK) return ret;

  tlm_id  = (AOBC_TLM_CODE)CTP_get_id(&AOBC_ctp_);

  switch (tlm_id)
  {
  case AOBC_Tlm_CODE_AOBC_AOBC:
    return AOBC_analyze_tlm_aobc_aobc_(&AOBC_ctp_, tlm_id, aobc_driver);
  case AOBC_Tlm_CODE_AOBC_HK:
    return AOBC_analyze_tlm_aobc_hk_(&AOBC_ctp_, tlm_id, aobc_driver);
  default:
    aobc_driver->info.comm.rx_err_code = AOBC_RX_ERR_CODE_TLM_NOT_FOUND;
    return DS_ERR_CODE_ERR;
  }
}

static DS_ERR_CODE AOBC_analyze_tlm_aobc_aobc_(const CommonTlmPacket* packet, AOBC_TLM_CODE tlm_id, AOBC_Driver* aobc_driver)
{
  const uint8_t* f = packet->packet;
  int8_t temp_i8 = 0;
  int16_t temp_i16 = 0;
  int32_t temp_i32 = 0;
  uint8_t temp_u8 = 0;
  uint16_t temp_u16 = 0;
  uint32_t temp_u32 = 0;
  float temp_f = 0.0f;
  double temp_d = 0.0;

  // GS へのテレメ中継のためのバッファーへのコピー
  CTP_copy_packet(&(aobc_driver->tlm_buffer.tlm[tlm_id].packet), packet);
  aobc_driver->tlm_buffer.tlm[tlm_id].is_null_packet = 0;
  // TODO: CRC チェック

  // MOBC 内部でテレメデータへアクセスしやすいようにするための構造体へのパース
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 13;
  temp_u16 &= 0x7;
  aobc_driver->tlm_data.aobc_aobc.ph.ver = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 12;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_aobc.ph.type = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 11;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_aobc.ph.sh_flag = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 0;
  temp_u16 &= 0x7ff;
  aobc_driver->tlm_data.aobc_aobc.ph.apid = temp_u16;
  endian_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 14;
  temp_u16 &= 0x3;
  aobc_driver->tlm_data.aobc_aobc.ph.seq_flag = temp_u16;
  endian_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 0;
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
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_counter), &(f[61]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_exec_time), &(f[65]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_exec_id), &(f[69]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_exec_sts), &(f[71]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_err_time), &(f[75]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_err_id), &(f[79]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_last_err_sts), &(f[81]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd_err_counter), &(f[85]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_counter), &(f[89]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_exec_time), &(f[93]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_exec_id), &(f[97]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_exec_sts), &(f[99]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_err_time), &(f[103]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_err_id), &(f[107]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_last_err_sts), &(f[109]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd_err_counter), &(f[113]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.counter), &(f[117]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.queued), &(f[121]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_exec_time), &(f[122]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_exec_id), &(f[126]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_exec_sts), &(f[128]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_err_time), &(f[132]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_err_id), &(f[136]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_err_sts), &(f[138]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.err_counter), &(f[142]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.soe_flag), &(f[146]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.lockout_flag), &(f[147]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.next_time), &(f[148]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.next_id), &(f[152]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.counter), &(f[154]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.queued), &(f[158]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_exec_time), &(f[159]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_exec_id), &(f[163]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_exec_sts), &(f[165]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_err_time), &(f[169]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_err_id), &(f[173]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_err_sts), &(f[175]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.err_counter), &(f[179]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.soe_flag), &(f[183]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.lockout_flag), &(f[184]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.next_time), &(f[185]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.next_id), &(f[189]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.counter), &(f[191]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.queued), &(f[195]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_exec_time), &(f[196]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_exec_id), &(f[200]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_exec_sts), &(f[202]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_err_time), &(f[206]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_err_id), &(f[210]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_err_sts), &(f[212]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.err_counter), &(f[216]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.soe_flag), &(f[220]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.lockout_flag), &(f[221]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.next_time), &(f[222]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.next_id), &(f[226]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_blk_ptr), &(f[228]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_cmd_ptr), &(f[229]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_regd_time), &(f[230]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_regd_id), &(f[234]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.ms_tlm_counter), &(f[236]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_aobc.ms_tlm_queued), &(f[240]), 1);
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

static DS_ERR_CODE AOBC_analyze_tlm_aobc_hk_(const CommonTlmPacket* packet, AOBC_TLM_CODE tlm_id, AOBC_Driver* aobc_driver)
{
  const uint8_t* f = packet->packet;
  int8_t temp_i8 = 0;
  int16_t temp_i16 = 0;
  int32_t temp_i32 = 0;
  uint8_t temp_u8 = 0;
  uint16_t temp_u16 = 0;
  uint32_t temp_u32 = 0;
  float temp_f = 0.0f;
  double temp_d = 0.0;

  // GS へのテレメ中継のためのバッファーへのコピー
  CTP_copy_packet(&(aobc_driver->tlm_buffer.tlm[tlm_id].packet), packet);
  aobc_driver->tlm_buffer.tlm[tlm_id].is_null_packet = 0;
  // TODO: CRC チェック

  // MOBC 内部でテレメデータへアクセスしやすいようにするための構造体へのパース
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 13;
  temp_u16 &= 0x7;
  aobc_driver->tlm_data.aobc_hk.ph.ver = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 12;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.ph.type = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 11;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.ph.sh_flag = temp_u16;
  endian_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 0;
  temp_u16 &= 0x7ff;
  aobc_driver->tlm_data.aobc_hk.ph.apid = temp_u16;
  endian_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 14;
  temp_u16 &= 0x3;
  aobc_driver->tlm_data.aobc_hk.ph.seq_flag = temp_u16;
  endian_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 0;
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
  temp_u8 >>= 0;
  temp_u8 &= 0x7f;
  aobc_driver->tlm_data.aobc_hk.obc_mm_opsmode_prev = temp_u8;
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tdsp_current_id), &(f[40]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_counter), &(f[42]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_exec_time), &(f[46]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_exec_id), &(f[50]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_exec_sts), &(f[52]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_err_time), &(f[53]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_err_id), &(f[57]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_last_err_sts), &(f[59]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_rt_cmd_counter), &(f[60]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_rt_cmd_last_exec_time), &(f[64]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_rt_cmd_last_exec_id), &(f[68]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_rt_cmd_last_exec_sts), &(f[70]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_rt_cmd_last_err_time), &(f[74]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_rt_cmd_last_err_id), &(f[78]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_rt_cmd_last_err_sts), &(f[80]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.counter), &(f[84]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.queued), &(f[88]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.last_exec_time), &(f[89]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.last_exec_id), &(f[93]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.last_exec_sts), &(f[95]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.last_err_time), &(f[96]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.last_err_id), &(f[100]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.last_err_sts), &(f[102]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.next_time), &(f[103]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.next_id), &(f[107]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_bc.counter), &(f[109]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_bc.queued), &(f[113]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_tlm.counter), &(f[114]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_tlc_tlm.queued), &(f[118]), 1);
  endian_memcpy(&temp_u8, &(f[119]), 1);
  temp_u8 >>= 7;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.soe_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[119]), 1);
  temp_u8 >>= 6;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tlc_gs.lockout_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[119]), 1);
  temp_u8 >>= 5;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tlc_bc.soe_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[119]), 1);
  temp_u8 >>= 4;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tlc_bc.lockout_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[119]), 1);
  temp_u8 >>= 3;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tlc_tlm.soe_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[119]), 1);
  temp_u8 >>= 2;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc_tlc_tlm.lockout_flag = temp_u8;
  endian_memcpy(&temp_u8, &(f[120]), 1);
  temp_u8 >>= 7;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.wdt.is_enable = temp_u8;
  endian_memcpy(&temp_u8, &(f[120]), 1);
  temp_u8 >>= 6;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.wdt.is_clear_enable = temp_u8;
  endian_memcpy(&temp_u8, &(f[120]), 1);
  temp_u8 >>= 0;
  temp_u8 &= 0x3f;
  aobc_driver->tlm_data.aobc_hk.dummy_data0 = temp_u8;
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_bct_blk_ptr), &(f[121]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_bct_cmd_ptr), &(f[123]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_bct_regd_time), &(f[124]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_bct_regd_id), &(f[128]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counter_total), &(f[130]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_high), &(f[134]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_middle), &(f[136]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_low), &(f[138]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_el), &(f[140]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_eh), &(f[142]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.group), &(f[144]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.local), &(f[145]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.err_level), &(f[149]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.time.total_cycle), &(f[150]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.time.step), &(f[154]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.note), &(f[155]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.group), &(f[159]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.local), &(f[160]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.time.total_cycle), &(f[164]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.time.step), &(f[168]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.note), &(f[169]), 2);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.eh.log_table.respond_counter), &(f[171]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc_gs_cmd_err_counter), &(f[175]), 1);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.git_rev_core), &(f[176]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.git_rev_user), &(f[180]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data1), &(f[184]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data2), &(f[188]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data3), &(f[192]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data4), &(f[196]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data5), &(f[200]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data6), &(f[204]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data7), &(f[208]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data8), &(f[212]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data9), &(f[216]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data10), &(f[220]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data11), &(f[224]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data12), &(f[228]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data13), &(f[232]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data14), &(f[236]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data15), &(f[240]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data16), &(f[244]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data17), &(f[248]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data18), &(f[252]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data19), &(f[256]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data20), &(f[260]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data21), &(f[264]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data22), &(f[268]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data23), &(f[272]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data24), &(f[276]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data25), &(f[280]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data26), &(f[284]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data27), &(f[288]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data28), &(f[292]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data29), &(f[296]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data30), &(f[300]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data31), &(f[304]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data32), &(f[308]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data33), &(f[312]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data34), &(f[316]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data35), &(f[320]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data36), &(f[324]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data37), &(f[328]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data38), &(f[332]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data39), &(f[336]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data40), &(f[340]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data41), &(f[344]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data42), &(f[348]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data43), &(f[352]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data44), &(f[356]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data45), &(f[360]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data46), &(f[364]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data47), &(f[368]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data48), &(f[372]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data49), &(f[376]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data50), &(f[380]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data51), &(f[384]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data52), &(f[388]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data53), &(f[392]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data54), &(f[396]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data55), &(f[400]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data56), &(f[404]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data57), &(f[408]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data58), &(f[412]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data59), &(f[416]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data60), &(f[420]), 4);
  endian_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data61), &(f[424]), 4);
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

TF_TLM_FUNC_ACK AOBC_pick_up_tlm_buffer(const AOBC_Driver* aobc_driver, AOBC_TLM_CODE tlm_id, uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  const CommonTlmPacket* buffered_packet;

  if (tlm_id >= AOBC_MAX_TLM_NUM) return TF_TLM_FUNC_ACK_NOT_DEFINED;
  if (aobc_driver->tlm_buffer.tlm[tlm_id].is_null_packet) return TF_TLM_FUNC_ACK_NULL_PACKET;

  buffered_packet = &(aobc_driver->tlm_buffer.tlm[tlm_id].packet);
  *len = CTP_get_packet_len(buffered_packet);

  if (*len > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

  memcpy(packet, &buffered_packet->packet, (size_t)(*len));
  return TF_TLM_FUNC_ACK_SUCCESS;
}

#pragma section
