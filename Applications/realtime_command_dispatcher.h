#ifndef REALTIME_COMMAND_DISPATCHER_H_
#define REALTIME_COMMAND_DISPATCHER_H_

#include "../TlmCmd/command_dispatcher.h"
#include "../System/ApplicationManager/app_info.h"
#include "../TlmCmd/common_tlm_cmd_packet.h"

extern const CommandDispatcher* const realtime_command_dispatcher;

AppInfo RTCD_create_app(void);

CCP_EXEC_STS Cmd_RTCD_CLEAR_ALL_REALTIME(const CTCP* packet);

CCP_EXEC_STS Cmd_RTCD_CLEAR_ERR_LOG(const CTCP* packet);

#endif
