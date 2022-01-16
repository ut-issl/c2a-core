/**
 * @file
 * @brief App 情報構造体定義と App 作成
 */
#ifndef APP_INFO_H_
#define APP_INFO_H_

#include "../TimeManager/obc_time.h"

typedef struct
{
  const char* name;           //!< アプリ名 (C2A 内部では使用されていない )
  step_t init_duration;       //!< アプリ初期化処理時間
  step_t prev;                //!< アプリ実行処理時間（直近）
  step_t min;                 //!< アプリ実行処理時間（最小値）
  step_t max;                 //!< アプリ実行処理時間（最大値）
  void (*initializer)(void);  //!< アプリ初期化関数
  void (*entry_point)(void);  //!< アプリ実行関数（エントリーポイント）
} AppInfo;

/**
 * @brief  AppInfo を作る
 * @param  name: アプリ名
 * @param  initializer: アプリ初期化関数
 * @param  entry_point: アプリ実行関数
 * @return 作成された AppInfo
 */
AppInfo AI_create_app_info(const char* name,
                           void (*initializer)(void),
                           void (*entry_point)(void));

#endif
