#pragma section REPRO
/**
 * @file
 * @brief  テレメトリ定義
 * @note   このコードは自動生成されています！
 */
#include <src_core/TlmCmd/telemetry_frame.h>
#include "telemetry_definitions.h"
#include "telemetry_source.h"

static TF_TLM_FUNC_ACK Tlm_AOBC_AOBC_(uint8_t* packet, uint16_t* len, uint16_t max_len);
static TF_TLM_FUNC_ACK Tlm_AOBC_HK_(uint8_t* packet, uint16_t* len, uint16_t max_len);

void TF_load_tlm_table(TF_TlmInfo tlm_table[TF_MAX_TLMS])
{
  tlm_table[Tlm_CODE_AOBC_AOBC].tlm_func = Tlm_AOBC_AOBC_;
  tlm_table[Tlm_CODE_AOBC_HK].tlm_func = Tlm_AOBC_HK_;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_AOBC_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (252 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(TMGR_get_master_clock().mode_cycle));
  TF_copy_double(&packet[30], TMGR_get_utl_unixtime_epoch());
  TF_copy_double(&packet[38], (double)(time_manager->unixtime_info_.cycle_correction));
  TF_copy_u8(&packet[46], (uint8_t)(mode_manager->stat));
  TF_copy_u8(&packet[47], (uint8_t)(mode_manager->current_id));
  TF_copy_u8(&packet[48], (uint8_t)(mode_manager->previous_id));
  TF_copy_u8(&packet[49], (uint8_t)(task_dispatcher->task_list_id));
  TF_copy_u32(&packet[50], (uint32_t)(task_dispatcher->tskd.prev_err.time.total_cycle));
  TF_copy_u8(&packet[54], (uint8_t)(task_dispatcher->tskd.prev_err.time.step));
  TF_copy_u16(&packet[55], (uint16_t)(task_dispatcher->tskd.prev_err.code));
  TF_copy_u8(&packet[57], (uint8_t)(task_dispatcher->tskd.prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[58], task_dispatcher->tskd.prev_err.cmd_ret.err_code);
  TF_copy_u32(&packet[62], PL_count_executed_nodes(&PH_gs_cmd_list));
  TF_copy_u32(&packet[66], (uint32_t)(gs_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[70], (uint16_t)(gs_command_dispatcher->prev.code));
  TF_copy_u8(&packet[72], (uint8_t)(gs_command_dispatcher->prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[73], gs_command_dispatcher->prev.cmd_ret.err_code);
  TF_copy_u32(&packet[77], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[81], (uint16_t)(gs_command_dispatcher->prev_err.code));
  TF_copy_u8(&packet[83], (uint8_t)(gs_command_dispatcher->prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[84], gs_command_dispatcher->prev_err.cmd_ret.err_code);
  TF_copy_u32(&packet[88], (uint32_t)(gs_command_dispatcher->error_counter));
  TF_copy_u32(&packet[92], PL_count_executed_nodes(&PH_rt_cmd_list));
  TF_copy_u32(&packet[96], (uint32_t)(realtime_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[100], (uint16_t)(realtime_command_dispatcher->prev.code));
  TF_copy_u8(&packet[102], (uint8_t)(realtime_command_dispatcher->prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[103], realtime_command_dispatcher->prev.cmd_ret.err_code);
  TF_copy_u32(&packet[107], (uint32_t)(realtime_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[111], (uint16_t)(realtime_command_dispatcher->prev_err.code));
  TF_copy_u8(&packet[113], (uint8_t)(realtime_command_dispatcher->prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[114], realtime_command_dispatcher->prev_err.cmd_ret.err_code);
  TF_copy_u32(&packet[118], (uint32_t)(realtime_command_dispatcher->error_counter));
  TF_copy_u32(&packet[122], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS]));
  TF_copy_u8(&packet[126], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS])));
  TF_copy_u32(&packet[127], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle));
  TF_copy_u16(&packet[131], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code));
  TF_copy_u8(&packet[133], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[134], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.cmd_ret.err_code);
  TF_copy_u32(&packet[138], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle));
  TF_copy_u16(&packet[142], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code));
  TF_copy_u8(&packet[144], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[145], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.cmd_ret.err_code);
  TF_copy_u32(&packet[149], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].error_counter));
  TF_copy_u8(&packet[153], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error));
  TF_copy_u8(&packet[154], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout));
  TF_copy_u32(&packet[155], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u16(&packet[159], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u32(&packet[161], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]));
  TF_copy_u8(&packet[165], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])));
  TF_copy_u32(&packet[166], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.time.total_cycle));
  TF_copy_u16(&packet[170], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.code));
  TF_copy_u8(&packet[172], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[173], timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.cmd_ret.err_code);
  TF_copy_u32(&packet[177], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.time.total_cycle));
  TF_copy_u16(&packet[181], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.code));
  TF_copy_u8(&packet[183], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[184], timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.cmd_ret.err_code);
  TF_copy_u32(&packet[188], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].error_counter));
  TF_copy_u8(&packet[192], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error));
  TF_copy_u8(&packet[193], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout));
  TF_copy_u32(&packet[194], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet))));
  TF_copy_u16(&packet[198], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet))));
  TF_copy_u32(&packet[200], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]));
  TF_copy_u8(&packet[204], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])));
  TF_copy_u32(&packet[205], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.time.total_cycle));
  TF_copy_u16(&packet[209], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.code));
  TF_copy_u8(&packet[211], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[212], timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.cmd_ret.err_code);
  TF_copy_u32(&packet[216], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.time.total_cycle));
  TF_copy_u16(&packet[220], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.code));
  TF_copy_u8(&packet[222], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[223], timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.cmd_ret.err_code);
  TF_copy_u32(&packet[227], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].error_counter));
  TF_copy_u8(&packet[231], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error));
  TF_copy_u8(&packet[232], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout));
  TF_copy_u32(&packet[233], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet))));
  TF_copy_u16(&packet[237], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet))));
  TF_copy_u8(&packet[239], (uint8_t)(block_command_table->pos.block));
  TF_copy_u8(&packet[240], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[241], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[245], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[247], PL_count_executed_nodes(&PH_rt_tlm_list));
  TF_copy_u8(&packet[251], (uint8_t)(PL_count_active_nodes(&PH_rt_tlm_list)));
