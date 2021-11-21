#ifndef NOP_H_
#define NOP_H_

#include "../System/ApplicationManager/app_info.h"
#include "../CmdTlm/common_tlm_cmd_packet.h"

AppInfo NOP_create_app(void);

CCP_EXEC_STS Cmd_NOP(const CTCP* packet);

#endif
