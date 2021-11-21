/**
 * @file  event_utility.h
 * @brief イベント処理の定期実行のためのApp
 * @note  実質的には event_manager を定期実行するだけ
 */
#ifndef EVENT_UTILITY_H_
#define EVENT_UTILITY_H_

#include "../System/ApplicationManager/app_info.h"
#include "../CmdTlm/common_tlm_cmd_packet.h"

AppInfo EVENT_UTIL_create_app(void);

#endif
