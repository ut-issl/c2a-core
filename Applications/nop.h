/**
 * @file
 * @brief NOP (No Operation) App
 * @note  C2Aは NOP があることを前提としている
 *        この App は必ず App 登録し， Cmd_NOP も必ずコマンド登録しておくこと
 */
#ifndef NOP_H_
#define NOP_H_

#include "../System/ApplicationManager/app_info.h"
#include "../CmdTlm/common_tlm_cmd_packet.h"

AppInfo NOP_create_app(void);

CCP_EXEC_STS Cmd_NOP(const CTCP* packet);

#endif
