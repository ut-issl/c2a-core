/**
 * @file
 * @brief 地上局から発行された RTC (Real Time Cmd) を実行する
 */
#ifndef GS_COMMAND_DISPATCHER_H_
#define GS_COMMAND_DISPATCHER_H_

#include "../TlmCmd/command_dispatcher.h"
#include "../System/ApplicationManager/app_info.h"

extern const CommandDispatcher* const gs_command_dispatcher;

/**
 * @brief  GSCD App 生成関数
 * @param  void
 * @return GSCD AppInfo
 */
AppInfo GSCD_create_app(void);

CCP_CmdRet Cmd_GSCD_CLEAR_ERR_LOG(const CommonCmdPacket* packet);

#endif
