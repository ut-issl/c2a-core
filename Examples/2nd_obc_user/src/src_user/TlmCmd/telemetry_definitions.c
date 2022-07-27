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
  if (309 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

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
  TF_copy_u32(&packet[73], PL_count_executed_nodes(&PH_gs_cmd_list));
  TF_copy_u32(&packet[77], (uint32_t)(gs_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[81], (uint16_t)(gs_command_dispatcher->prev.code));
  TF_copy_i32(&packet[83], (int32_t)(gs_command_dispatcher->prev.sts));
  TF_copy_u32(&packet[87], (uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[91], (uint16_t)(gs_command_dispatcher->prev_err.code));
  TF_copy_i32(&packet[93], (int32_t)(gs_command_dispatcher->prev_err.sts));
  TF_copy_u32(&packet[97], (uint32_t)(gs_command_dispatcher->error_counter));
  TF_copy_u32(&packet[101], PL_count_executed_nodes(&PH_rt_cmd_list));
  TF_copy_u32(&packet[105], (uint32_t)(realtime_command_dispatcher->prev.time.total_cycle));
  TF_copy_u16(&packet[109], (uint16_t)(realtime_command_dispatcher->prev.code));
  TF_copy_i32(&packet[111], (int32_t)(realtime_command_dispatcher->prev.sts));
  TF_copy_u32(&packet[115], (uint32_t)(realtime_command_dispatcher->prev_err.time.total_cycle));
  TF_copy_u16(&packet[119], (uint16_t)(realtime_command_dispatcher->prev_err.code));
  TF_copy_i32(&packet[121], (int32_t)(realtime_command_dispatcher->prev_err.sts));
  TF_copy_u32(&packet[125], (uint32_t)(realtime_command_dispatcher->error_counter));
  TF_copy_u32(&packet[129], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS]));
  TF_copy_u8(&packet[133], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS])));
  TF_copy_u32(&packet[134], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle));
  TF_copy_u16(&packet[138], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code));
  TF_copy_i32(&packet[140], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.sts));
  TF_copy_u32(&packet[144], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle));
  TF_copy_u16(&packet[148], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code));
  TF_copy_i32(&packet[150], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.sts));
  TF_copy_u32(&packet[154], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].error_counter));
  TF_copy_u8(&packet[158], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error));
  TF_copy_u8(&packet[159], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout));
  TF_copy_u32(&packet[160], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u16(&packet[164], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u32(&packet[166], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]));
  TF_copy_u8(&packet[170], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])));
  TF_copy_u32(&packet[171], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.time.total_cycle));
  TF_copy_u16(&packet[175], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.code));
  TF_copy_i32(&packet[177], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.sts));
  TF_copy_u32(&packet[181], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.time.total_cycle));
  TF_copy_u16(&packet[185], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.code));
  TF_copy_i32(&packet[187], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.sts));
  TF_copy_u32(&packet[191], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].error_counter));
  TF_copy_u8(&packet[195], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error));
  TF_copy_u8(&packet[196], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout));
  TF_copy_u32(&packet[197], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet))));
  TF_copy_u16(&packet[201], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet))));
  TF_copy_u32(&packet[203], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]));
  TF_copy_u8(&packet[207], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])));
  TF_copy_u32(&packet[208], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.time.total_cycle));
  TF_copy_u16(&packet[212], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.code));
  TF_copy_i32(&packet[214], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.sts));
  TF_copy_u32(&packet[218], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.time.total_cycle));
  TF_copy_u16(&packet[222], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.code));
  TF_copy_i32(&packet[224], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.sts));
  TF_copy_u32(&packet[228], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].error_counter));
  TF_copy_u8(&packet[232], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error));
  TF_copy_u8(&packet[233], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout));
  TF_copy_u32(&packet[234], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet))));
  TF_copy_u16(&packet[238], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet))));
  TF_copy_u32(&packet[240], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]));
  TF_copy_u8(&packet[244], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])));
  TF_copy_u32(&packet[245], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.time.total_cycle));
  TF_copy_u16(&packet[249], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.code));
  TF_copy_i32(&packet[251], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.sts));
  TF_copy_u32(&packet[255], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.time.total_cycle));
  TF_copy_u16(&packet[259], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.code));
  TF_copy_i32(&packet[261], (int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.sts));
  TF_copy_u32(&packet[265], (uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].error_counter));
  TF_copy_u8(&packet[269], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].stop_on_error));
  TF_copy_u8(&packet[270], (uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].lockout));
  TF_copy_u32(&packet[271], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))->packet))));
  TF_copy_u16(&packet[275], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))->packet))));
  TF_copy_u8(&packet[277], (uint8_t)(block_command_table->pos.block));
  TF_copy_u8(&packet[278], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[279], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[283], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[294], PL_count_executed_nodes(&PH_ms_tlm_list));
  TF_copy_u8(&packet[298], (uint8_t)(PL_count_active_nodes(&PH_ms_tlm_list)));
  TF_copy_u32(&packet[299], PL_count_executed_nodes(&PH_st_tlm_list));
  TF_copy_u8(&packet[303], (uint8_t)(PL_count_active_nodes(&PH_st_tlm_list)));
  TF_copy_u32(&packet[304], PL_count_executed_nodes(&PH_rp_tlm_list));
  TF_copy_u8(&packet[308], (uint8_t)(PL_count_active_nodes(&PH_rp_tlm_list)));
