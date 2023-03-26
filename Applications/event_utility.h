/**
 * @file
 * @brief イベント処理の定期実行のためのApp
 * @note  実質的には event_handler を定期実行するだけ
 */
#ifndef EVENT_UTILITY_H_
#define EVENT_UTILITY_H_

#include "../System/ApplicationManager/app_info.h"
#include "../TlmCmd/common_cmd_packet.h"

AppInfo EVENT_UTIL_create_app(void);

/**
 * @struct EventUtility
 * @brief  EventUtility の AppInfo 構造体
 */
typedef struct
{
  uint8_t is_enabled_eh_execution;    //!< EH の実行が有効か？
} EventUtility;

extern const EventUtility* const event_utility;

CCP_CmdRet Cmd_EVENT_UTIL_ENABLE_EH_EXEC(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_EVENT_UTIL_DISABLE_EH_EXEC(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_EVENT_UTIL_EXEC_EH(const CommonCmdPacket* packet);

#endif