#endif

  *len = 252;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_HK_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (422 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], TMGR_get_master_clock().mode_cycle);
  TF_copy_double(&packet[30], TMGR_get_unixtime_at_ti0());
  TF_copy_u8(&packet[38], (uint8_t)(mode_manager->current_id));
  TF_copy_u8(&packet[39], (uint8_t)( ((uint8_t)(mode_manager->stat) << 7 & 0x80) | ((uint8_t)(mode_manager->previous_id) & 0x7F) ));
  TF_copy_u16(&packet[40], task_dispatcher->task_list_id);
  TF_copy_u32(&packet[42], PL_count_executed_nodes(&PH_gs_cmd_list));
  TF_copy_u32(&packet[46], (uint32_t)(gs_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[50], (uint16_t)(gs_command_dispatcher->prev.code));
  TF_copy_u8(&packet[52], (uint8_t)(gs_command_dispatcher->prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[53], gs_command_dispatcher->prev.cmd_ret.err_code);
  TF_copy_u32(&packet[57], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[61], (uint16_t)(gs_command_dispatcher->prev_err.code));
  TF_copy_u8(&packet[63], (uint8_t)(gs_command_dispatcher->prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[64], gs_command_dispatcher->prev_err.cmd_ret.err_code);
  TF_copy_u32(&packet[68], (uint32_t)(gs_command_dispatcher->error_counter));
  TF_copy_u32(&packet[72], PL_count_executed_nodes(&PH_rt_cmd_list));
  TF_copy_u32(&packet[76], (uint32_t)(realtime_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[80], (uint16_t)(realtime_command_dispatcher->prev.code));
  TF_copy_u8(&packet[82], (uint8_t)(realtime_command_dispatcher->prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[83], realtime_command_dispatcher->prev.cmd_ret.err_code);
  TF_copy_u32(&packet[87], (uint32_t)(realtime_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[91], (uint16_t)(realtime_command_dispatcher->prev_err.code));
  TF_copy_u8(&packet[93], (uint8_t)(realtime_command_dispatcher->prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[94], realtime_command_dispatcher->prev_err.cmd_ret.err_code);
  TF_copy_u32(&packet[98], (uint32_t)(realtime_command_dispatcher->error_counter));
  TF_copy_u32(&packet[102], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS]));
  TF_copy_u8(&packet[106], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS])));
  TF_copy_u32(&packet[107], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle));
  TF_copy_u16(&packet[111], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code));
  TF_copy_u8(&packet[113], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.cmd_ret.exec_sts));
  TF_copy_u32(&packet[114], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.cmd_ret.err_code);
  TF_copy_u32(&packet[118], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle));
  TF_copy_u16(&packet[122], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code));
  TF_copy_u8(&packet[124], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.cmd_ret.exec_sts));
  TF_copy_u32(&packet[125], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.cmd_ret.err_code);
  TF_copy_u8(&packet[129], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].error_counter));
  TF_copy_u32(&packet[130], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u16(&packet[134], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u32(&packet[136], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]));
  TF_copy_u8(&packet[140], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])));
  TF_copy_u32(&packet[141], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]));
  TF_copy_u8(&packet[145], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])));
  TF_copy_u8(&packet[146], (uint8_t)(((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error << 7 & 0x80) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout << 6 & 0x40) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error << 5 & 0x20) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout << 4 & 0x10) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error << 3 & 0x08) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout << 2 & 0x04) | (0 << 1 & 0x02) | (0 & 0x01) ));
  TF_copy_u8(&packet[147], (uint8_t)((wdt_config->is_wdt_enable << 7 & 0x80) | (wdt_config->is_clear_enable << 6 & 0x40) | 0x00));
  TF_copy_u16(&packet[148], block_command_table->pos.block);
  TF_copy_u8(&packet[150], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[151], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[155], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[157], event_logger->statistics.record_counter_total);
  TF_copy_u16(&packet[161], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH]);
  TF_copy_u16(&packet[163], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_MIDDLE]);
  TF_copy_u16(&packet[165], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW]);
  TF_copy_u16(&packet[167], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL]);
  TF_copy_u16(&packet[169], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH]);
  TF_copy_u8(&packet[171], (uint8_t)event_logger->latest_event.group);
  TF_copy_u32(&packet[172], event_logger->latest_event.local);
  TF_copy_u8(&packet[176], (uint8_t)event_logger->latest_event.err_level);
  TF_copy_u32(&packet[177], event_logger->latest_event.time.total_cycle);
  TF_copy_u8(&packet[181], (uint8_t)event_logger->latest_event.time.step);
  TF_copy_u32(&packet[182], event_logger->latest_event.note);
  TF_copy_u8(&packet[186], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->group);
  TF_copy_u32(&packet[187], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->local);
  TF_copy_u32(&packet[191], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.total_cycle);
  TF_copy_u8(&packet[195], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.step);
  TF_copy_u16(&packet[196], (uint16_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->note);
  TF_copy_u32(&packet[198], event_handler->log_table.respond_counter);
  TF_copy_u32(&packet[202], GIT_REV_CORE_SHORT);
  TF_copy_u32(&packet[206], GIT_REV_USER_SHORT);
  TF_copy_u8(&packet[210], (uint8_t)(block_command_table->pos.block));
  TF_copy_u8(&packet[211], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[212], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[216], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[218], 0);
  TF_copy_u32(&packet[222], 0);
  TF_copy_u32(&packet[226], 0);
  TF_copy_u32(&packet[230], 0);
  TF_copy_u32(&packet[234], 0);
  TF_copy_u32(&packet[238], 0);
  TF_copy_u32(&packet[242], 0);
  TF_copy_u32(&packet[246], 0);
  TF_copy_u32(&packet[250], 0);
  TF_copy_u32(&packet[254], 0);
  TF_copy_u32(&packet[258], 0);
  TF_copy_u32(&packet[262], 0);
  TF_copy_u32(&packet[266], 0);
  TF_copy_u32(&packet[270], 0);
  TF_copy_u32(&packet[274], 0);
  TF_copy_u32(&packet[278], 0);
  TF_copy_u32(&packet[282], 0);
  TF_copy_u32(&packet[286], 0);
  TF_copy_u32(&packet[290], 0);
  TF_copy_u32(&packet[294], 0);
  TF_copy_u32(&packet[298], 0);
  TF_copy_u32(&packet[302], 0);
  TF_copy_u32(&packet[306], 0);
  TF_copy_u32(&packet[310], 0);
  TF_copy_u32(&packet[314], 0);
  TF_copy_u32(&packet[318], 0);
  TF_copy_u32(&packet[322], 0);
  TF_copy_u32(&packet[326], 0);
  TF_copy_u32(&packet[330], 0);
  TF_copy_u32(&packet[334], 0);
  TF_copy_u32(&packet[338], 0);
  TF_copy_u32(&packet[342], 0);
  TF_copy_u32(&packet[346], 0);
  TF_copy_u32(&packet[350], 0);
  TF_copy_u32(&packet[354], 0);
  TF_copy_u32(&packet[358], 0);
  TF_copy_u32(&packet[362], 0);
  TF_copy_u32(&packet[366], 0);
  TF_copy_u32(&packet[370], 0);
  TF_copy_u32(&packet[374], 0);
  TF_copy_u32(&packet[378], 0);
  TF_copy_u32(&packet[382], 0);
  TF_copy_u32(&packet[386], 0);
  TF_copy_u32(&packet[390], 0);
  TF_copy_u32(&packet[394], 0);
  TF_copy_u32(&packet[398], 0);
  TF_copy_u32(&packet[402], 0);
  TF_copy_u32(&packet[406], 0);
  TF_copy_u32(&packet[410], 0);
  TF_copy_u32(&packet[414], 0);
  TF_copy_u32(&packet[418], 0);
#endif

  *len = 422;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

#pragma section
