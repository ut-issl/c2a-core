#ifndef WATCHDOG_TIMER_CMD_H_
#define WATCHDOG_TIMER_CMD_H_

#include "../../TlmCmd/common_cmd_packet.h"

// ↓全再プロ対象内

CCP_CmdRet Cmd_WDT_INIT(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_WDT_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_WDT_DISABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_WDT_STOP_CLEAR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_WDT_START_CLEAR(const CommonCmdPacket* packet);

#endif // WATCHDOG_TIMER_CMD_H_
