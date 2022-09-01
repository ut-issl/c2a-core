#pragma section REPRO
/**
 * @file
 * @brief NOP (No Operation) App
 * @note  C2Aは NOP があることを前提としている
 *        この App は必ず App 登録し， Cmd_NOP も必ずコマンド登録しておくこと
 */
#include "nop.h"
#include <stddef.h>
#include "../TlmCmd/common_cmd_packet_util.h"

static void NOP_nop_(void);

AppInfo NOP_create_app(void)
{
  return AI_create_app_info("nop", NULL, NOP_nop_);
}

static void NOP_nop_() {
  // no operation
}

CCP_CmdRet Cmd_NOP(const CommonCmdPacket* packet)
{
  (void)packet;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
