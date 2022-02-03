#pragma section REPRO
#include "debug_apps.h"

#include <stddef.h> // for NULL

#include <src_core/Library/git_revision.h>
#include <src_core/Library/print.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_core/System/TaskManager/task_dispatcher.h>
#include <src_core/System/AnomalyLogger/anomaly_logger.h>
// #include "../../ReProgramming/ReProg.h"
#include <src_core/TlmCmd/packet_handler.h>
#include "../../TlmCmd/telemetry_definitions.h"
#include <src_core/TlmCmd/block_command_table.h>
// #include "stt_sel_detector.h"
#include <src_core/Applications/memory_dump.h>
#include <src_core/Applications/gs_command_dispatcher.h>
#include <src_core/Applications/realtime_command_dispatcher.h>
#include <src_core/Applications/timeline_command_dispatcher.h>
#include "../../Applications/DriverInstances/di_gs.h"
// #include <src_core/TlmCmd/telemetry_generator.h>
#include "../../Library/VT100.h"
#include "../../Library/stdint.h"

void APP_DBG_flush_screen_(void);
void APP_DBG_print_time_stamp_(void);
void APP_DBG_print_cmd_status_(void);
void APP_DBG_print_git_rev_(void);

AppInfo APP_DBG_flush_screen(void)
{
  return AI_create_app_info("fscr", NULL, APP_DBG_flush_screen_);
}

AppInfo APP_DBG_print_time_stamp(void)
{
  return AI_create_app_info("tstm", NULL, APP_DBG_print_time_stamp_);
}

AppInfo APP_DBG_print_cmd_status(void)
{
  return AI_create_app_info("cmds", NULL, APP_DBG_print_cmd_status_);
}

AppInfo APP_DBG_print_git_rev(void)
{
  return AI_create_app_info("git_rev", NULL, APP_DBG_print_git_rev_);
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
  Printf("CYCLE: TOTAL %04d, MODE %04d\n",
         TMGR_get_master_total_cycle(), TMGR_get_master_mode_cycle());
  VT100_erase_line();
  Printf("MODE: STAT %d, PREV %d, CURR %d\n",
         mode_manager->stat, mode_manager->previous_id, mode_manager->current_id);
}

void APP_DBG_print_cmd_status_(void)
{
  VT100_erase_line();
  Printf("CMD: GS %3d, RT %3d, Ack %3d, Code 0x%02x, Sts %3d\n",
         (PL_count_executed_nodes(&PH_gs_cmd_list) & 0xff),
         (PL_count_executed_nodes(&PH_rt_cmd_list) & 0xff),
         gs_driver->info[gs_driver->tlm_tx_port_type].cmd_ack, gs_command_dispatcher->prev.code, gs_command_dispatcher->prev.sts);
}

void APP_DBG_print_git_rev_(void)
{
  VT100_erase_line();
  Printf("Git rev: CORE 0x%07x, USER 0x%07x\n",
         GIT_REV_CORE_SHORT, GIT_REV_USER_SHORT);
}

#pragma section
