/**
 * @file
 * @brief watchdog timer
 * @note  こいつもBootLoaderに配置するので，リプロ可能領域のものをincludeしないこと！！
 *        なお，CMDはリプロ対象内！！
 */
#ifndef WATCHDOG_TIMER_H_
#define WATCHDOG_TIMER_H_

#include "../../IfWrapper/wdt.h"

extern const WDT_Config* const wdt_config;

/**
 * @brief  WDTの初期化
 * @param  void
 * @return void
 * @note   IF_wapperではなく，この関数がglobalに公開される
 */
void WDT_init(void);

/**
 * @brief  WDTのクリア
 * @param  void
 * @return void
 * @note   IF_wapperではなく，この関数がglobalに公開される
 */
void WDT_clear_wdt(void);

#endif
