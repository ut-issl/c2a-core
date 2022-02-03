#pragma section REPRO
#include "c2a_core_main.h"

#include "./Library/git_revision.h"
#include "./Library/print.h"
#include "./System/TaskManager/task_dispatcher.h"
#include "./System/ApplicationManager/app_manager.h"
#include "./System/EventManager/event_manager.h"
#include "./System/AnomalyLogger/anomaly_logger.h"
#include "./System/TimeManager/time_manager.h"
#include "./System/ModeManager/mode_manager.h"
#include "./System/WatchdogTimer/watchdog_timer.h"
#include "./TlmCmd/packet_handler.h"
#include "./TlmCmd/block_command_table.h"
#include "./TlmCmd/command_analyze.h"
#include "./TlmCmd/telemetry_frame.h"

#include <src_user/Applications/app_registry.h>

// git revisionをコードに埋め込む
const char GIT_REV_CORE[41]         = GIT_REVISION_C2A_CORE;
const uint32_t GIT_REV_CORE_SHORT   = GIT_REVISION_C2A_CORE_SHORT;
const char GIT_REV_USER[41]         = GIT_REVISION_C2A_USER;
const uint32_t GIT_REV_USER_SHORT   = GIT_REVISION_C2A_USER_SHORT;

void C2A_core_init(void)
{
  CA_initialize();            // Cmd Analyze
  Printf("C2A_init: CA_initialize done.\n");
  TF_initialize();            // TLM frame
  Printf("C2A_init: TF_initialize done.\n");
  PH_init();                  // Packet Handler
  Printf("C2A_init: PH_init done.\n");
  EM_initialize();            // Event Manager．App Managerより先に初期化するべき
  Printf("C2A_init: EM_initialize done.\n");
  AL_initialize();            // Anomaly Logger．App Managerより先に初期化するべき
  Printf("C2A_init: AL_initialize done.\n");
  AM_initialize();            // App Manager
  Printf("C2A_init: AM_initialize done.\n");
  AR_load_initial_settings(); // App Registry
  Printf("C2A_init: AR_load_initial_settings done.\n");
  AM_initialize_all_apps();   // App Managerに登録されてる全アプリの初期化
  Printf("C2A_init: AM_initialize_all_apps done.\n");
  BCT_initialize();           // Block Cmd Table
                              // BC_load_defaults() もここで呼ばれる
  Printf("C2A_init: BCT_initialize done.\n");
  MM_initialize();            // Mode Manager
                              // ここでSTART UP to INITIALのモード遷移も行われる
  Printf("C2A_init: MM_initialize done.\n");
  TDSP_initialize();          // Task Dispatcher
                              // MM初期化よりもあとに行われる必要がある
  Printf("C2A_init: TDSP_initialize done.\n");
  // DebugOutInit();             // Debugのinit          // LVTTL UART ch1での出力．邪魔なのでPrintfの中身とともに無効化 (2019-04-09)
  // Printf("C2A_init: DebugOutInit done.\n");

  TMGR_down_initializing_flag();
}

void C2A_core_main(void)
{
  // ここでいうtask dispatcherは，TL0とかではなく，
  // task listのBlockCommandをdispatchしている．
  // TL0などのPLをdispatchしているのは，tlc_dispatcher @ App/timeline_command_dispatcher である．
  // なお，PL (packetList) のPL_infoとは，順次dispatchしていくパケットを時系列にならべたlinked listである．
  TDSP_execute_pl_as_task_list();
  WDT_clear_wdt();
}

#pragma section