#endif

  *len = 309;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

static TF_TLM_FUNC_ACK Tlm_AOBC_HK_(uint8_t* packet, uint16_t* len, uint16_t max_len)
{
  if (413 > max_len) return TF_TLM_FUNC_ACK_TOO_SHORT_LEN;

#ifndef BUILD_SETTINGS_FAST_BUILD
  TF_copy_u32(&packet[26], TMGR_get_master_clock().mode_cycle);
  TF_copy_double(&packet[30], TMGR_get_unixtime_at_ti0());
  TF_copy_u8(&packet[38], (uint8_t)(mode_manager->current_id));
  TF_copy_u8(&packet[39], (uint8_t)( ((uint8_t)(mode_manager->stat) << 7 & 0x80) | ((uint8_t)(mode_manager->previous_id) & 0x7F) ));
  TF_copy_u16(&packet[40], TDSP_info->task_list_id);
  TF_copy_u32(&packet[48], PL_count_executed_nodes(&PH_gs_cmd_list));
  TF_copy_u32(&packet[52], gs_command_dispatcher->prev.time.total_cycle);
  TF_copy_u16(&packet[56], (uint16_t)(gs_command_dispatcher->prev.code));
  TF_copy_i8(&packet[58], (int8_t)(gs_command_dispatcher->prev.sts));
  TF_copy_u32(&packet[59], gs_command_dispatcher->prev_err.time.total_cycle);
  TF_copy_u16(&packet[63], (uint16_t)(gs_command_dispatcher->prev_err.code));
  TF_copy_i8(&packet[65], (int8_t)(gs_command_dispatcher->prev_err.sts));
  TF_copy_u32(&packet[66], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS]));
  TF_copy_u8(&packet[70], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS])));
  TF_copy_u32(&packet[71], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle);
  TF_copy_u16(&packet[75], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code));
  TF_copy_i8(&packet[77], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.sts));
  TF_copy_u32(&packet[78], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle);
  TF_copy_u16(&packet[82], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code));
  TF_copy_i8(&packet[84], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.sts));
  TF_copy_u32(&packet[85], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u16(&packet[89], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet))));
  TF_copy_u32(&packet[91], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]));
  TF_copy_u8(&packet[95], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])));
  TF_copy_u32(&packet[96], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]));
  TF_copy_u8(&packet[100], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])));
  TF_copy_u32(&packet[101], PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]));
  TF_copy_u8(&packet[105], (uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])));
  TF_copy_u32(&packet[106], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.time.total_cycle);
  TF_copy_u16(&packet[110], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.code));
  TF_copy_i8(&packet[112], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.sts));
  TF_copy_u32(&packet[113], timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.time.total_cycle);
  TF_copy_u16(&packet[117], (uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.code));
  TF_copy_i8(&packet[119], (int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.sts));
  TF_copy_u32(&packet[120], (PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))->packet))));
  TF_copy_u16(&packet[124], (uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))->packet))));
  TF_copy_u8(&packet[126], (uint8_t)(((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error << 7 & 0x80) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout << 6 & 0x40) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error << 5 & 0x20) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout << 4 & 0x10) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error << 3 & 0x08) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout << 2 & 0x04) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].stop_on_error << 1 & 0x02) | ((uint8_t)timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].lockout & 0x01) ));
  TF_copy_u8(&packet[127], (uint8_t)((wdt_config->is_wdt_enable << 7 & 0x80) | (wdt_config->is_clear_enable << 6 & 0x40) | 0x00));
  TF_copy_u16(&packet[128], block_command_table->pos.block);
  TF_copy_u8(&packet[130], (uint8_t)(block_command_table->pos.cmd));
  TF_copy_u32(&packet[131], ((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u16(&packet[135], (uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1))));
  TF_copy_u32(&packet[137], event_logger->statistics.record_counter_total);
  TF_copy_u16(&packet[141], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH]);
  TF_copy_u16(&packet[143], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_MIDDLE]);
  TF_copy_u16(&packet[145], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW]);
  TF_copy_u16(&packet[147], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL]);
  TF_copy_u16(&packet[149], (uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH]);
  TF_copy_u8(&packet[151], (uint8_t)event_logger->latest_event.group);
  TF_copy_u32(&packet[152], event_logger->latest_event.local);
  TF_copy_u8(&packet[156], (uint8_t)event_logger->latest_event.err_level);
  TF_copy_u32(&packet[157], event_logger->latest_event.time.total_cycle);
  TF_copy_u8(&packet[161], (uint8_t)event_logger->latest_event.time.step);
  TF_copy_u32(&packet[162], event_logger->latest_event.note);
  TF_copy_u8(&packet[166], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->group);
  TF_copy_u32(&packet[167], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->local);
  TF_copy_u32(&packet[171], EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.total_cycle);
  TF_copy_u8(&packet[175], (uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.step);
  TF_copy_u16(&packet[176], (uint16_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->note);
  TF_copy_u32(&packet[178], event_handler->log_table.respond_counter);
  TF_copy_u8(&packet[204], (uint8_t)(gs_command_dispatcher->error_counter));
  TF_copy_u32(&packet[225], GIT_REV_CORE_SHORT);
  TF_copy_u32(&packet[229], GIT_REV_USER_SHORT);
  TF_copy_u32(&packet[233], 0);
  TF_copy_u32(&packet[237], 0);
  TF_copy_u32(&packet[241], 0);
  TF_copy_u32(&packet[245], 0);
  TF_copy_u32(&packet[249], 0);
  TF_copy_u32(&packet[253], 0);
  TF_copy_u32(&packet[257], 0);
  TF_copy_u32(&packet[261], 0);
  TF_copy_u32(&packet[265], 0);
  TF_copy_u32(&packet[269], 0);
  TF_copy_u32(&packet[273], 0);
  TF_copy_u32(&packet[277], 0);
  TF_copy_u32(&packet[281], 0);
  TF_copy_u32(&packet[285], 0);
  TF_copy_u32(&packet[289], 0);
  TF_copy_u32(&packet[293], 0);
  TF_copy_u32(&packet[297], 0);
  TF_copy_u32(&packet[301], 0);
  TF_copy_u32(&packet[305], 0);
  TF_copy_u32(&packet[309], 0);
  TF_copy_u32(&packet[313], 0);
  TF_copy_u32(&packet[317], 0);
  TF_copy_u32(&packet[321], 0);
  TF_copy_u32(&packet[325], 0);
  TF_copy_u32(&packet[329], 0);
  TF_copy_u32(&packet[333], 0);
  TF_copy_u32(&packet[337], 0);
  TF_copy_u32(&packet[341], 0);
  TF_copy_u32(&packet[345], 0);
  TF_copy_u32(&packet[349], 0);
  TF_copy_u32(&packet[353], 0);
  TF_copy_u32(&packet[357], 0);
  TF_copy_u32(&packet[361], 0);
  TF_copy_u32(&packet[365], 0);
  TF_copy_u32(&packet[369], 0);
  TF_copy_u32(&packet[373], 0);
  TF_copy_u32(&packet[377], 0);
  TF_copy_u32(&packet[381], 0);
  TF_copy_u32(&packet[385], 0);
  TF_copy_u32(&packet[389], 0);
  TF_copy_u32(&packet[393], 0);
  TF_copy_u32(&packet[397], 0);
  TF_copy_u32(&packet[401], 0);
  TF_copy_u32(&packet[405], 0);
  TF_copy_u32(&packet[409], 0);
#endif

  *len = 413;
  return TF_TLM_FUNC_ACK_SUCCESS;
}

#pragma section
