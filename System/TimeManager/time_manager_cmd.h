#ifndef TIME_MANAGER_CMD_H_
#define TIME_MANAGER_CMD_H_

#include "../../TlmCmd/common_cmd_packet.h"

CCP_CmdRet Cmd_TMGR_SET_TIME(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_UPDATE_UNIXTIME(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_SET_UTL_UNIXTIME_EPOCH(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_SET_CYCLE_CORRECTION(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_RESET_CYCLE_CORRECTION(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_CLEAR_UNIXTIME_INFO(const CommonCmdPacket* packet);


#endif // TIME_MANAGER_CMD_H_
