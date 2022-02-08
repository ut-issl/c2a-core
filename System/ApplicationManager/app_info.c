#pragma section REPRO
/**
 * @file
 * @brief App 情報構造体定義と App 作成
 */
#include "app_info.h"
#include <stddef.h>

AppInfo AI_create_app_info(const char* name,
                           void (*initializer)(void),
                           void (*entry_point)(void))
{
  AppInfo ai;

  ai.name = name;
  ai.init_duration = 0;
  ai.prev = 0;
  ai.max = 0;
  ai.min = 0xffffffff;
  ai.initializer = initializer;
  ai.entry_point = entry_point;

  return ai;
}

#pragma section
