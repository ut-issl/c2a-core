/**
 * @file
 * @brief watchdog timer の IF wapper
 */
#ifndef WDT_H_
#define WDT_H_

#include <stdint.h>

typedef struct
{
  int     timer_setting;
  uint8_t is_wdt_enable;
  uint8_t is_clear_enable;
} WDT_Config;

/**
 * @brief  WDTの初期化
 * @param  *wdt_config: WDT_Config 構造体へのポインタ
 * @retval 0:     正常終了
 * @retval 0以外: 異常終了
 */
int WDT_initialize(void* wdt_config);

/**
 * @brief  WDTのカウンタクリア
 * @param  *wdt_config: WDT_Config 構造体へのポインタ
 * @retval 0:     正常終了
 * @retval 0以外: 異常終了（パラメタエラーやWDT無効中など）
 */
int WDT_clear(void* wdt_config);

/**
 * @brief  WDTの有効化
 * @param  *wdt_config: WDT_Config 構造体へのポインタ
 * @retval 0:     正常終了
 * @retval 0以外: 異常終了（パラメタエラーやすでにWDTが有効な場合など）
 */
int WDT_enable(void* wdt_config);

/**
 * @brief  WDTの有効化
 * @param  *wdt_config: WDT_Config 構造体へのポインタ
 * @retval 0:     正常終了
 * @retval 0以外: 異常終了（パラメタエラーやすでにWDTが無効な場合など）
 */
int WDT_disable(void* wdt_config);

/**
 * @brief  WDTの時間設定
 * @param  *wdt_config: WDT_Config 構造体へのポインタ
 * @retval 0:     正常終了
 * @retval 0以外: 異常終了（パラメタエラーやすでにWDT動作中など）
 */
int WDT_set_timer(void* wdt_config, int time);

#endif
