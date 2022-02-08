#pragma section REPRO
#include "app_manager.h"
#include <src_user/Settings/sils_define.h>

#include <string.h> // for memcpy

#include "../../Applications/nop.h"
#include "../AnomalyLogger/anomaly_logger.h"
#include "../EventManager/event_logger.h"
#include "../TimeManager/time_manager.h"
#include "../WatchdogTimer/watchdog_timer.h"
#include <src_user/TlmCmd/command_definitions.h>
#include "../../Library/print.h"   // for Printf
#include "../../Library/endian_memcpy.h"

static AM_ACK AM_initialize_app_(size_t id);
static AM_ACK AM_execute_app_(size_t id);

static AppManager app_manager_;
const AppManager* const app_manager = &app_manager_;

void AM_initialize(void)
{
  int i;

  for (i = 0; i < AM_MAX_APPS; ++i)
  {
    app_manager_.ais[i] = NOP_create_app();
  }

  // テレメトリページ番号を0に初期化。
  app_manager_.page_no = 0;
}

AM_ACK AM_register_ai(size_t id,
                      const AppInfo* ai)
{
  if (id >= AM_MAX_APPS)
  {
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_APP_MANAGER, AM_INVALID_ID);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_APP_MANAGER, AM_INVALID_ID, EL_ERROR_LEVEL_LOW, (uint32_t)id);
    return AM_INVALID_ID;
  }

  app_manager_.ais[id] = *ai;
  return AM_SUCCESS;
}

void AM_initialize_all_apps(void)
{
  size_t i;

  for (i = 0; i < AM_MAX_APPS; ++i)
  {
    AM_initialize_app_(i);
  }
}

CCP_EXEC_STS Cmd_AM_REGISTER_APP(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t id;
  AppInfo ai;

  // パラメータを読み出し。
  endian_memcpy(&id, param, 4);
  endian_memcpy(&ai.initializer, param + 4, 4);
  endian_memcpy(&ai.entry_point, param + 8, 4);

  ai.name = "SPECIAL";
  ai.prev = 0;
  ai.max = 0;
  ai.min = 0xffffffff;

  switch (AM_register_ai(id, &ai))
  {
  case AM_SUCCESS:
    return CCP_EXEC_SUCCESS;
  case AM_INVALID_ID:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}

CCP_EXEC_STS Cmd_AM_INITIALIZE_APP(const CommonCmdPacket* packet)
{
  size_t id = AM_MAX_APPS;

  // パラメータ読み出し。
  endian_memcpy(&id, CCP_get_param_head(packet), 4);

  switch (AM_initialize_app_(id))
  {
  case AM_SUCCESS:
  case AM_NOT_REGISTERED:
    return CCP_EXEC_SUCCESS;
  case AM_INVALID_ID:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}

static AM_ACK AM_initialize_app_(size_t id)
{
#ifndef SILS_FW
  ObcTime start, finish;
#endif

  if (id >= AM_MAX_APPS)
  {
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_APP_MANAGER, AM_INVALID_ID);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_APP_MANAGER, AM_INVALID_ID, EL_ERROR_LEVEL_LOW, (uint32_t)id);
    return AM_INVALID_ID;
  }
  else if (app_manager_.ais[id].initializer == NULL)
  {
    return AM_NOT_REGISTERED;
  }

#ifdef SILS_FW
  app_manager_.ais[id].initializer();
#else
  start = TMGR_get_master_clock_from_boot();
  app_manager_.ais[id].initializer();
  finish = TMGR_get_master_clock_from_boot();

  // 処理時間情報アップデート
  app_manager_.ais[id].init_duration = OBCT_diff_in_step(&start, &finish);
#endif

  WDT_clear_wdt();
  Printf("App: %s Init Complete !!\n", app_manager_.ais[id].name);
  return AM_SUCCESS;
}

CCP_EXEC_STS Cmd_AM_EXECUTE_APP(const CommonCmdPacket* packet)
{
  size_t id = AM_MAX_APPS;

  // パラメータ読み出し。
  endian_memcpy(&id, CCP_get_param_head(packet), 4);

  switch (AM_execute_app_(id))
  {
  case AM_SUCCESS:
    return CCP_EXEC_SUCCESS;
  case AM_INVALID_ID:
  case AM_NOT_REGISTERED:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}

static AM_ACK AM_execute_app_(size_t id)
{
#ifndef SILS_FW
  ObcTime start, finish;
#endif

  if (id >= AM_MAX_APPS)
  {
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_APP_MANAGER, AM_INVALID_ID);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_APP_MANAGER, AM_INVALID_ID, EL_ERROR_LEVEL_LOW, (uint32_t)id);
    return AM_INVALID_ID;
  }
  else if (app_manager_.ais[id].entry_point == NULL)
  {
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_APP_MANAGER, AM_NOT_REGISTERED);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_APP_MANAGER, AM_NOT_REGISTERED, EL_ERROR_LEVEL_LOW, (uint32_t)id);
    return AM_NOT_REGISTERED;
  }

#ifdef SILS_FW
  app_manager_.ais[id].entry_point();
#else
  start = TMGR_get_master_clock();
  app_manager_.ais[id].entry_point();
  finish = TMGR_get_master_clock();

  // 処理時間情報アップデート
  app_manager_.ais[id].prev = OBCT_diff_in_step(&start, &finish);

  if (app_manager_.ais[id].max < app_manager_.ais[id].prev)
  {
    app_manager_.ais[id].max = app_manager_.ais[id].prev;
  }

  if (app_manager_.ais[id].min > app_manager_.ais[id].prev)
  {
    app_manager_.ais[id].min = app_manager_.ais[id].prev;
  }

#endif

  return AM_SUCCESS;
}

CCP_EXEC_STS Cmd_AM_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page;

  page = CCP_get_param_head(packet)[0];

  if (page >= AM_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  app_manager_.page_no = page;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_AM_CLEAR_APP_INFO(const CommonCmdPacket* packet)
{
  int i;
  (void)packet;

  for (i = 0; i < AM_MAX_APPS; ++i)
  {
    app_manager_.ais[i].prev = 0;
    app_manager_.ais[i].max  = 0;
    app_manager_.ais[i].min  = 0xffffffff;
  }

  return CCP_EXEC_SUCCESS;
}


#pragma section
