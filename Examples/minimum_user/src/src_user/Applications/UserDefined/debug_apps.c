#pragma section REPRO
#include "debug_apps.h"

#include <stddef.h> // for NULL
#include <stdint.h>

#include <src_core/Library/git_revision.h>
#include <src_core/Library/print.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_core/System/TaskManager/task_dispatcher.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/System/EventManager/event_handler.h>
#include <src_core/TlmCmd/packet_handler.h>
#include "../../TlmCmd/telemetry_definitions.h"
#include <src_core/TlmCmd/block_command_table.h>
#include <src_core/Applications/memory_dump.h>
#include <src_core/Applications/gs_command_dispatcher.h>
#include <src_core/Applications/realtime_command_dispatcher.h>
#include <src_core/Applications/timeline_command_dispatcher.h>
#include "../../Applications/DriverInstances/di_gs.h"
// #include <src_core/TlmCmd/telemetry_generator.h>
#include "../../Library/vt100.h"

void APP_DBG_flush_screen_(void);
void APP_DBG_print_time_stamp_(void);
void APP_DBG_print_cmd_status_(void);
void APP_DBG_print_event_logger0_(void);
void APP_DBG_print_event_logger1_(void);
void APP_DBG_print_event_handler_(void);
void APP_DBG_print_git_rev_(void);

AppInfo APP_DBG_flush_screen(void)
{
  return AI_create_app_info("debug_fscr", NULL, APP_DBG_flush_screen_);
}

AppInfo APP_DBG_print_time_stamp(void)
{
  return AI_create_app_info("debug_tstm", NULL, APP_DBG_print_time_stamp_);
}

AppInfo APP_DBG_print_cmd_status(void)
{
  return AI_create_app_info("debug_cmds", NULL, APP_DBG_print_cmd_status_);
}

AppInfo APP_DBG_print_event_logger0(void)
{
  return AI_create_app_info("debug_el0", NULL, APP_DBG_print_event_logger0_);
}

AppInfo APP_DBG_print_event_logger1(void)
{
  return AI_create_app_info("debug_el1", NULL, APP_DBG_print_event_logger1_);
}

AppInfo APP_DBG_print_event_handler(void)
{
  return AI_create_app_info("debug_eh", NULL, APP_DBG_print_event_handler_);
}

AppInfo APP_DBG_print_git_rev(void)
{
  return AI_create_app_info("debug_git_rev", NULL, APP_DBG_print_git_rev_);
}

void APP_DBG_flush_screen_(void)
{
  VT100_erase_down();
  VT100_reset_cursor();
  VT100_erase_line();
  Printf("-- C2A SAMPLE Flight S/W (H-ON, F-ON) --\n");
  VT100_erase_line();
  Printf("BUILD: %s %s\n", __DATE__, __TIME__);
}

void APP_DBG_print_time_stamp_(void)
{
  VT100_erase_line();
  Printf("CYCLE: TOTAL %08d, MODE %08d\n",
         TMGR_get_master_total_cycle(), TMGR_get_master_mode_cycle());
  VT100_erase_line();
  Printf("MODE: STAT %d, PREV %d, CURR %d\n",
         mode_manager->stat, mode_manager->previous_id, mode_manager->current_id);
}

void APP_DBG_print_cmd_status_(void)
{
  VT100_erase_line();
  Printf("CMD: GS %3d, RT %3d, Ack %2d, ID 0x%02x, Sts %1d, EC %d\n",
         (PL_count_executed_nodes(&PH_gs_cmd_list) & 0xff),
         (PL_count_executed_nodes(&PH_rt_cmd_list) & 0xff),
         gs_driver->info[gs_driver->tlm_tx_port_type].rx.cmd_ack,
         gs_command_dispatcher->prev.code,
         gs_command_dispatcher->prev.cmd_ret.exec_sts,
         gs_command_dispatcher->prev.cmd_ret.err_code);
}

void APP_DBG_print_event_logger0_(void)
{
// 一旦めんどくさいので，以下の時のみ対応で書く
#ifdef EL_IS_ENABLE_TLOG
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
#ifdef EL_IS_ENABLE_EVENT_NOTE
  const EL_Event* latest_high = EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0);

  VT100_erase_line();
  Printf("EL Cnt: %3d, H %3d, M %3d, L %3d, EL %3d, EH %3d\n",
         event_logger->statistics.record_counter_total & 0xff,
         event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH] & 0xff,
         event_logger->statistics.record_counters[EL_ERROR_LEVEL_MIDDLE] & 0xff,
         event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW] & 0xff,
         event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL] & 0xff,
         event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH] & 0xff);
  VT100_erase_line();
  Printf("EL H: %3d, %10d, %10d, %08d, %02d\n",
         latest_high->group, latest_high->local, latest_high->note,
         latest_high->time.total_cycle, latest_high->time.step);
#endif
#endif
#endif
#endif
}

void APP_DBG_print_event_logger1_(void)
{
// 一旦めんどくさいので，以下の時のみ対応で書く
#ifdef EL_IS_ENABLE_TLOG
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
#ifdef EL_IS_ENABLE_EVENT_NOTE
  const EL_Event* latest_mid = EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_MIDDLE, 0);
  const EL_Event* latest_low = EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_LOW, 0);

  VT100_erase_line();
  Printf("EL M: %3d, %10d, %10d, %08d, %02d\n",
         latest_mid->group, latest_mid->local, latest_mid->note,
         latest_mid->time.total_cycle, latest_mid->time.step);
  VT100_erase_line();
  Printf("EL L: %3d, %10d, %10d, %08d, %02d\n",
         latest_low->group, latest_low->local, latest_low->note,
         latest_low->time.total_cycle, latest_low->time.step);
#endif
#endif
#endif
#endif
}

void APP_DBG_print_event_handler_(void)
{
  const EH_Log* latest = EH_get_the_nth_log_from_the_latest(0);
  const EH_Log* second = EH_get_the_nth_log_from_the_latest(1);
  VT100_erase_line();
  Printf("EH: Cnt %3d, 1st %3d, %08d, 2nd %3d, %08d\n",
         event_handler->log_table.respond_counter & 0xff,
         latest->rule_id,
         latest->respond_time_in_master_cycle,
         second->rule_id,
         second->respond_time_in_master_cycle);
}

void APP_DBG_print_git_rev_(void)
{
  VT100_erase_line();
  Printf("Git rev: CORE 0x%07x, USER 0x%07x\n",
         GIT_REV_CORE_SHORT, GIT_REV_USER_SHORT);
}

#pragma section
