#ifndef APP_INFO_H_
#define APP_INFO_H_

#include "../TimeManager/obc_time.h"

typedef struct
{
  const char* name;
  step_t init_duration; // アプリ初期化処理時間保存用
  step_t prev;          // アプリ実行処理時間保存用
  step_t min;           // アプリ実行処理時間保存用
  step_t max;           // アプリ実行処理時間保存用
  void (*initializer)(void);
  void (*entry_point)(void);
} AppInfo;

AppInfo create_app_info(const char* name,
                        void (*initializer)(void),
                        void (*entry_point)(void));

#endif // APP_INFO_H_
