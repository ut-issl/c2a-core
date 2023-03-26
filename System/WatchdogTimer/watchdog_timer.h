/**
 * @file
 * @brief watchdog timer
 * @note  こいつもBootLoaderに配置するので，リプロ可能領域のものをincludeしないこと！！
 *        なお，CMDはリプロ対象内！！
 */
#ifndef WATCHDOG_TIMER_H_
#define WATCHDOG_TIMER_H_

#include "../../IfWrapper/wdt.h"
#include "../../TlmCmd/common_cmd_packet.h"

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

// ↑全再プロ対象外
// ↓全再プロ対象内

CCP_CmdRet Cmd_WDT_INIT(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_WDT_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_WDT_DISABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_WDT_STOP_CLEAR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_WDT_START_CLEAR(const CommonCmdPacket* packet);

#endif
