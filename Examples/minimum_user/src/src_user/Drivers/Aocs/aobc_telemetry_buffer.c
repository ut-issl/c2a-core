#pragma section REPRO
/**
 * @file
 * @brief  テレメトリバッファー（テレメ中継）
 * @note   このコードは自動生成されています！
 */
#include <src_core/Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.h>
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

  ret = CTP_get_ctp_from_dssc(p_stream_config, &AOBC_ctp_);
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
  ENDIAN_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 13;
  temp_u16 &= 0x7;
  aobc_driver->tlm_data.aobc_aobc.ph.ver = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 12;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_aobc.ph.type = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 11;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_aobc.ph.sh_flag = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 0;
  temp_u16 &= 0x7ff;
  aobc_driver->tlm_data.aobc_aobc.ph.apid = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 14;
  temp_u16 &= 0x3;
  aobc_driver->tlm_data.aobc_aobc.ph.seq_flag = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 0;
  temp_u16 &= 0x3fff;
  aobc_driver->tlm_data.aobc_aobc.ph.seq_count = temp_u16;
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.ph.packet_len), &(f[4]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.ver), &(f[6]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.ti), &(f[7]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.tlm_id), &(f[11]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.global_time), &(f[12]), 8);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.on_board_subnet_time), &(f[20]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.dest_flags), &(f[24]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.sh.dest_info), &(f[25]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tm_mode_time), &(f[26]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tm_utl_unixtime_epoch), &(f[30]), 8);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tm_cycles_per_sec_fix_ratio), &(f[38]), 8);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.mm_sts), &(f[46]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.mm_opsmode), &(f[47]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.mm_opsmode_prev), &(f[48]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp.current_id), &(f[49]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp.last_err.time.total_cycle), &(f[50]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp.last_err.time.step), &(f[54]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp.last_err.id), &(f[55]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp.last_err.exec_sts), &(f[57]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tdsp.last_err.err_code), &(f[58]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.counter), &(f[62]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.last_exec.time), &(f[66]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.last_exec.id), &(f[70]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.last_exec.exec_sts), &(f[72]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.last_exec.err_code), &(f[73]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.last_err.time), &(f[77]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.last_err.id), &(f[81]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.last_err.exec_sts), &(f[83]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.last_err.err_code), &(f[84]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.gs_cmd.err_counter), &(f[88]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.counter), &(f[92]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.last_exec.time), &(f[96]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.last_exec.id), &(f[100]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.last_exec.exec_sts), &(f[102]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.last_exec.err_code), &(f[103]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.last_err.time), &(f[107]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.last_err.id), &(f[111]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.last_err.exec_sts), &(f[113]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.last_err.err_code), &(f[114]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_cmd.err_counter), &(f[118]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.counter), &(f[122]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.queued), &(f[126]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_exec.time), &(f[127]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_exec.id), &(f[131]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_exec.exec_sts), &(f[133]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_exec.err_code), &(f[134]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_err.time), &(f[138]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_err.id), &(f[142]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_err.exec_sts), &(f[144]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.last_err.err_code), &(f[145]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.err_counter), &(f[149]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.soe_flag), &(f[153]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.lockout_flag), &(f[154]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.next_time), &(f[155]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_gs.next_id), &(f[159]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.counter), &(f[161]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.queued), &(f[165]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_exec.time), &(f[166]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_exec.id), &(f[170]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_exec.exec_sts), &(f[172]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_exec.err_code), &(f[173]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_err.time), &(f[177]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_err.id), &(f[181]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_err.exec_sts), &(f[183]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.last_err.err_code), &(f[184]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.err_counter), &(f[188]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.soe_flag), &(f[192]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.lockout_flag), &(f[193]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.next_time), &(f[194]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_bc.next_id), &(f[198]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.counter), &(f[200]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.queued), &(f[204]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_exec.time), &(f[205]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_exec.id), &(f[209]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_exec.exec_sts), &(f[211]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_exec.err_code), &(f[212]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_err.time), &(f[216]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_err.id), &(f[220]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_err.exec_sts), &(f[222]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.last_err.err_code), &(f[223]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.err_counter), &(f[227]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.soe_flag), &(f[231]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.lockout_flag), &(f[232]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.next_time), &(f[233]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.tlc_tlm.next_id), &(f[237]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_blk_ptr), &(f[239]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_cmd_ptr), &(f[240]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_regd_time), &(f[241]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.bct_regd_id), &(f[245]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_tlm_counter), &(f[247]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_aobc.rt_tlm_queued), &(f[251]), 1);
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
  ENDIAN_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 13;
  temp_u16 &= 0x7;
  aobc_driver->tlm_data.aobc_hk.ph.ver = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 12;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.ph.type = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 11;
  temp_u16 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.ph.sh_flag = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[0]), 2);
  temp_u16 >>= 0;
  temp_u16 &= 0x7ff;
  aobc_driver->tlm_data.aobc_hk.ph.apid = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 14;
  temp_u16 &= 0x3;
  aobc_driver->tlm_data.aobc_hk.ph.seq_flag = temp_u16;
  ENDIAN_memcpy(&temp_u16, &(f[2]), 2);
  temp_u16 >>= 0;
  temp_u16 &= 0x3fff;
  aobc_driver->tlm_data.aobc_hk.ph.seq_count = temp_u16;
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.ph.packet_len), &(f[4]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.ver), &(f[6]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.ti), &(f[7]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.tlm_id), &(f[11]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.global_time), &(f[12]), 8);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.on_board_subnet_time), &(f[20]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.dest_flags), &(f[24]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.sh.dest_info), &(f[25]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tm_mode_time), &(f[26]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tm_unixtime_at_ti0), &(f[30]), 8);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.mm_opsmode), &(f[38]), 1);
  ENDIAN_memcpy(&temp_u8, &(f[39]), 1);
  temp_u8 >>= 7;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc.mm_sts = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[39]), 1);
  temp_u8 >>= 0;
  temp_u8 &= 0x7f;
  aobc_driver->tlm_data.aobc_hk.obc.mm_opsmode_prev = temp_u8;
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tdsp.current_id), &(f[40]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.counter), &(f[42]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.last_exec.time), &(f[46]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.last_exec.id), &(f[50]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.last_exec.exec_sts), &(f[52]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.last_exec.err_code), &(f[53]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.last_err.time), &(f[57]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.last_err.id), &(f[61]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.last_err.exec_sts), &(f[63]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.last_err.err_code), &(f[64]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.gs_cmd.err_counter), &(f[68]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.counter), &(f[72]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.last_exec.time), &(f[76]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.last_exec.id), &(f[80]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.last_exec.exec_sts), &(f[82]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.last_err.err_code), &(f[83]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.last_err.time), &(f[87]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.last_err.id), &(f[91]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.last_err.exec_sts), &(f[93]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.last_exec.err_code), &(f[94]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.rt_cmd.err_counter), &(f[98]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.counter), &(f[102]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.queued), &(f[106]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.last_exec.time), &(f[107]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.last_exec.id), &(f[111]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.last_exec.exec_sts), &(f[113]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.last_exec.err_code), &(f[114]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.last_err.time), &(f[118]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.last_err.id), &(f[122]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.last_err.exec_sts), &(f[124]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.last_err.err_code), &(f[125]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.err_counter), &(f[129]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.next_time), &(f[130]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.next_id), &(f[134]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_bc.counter), &(f[136]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_bc.queued), &(f[140]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_tlm.counter), &(f[141]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.tlc_tlm.queued), &(f[145]), 1);
  ENDIAN_memcpy(&temp_u8, &(f[146]), 1);
  temp_u8 >>= 7;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.soe_flag = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[146]), 1);
  temp_u8 >>= 6;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc.tlc_gs.lockout_flag = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[146]), 1);
  temp_u8 >>= 5;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc.tlc_bc.soe_flag = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[146]), 1);
  temp_u8 >>= 4;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc.tlc_bc.lockout_flag = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[146]), 1);
  temp_u8 >>= 3;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc.tlc_tlm.soe_flag = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[146]), 1);
  temp_u8 >>= 2;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.obc.tlc_tlm.lockout_flag = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[147]), 1);
  temp_u8 >>= 7;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.wdt.is_enable = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[147]), 1);
  temp_u8 >>= 6;
  temp_u8 &= 0x1;
  aobc_driver->tlm_data.aobc_hk.wdt.is_clear_enable = temp_u8;
  ENDIAN_memcpy(&temp_u8, &(f[147]), 1);
  temp_u8 >>= 0;
  temp_u8 &= 0x3f;
  aobc_driver->tlm_data.aobc_hk.dummy_data0 = temp_u8;
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.bct_blk_ptr), &(f[148]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.bct_cmd_ptr), &(f[150]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.bct_regd_time), &(f[151]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.obc.bct_regd_id), &(f[155]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counter_total), &(f[157]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_high), &(f[161]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_middle), &(f[163]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_low), &(f[165]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_el), &(f[167]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.statistics.record_counters_eh), &(f[169]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.group), &(f[171]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.local), &(f[172]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.err_level), &(f[176]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.time.total_cycle), &(f[177]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.time.step), &(f[181]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_event.note), &(f[182]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.group), &(f[186]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.local), &(f[187]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.time.total_cycle), &(f[191]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.time.step), &(f[195]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.el.latest_high_event.note), &(f[196]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.eh.log_table.respond_counter), &(f[198]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.git_rev_core), &(f[202]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.git_rev_user), &(f[206]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.bct_blk_ptr), &(f[210]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.bct_cmd_ptr), &(f[211]), 1);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.bct_regd_time), &(f[212]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.bct_regd_id), &(f[216]), 2);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data1), &(f[218]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data2), &(f[222]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data3), &(f[226]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data4), &(f[230]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data5), &(f[234]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data6), &(f[238]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data7), &(f[242]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data8), &(f[246]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data9), &(f[250]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data10), &(f[254]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data11), &(f[258]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data12), &(f[262]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data13), &(f[266]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data14), &(f[270]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data15), &(f[274]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data16), &(f[278]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data17), &(f[282]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data18), &(f[286]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data19), &(f[290]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data20), &(f[294]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data21), &(f[298]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data22), &(f[302]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data23), &(f[306]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data24), &(f[310]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data25), &(f[314]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data26), &(f[318]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data27), &(f[322]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data28), &(f[326]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data29), &(f[330]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data30), &(f[334]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data31), &(f[338]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data32), &(f[342]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data33), &(f[346]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data34), &(f[350]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data35), &(f[354]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data36), &(f[358]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data37), &(f[362]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data38), &(f[366]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data39), &(f[370]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data40), &(f[374]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data41), &(f[378]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data42), &(f[382]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data43), &(f[386]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data44), &(f[390]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data45), &(f[394]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data46), &(f[398]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data47), &(f[402]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data48), &(f[406]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data49), &(f[410]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data50), &(f[414]), 4);
  ENDIAN_memcpy(&(aobc_driver->tlm_data.aobc_hk.dummy_data51), &(f[418]), 4);
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
