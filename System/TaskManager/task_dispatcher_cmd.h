#ifndef TASK_DISPATCHER_CMD_H_
#define TASK_DISPATCHER_CMD_H_

#include "../../TlmCmd/common_cmd_packet.h"

/**
 * @brief 指定したブロックコマンドを、次にタスクリストに展開するものとして登録するコマンド
 */
CCP_CmdRet Cmd_TDSP_SET_TASK_LIST(const CommonCmdPacket* packet);

#endif // TASK_DISPATCHER_CMD_H_
