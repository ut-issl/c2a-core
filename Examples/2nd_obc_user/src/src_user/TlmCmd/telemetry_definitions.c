#pragma section REPRO
/**
 * @file
 * @brief  テレメトリ定義
 * @note   このコードは自動生成されています！
 */
#include "../../src_core/TlmCmd/telemetry_frame.h"
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
  if (241 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], (uint32_t)(TMGR_get_master_clock().mode_cycle));
  TF_copy_double(&packet[30], TMGR_get_utl_unixtime_epoch());
  TF_copy_double(&packet[38], (double)(time_manager->unixtime_info_.cycle_correction));
  TF_copy_u8(&packet[46], (uint8_t)(mode_manager->stat));
  TF_copy_u8(&packet[47], (uint8_t)(mode_manager->current_id));
  TF_copy_u8(&packet[48], (uint8_t)(mode_manager->previous_id));
  TF_copy_u8(&packet[49], (uint8_t)(TDSP_info->task_list_id));
  TF_copy_u32(&packet[50], (uint32_t)(TDSP_info->tskd.prev_err.time.total_cycle));
  TF_copy_u8(&packet[54], (uint8_t)(TDSP_info->tskd.prev_err.time.step));
  TF_copy_u16(&packet[55], (uint16_t)(TDSP_info->tskd.prev_err.code));
  TF_copy_i32(&packet[57], (int32_t)(TDSP_info->tskd.prev_err.sts));
  TF_copy_u32(&packet[61], PL_count_executed_nodes(&PH_gs_cmd_list));
  TF_copy_u32(&packet[65], (uint32_t)(gs_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[69], (uint16_t)(gs_command_dispatcher->prev.code));
  TF_copy_i32(&packet[71], (int32_t)(gs_command_dispatcher->prev.sts));
  TF_copy_u32(&packet[75], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[79], (uint16_t)(gs_command_dispatcher->prev_err.code));
  TF_copy_i32(&packet[81], (int32_t)(gs_command_dispatcher->prev_err.sts));
  TF_copy_u32(&packet[85], (uint32_t)(gs_command_dispatcher->error_counter));
  TF_copy_u32(&packet[89], PL_count_executed_nodes(&PH_rt_cmd_list));
  TF_copy_u32(&packet[93], (uint32_t)(realtime_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[97], (uint16_t)(realtime_command_dispatcher->prev.code));
  TF_copy_i32(&packet[99], (int32_t)(realtime_command_dispatcher->prev.sts));
  TF_copy_u32(&packet[103], (uint32_t)(realtime_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[107], (uint16_t)(realtime_command_dispatcher->prev_err.code));
  TF_copy_i32(&packet[109], (int32_t)(realtime_command_dispatcher->prev_err.sts));
  TF_copy_u32(&packet[113], (uint32_t)(realtime_command_dispatcher->error_counter));
  TF_copy_u32(&packet[117], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS]));
  TF_copy_u8(&packet[121], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS])));
  TF_copy_u32(&packet[122], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle));
  TF_copy_u16(&packet[126], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code));
  TF_copy_i32(&packet[128], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.sts));
  TF_copy_u32(&packet[132], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle));
  TF_copy_u16(&packet[136], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code));
  TF_copy_i32(&packet[138], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.sts));
  TF_copy_u32(&packet[142], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].error_counter));
  TF_copy_u8(&packet[146], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error));
  TF_copy_u8(&packet[147], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout));
  TF_copy_u32(&packet[148], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u16(&packet[152], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u32(&packet[154], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]));
  TF_copy_u8(&packet[158], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])));
  TF_copy_u32(&packet[159], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.time.total_cycle));
  TF_copy_u16(&packet[163], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.code));
  TF_copy_i32(&packet[165], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.sts));
  TF_copy_u32(&packet[169], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.time.total_cycle));
  TF_copy_u16(&packet[173], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.code));
  TF_copy_i32(&packet[175], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.sts));
  TF_copy_u32(&packet[179], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].error_counter));
  TF_copy_u8(&packet[183], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error));
  TF_copy_u8(&packet[184], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout));
  TF_copy_u32(&packet[185], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet))));
  TF_copy_u16(&packet[189], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet))));
  TF_copy_u32(&packet[191], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]));
  TF_copy_u8(&packet[195], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])));
  TF_copy_u32(&packet[196], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.time.total_cycle));
  TF_copy_u16(&packet[200], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.code));
  TF_copy_i32(&packet[202], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.sts));
  TF_copy_u32(&packet[206], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.time.total_cycle));
  TF_copy_u16(&packet[210], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.code));
  TF_copy_i32(&packet[212], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.sts));
  TF_copy_u32(&packet[216], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].error_counter));
  TF_copy_u8(&packet[220], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error));
  TF_copy_u8(&packet[221], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout));
  TF_copy_u32(&packet[222], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet))));
  TF_copy_u16(&packet[226], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet))));
  TF_copy_u8(&packet[228], (uint8_t)(block_command_table->pos.block));
  TF_copy_u8(&packet[229], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[230], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[234], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[236], PL_count_executed_nodes(&PH_ms_tlm_list));
  TF_copy_u8(&packet[240], (uint8_t)(PL_count_active_nodes(&PH_ms_tlm_list)));
