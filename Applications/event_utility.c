#pragma section REPRO
/**
 * @file  event_utility.c
 * @brief イベント処理の定期実行のためのApp
 * @note  実質的には event_manager を定期実行するだけ
 */
#include "event_utility.h"

#include <stddef.h> // for NULL

static void EVENT_UTIL_update_(void);

AppInfo EVENT_UTIL_create_app(void)
{
  return create_app_info("event", NULL, EVENT_UTIL_update_);
}

static void EVENT_UTIL_update_()
{
  // TODO: 実装する
  // EM_update_by_event_utility_app();
}

#pragma section
