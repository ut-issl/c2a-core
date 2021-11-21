#ifndef GS_COMMAND_DISPATCHER_H_
#define GS_COMMAND_DISPATCHER_H_

#include "../CmdTlm/command_dispatcher.h"
#include "../System/ApplicationManager/app_info.h"

extern const CommandDispatcher* const gs_command_dispatcher;

AppInfo GSCD_create_app(void);

CCP_EXEC_STS Cmd_GSCD_CLEAR_ERR_LOG(const CTCP* packet);

#endif
