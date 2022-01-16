#pragma section REPRO
/**
 * @file
 * @brief イベント処理の定期実行のためのApp
 * @note  実質的には event_handler を定期実行するだけ
 */
#include "event_utility.h"
#include "../System/EventManager/event_handler.h"

#include <stddef.h> // for NULL

static void EVENT_UTIL_init_(void);
static void EVENT_UTIL_update_(void);

static EventUtility event_utility_;
const EventUtility* const event_utility = &event_utility_;

AppInfo EVENT_UTIL_create_app(void)
{
  return AI_create_app_info("event_util", EVENT_UTIL_init_, EVENT_UTIL_update_);
}

static void EVENT_UTIL_init_(void)
{
  event_utility_.is_enabled_eh_execution = 1;
}

static void EVENT_UTIL_update_()
{
  if (event_utility_.is_enabled_eh_execution)
  {
    EH_execute();
  }
}

CCP_EXEC_STS Cmd_EVENT_UTIL_ENABLE_EH_EXEC(const CTCP* packet)
{
  (void)packet;
  event_utility_.is_enabled_eh_execution = 1;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_EVENT_UTIL_DISABLE_EH_EXEC(const CTCP* packet)
{
  (void)packet;
  event_utility_.is_enabled_eh_execution = 0;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_EVENT_UTIL_EXEC_EH(const CTCP* packet)
{
  (void)packet;
  EH_execute();
  return CCP_EXEC_SUCCESS;
}

#pragma section
