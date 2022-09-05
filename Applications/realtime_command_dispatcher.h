#ifndef REALTIME_COMMAND_DISPATCHER_H_
#define REALTIME_COMMAND_DISPATCHER_H_

#include "../TlmCmd/command_dispatcher.h"
#include "../System/ApplicationManager/app_info.h"
#include "../TlmCmd/common_cmd_packet.h"

extern const CommandDispatcher* const realtime_command_dispatcher;

AppInfo RTCD_create_app(void);

CCP_CmdRet Cmd_RTCD_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_RTCD_CLEAR_ERR_LOG(const CommonCmdPacket* packet);

#endif