#endif

  *len = 241;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_HK_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (432 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], TMGR_get_master_clock().mode_cycle);
  TF_copy_double(&packet[30], TMGR_get_unixtime_at_ti0());
  TF_copy_u8(&packet[38], (uint8_t)(mode_manager->current_id));
  TF_copy_u8(&packet[39], (uint8_t)( ((uint8_t)(mode_manager->stat) << 7 & 0x80) | ((uint8_t)(mode_manager->previous_id) & 0x7F) ));
  TF_copy_u16(&packet[40], TDSP_info->task_list_id);
  TF_copy_u32(&packet[42], PL_count_executed_nodes(&PH_gs_cmd_list));
  TF_copy_u32(&packet[46], gs_command_dispatcher->prev.time.total_cycle);
  TF_copy_u16(&packet[50], (uint16_t)(gs_command_dispatcher->prev.code));
  TF_copy_i8(&packet[52], (int8_t)(gs_command_dispatcher->prev.sts));
  TF_copy_u32(&packet[53], gs_command_dispatcher->prev_err.time.total_cycle);
  TF_copy_u16(&packet[57], (uint16_t)(gs_command_dispatcher->prev_err.code));
  TF_copy_i8(&packet[59], (int8_t)(gs_command_dispatcher->prev_err.sts));
  TF_copy_u32(&packet[60], PL_count_executed_nodes(&PH_rt_cmd_list));
  TF_copy_u32(&packet[64], (uint32_t)(realtime_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[68], (uint16_t)(realtime_command_dispatcher->prev.code));
  TF_copy_i32(&packet[70], (int32_t)(realtime_command_dispatcher->prev.sts));
  TF_copy_u32(&packet[74], (uint32_t)(realtime_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[78], (uint16_t)(realtime_command_dispatcher->prev_err.code));
  TF_copy_i32(&packet[80], (int32_t)(realtime_command_dispatcher->prev_err.sts));
  TF_copy_u32(&packet[84], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS]));
  TF_copy_u8(&packet[88], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS])));
  TF_copy_u32(&packet[89], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle);
  TF_copy_u16(&packet[93], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code));
  TF_copy_i8(&packet[95], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.sts));
  TF_copy_u32(&packet[96], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle);
  TF_copy_u16(&packet[100], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code));
  TF_copy_i8(&packet[102], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.sts));
  TF_copy_u32(&packet[103], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u16(&packet[107], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u32(&packet[109], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]));
  TF_copy_u8(&packet[113], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])));
  TF_copy_u32(&packet[114], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]));
  TF_copy_u8(&packet[118], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])));
  TF_copy_u8(&packet[119], (uint8_t)(((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error << 7 & 0x80) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout << 6 & 0x40) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error << 5 & 0x20) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout << 4 & 0x10) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error << 3 & 0x08) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout << 2 & 0x04) | (0 << 1 & 0x02) | (0 & 0x01) ));
  TF_copy_u8(&packet[120], (uint8_t)((wdt_config->is_wdt_enable << 7 & 0x80) | (wdt_config->is_clear_enable << 6 & 0x40) | 0x00));
  TF_copy_u16(&packet[121], block_command_table->pos.block);
  TF_copy_u8(&packet[123], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[124], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[128], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[130], event_logger->statistics.record_counter_total);
  TF_copy_u16(&packet[134], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH]);
  TF_copy_u16(&packet[136], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_MIDDLE]);
  TF_copy_u16(&packet[138], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW]);
  TF_copy_u16(&packet[140], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL]);
  TF_copy_u16(&packet[142], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH]);
  TF_copy_u8(&packet[144], (uint8_t)event_logger->latest_event.group);
  TF_copy_u32(&packet[145], event_logger->latest_event.local);
  TF_copy_u8(&packet[149], (uint8_t)event_logger->latest_event.err_level);
  TF_copy_u32(&packet[150], event_logger->latest_event.time.total_cycle);
  TF_copy_u8(&packet[154], (uint8_t)event_logger->latest_event.time.step);
  TF_copy_u32(&packet[155], event_logger->latest_event.note);
  TF_copy_u8(&packet[159], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->group);
  TF_copy_u32(&packet[160], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->local);
  TF_copy_u32(&packet[164], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.total_cycle);
  TF_copy_u8(&packet[168], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.step);
  TF_copy_u16(&packet[169], (uint16_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->note);
  TF_copy_u32(&packet[171], event_handler->log_table.respond_counter);
  TF_copy_u8(&packet[175], (uint8_t)(gs_command_dispatcher->error_counter));
  TF_copy_u32(&packet[176], GIT_REV_CORE_SHORT);
  TF_copy_u32(&packet[180], GIT_REV_USER_SHORT);
  TF_copy_u8(&packet[184], (uint8_t)(block_command_table->pos.block));
  TF_copy_u8(&packet[185], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[186], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[190], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[192], 0);
  TF_copy_u32(&packet[196], 0);
  TF_copy_u32(&packet[200], 0);
  TF_copy_u32(&packet[204], 0);
  TF_copy_u32(&packet[208], 0);
  TF_copy_u32(&packet[212], 0);
  TF_copy_u32(&packet[216], 0);
  TF_copy_u32(&packet[220], 0);
  TF_copy_u32(&packet[224], 0);
  TF_copy_u32(&packet[228], 0);
  TF_copy_u32(&packet[232], 0);
  TF_copy_u32(&packet[236], 0);
  TF_copy_u32(&packet[240], 0);
  TF_copy_u32(&packet[244], 0);
  TF_copy_u32(&packet[248], 0);
  TF_copy_u32(&packet[252], 0);
  TF_copy_u32(&packet[256], 0);
  TF_copy_u32(&packet[260], 0);
  TF_copy_u32(&packet[264], 0);
  TF_copy_u32(&packet[268], 0);
  TF_copy_u32(&packet[272], 0);
  TF_copy_u32(&packet[276], 0);
  TF_copy_u32(&packet[280], 0);
  TF_copy_u32(&packet[284], 0);
  TF_copy_u32(&packet[288], 0);
  TF_copy_u32(&packet[292], 0);
  TF_copy_u32(&packet[296], 0);
  TF_copy_u32(&packet[300], 0);
  TF_copy_u32(&packet[304], 0);
  TF_copy_u32(&packet[308], 0);
  TF_copy_u32(&packet[312], 0);
  TF_copy_u32(&packet[316], 0);
  TF_copy_u32(&packet[320], 0);
  TF_copy_u32(&packet[324], 0);
  TF_copy_u32(&packet[328], 0);
  TF_copy_u32(&packet[332], 0);
  TF_copy_u32(&packet[336], 0);
  TF_copy_u32(&packet[340], 0);
  TF_copy_u32(&packet[344], 0);
  TF_copy_u32(&packet[348], 0);
  TF_copy_u32(&packet[352], 0);
  TF_copy_u32(&packet[356], 0);
  TF_copy_u32(&packet[360], 0);
  TF_copy_u32(&packet[364], 0);
  TF_copy_u32(&packet[368], 0);
  TF_copy_u32(&packet[372], 0);
  TF_copy_u32(&packet[376], 0);
  TF_copy_u32(&packet[380], 0);
  TF_copy_u32(&packet[384], 0);
  TF_copy_u32(&packet[388], 0);
  TF_copy_u32(&packet[392], 0);
  TF_copy_u32(&packet[396], 0);
  TF_copy_u32(&packet[400], 0);
  TF_copy_u32(&packet[404], 0);
  TF_copy_u32(&packet[408], 0);
  TF_copy_u32(&packet[412], 0);
  TF_copy_u32(&packet[416], 0);
  TF_copy_u32(&packet[420], 0);
  TF_copy_u32(&packet[424], 0);
  TF_copy_u32(&packet[428], 0);
#endif

  *len = 432;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

#pragma section
