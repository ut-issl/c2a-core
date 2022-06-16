#pragma section REPRO
/**
 * @file
 * @brief BCLの prepare param が上手くいくかどうか確認. テスト用なので実機環境ではビルド対象に含めなくても良い
 */
#include "nbc_header.h"

#include <src_core/TlmCmd/block_command_loader.h>

#include "../command_definitions.h"
#include "../../Drivers/Aocs/aobc_command_definitions.h"
#include "../block_command_definitions.h"

void BCL_load_test_bcl(void)
{
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_prepare_param_uint8(22);
  BCL_tool_prepare_param_uint16(333);
  BCL_tool_register_cmd(1, Cmd_CODE_MM_SET_TRANSITION_TABLE);

  BCL_tool_prepare_param_uint8(44);
  BCL_tool_prepare_param_uint32(555555);
  BCL_tool_register_cmd(2, Cmd_CODE_TLCD_CLEAR_TIMELINE_AT);

  BCL_tool_prepare_param_double(12345.6789);
  BCL_tool_register_cmd(3, Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH);

  BCL_tool_prepare_param_double(-12345.6789);
  BCL_tool_register_cmd(4, Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH);

  // other_obc コマンドのチェック
  // FIXME: other OBC のコマンド数が 本OBC より多くなるとキャストできなくて困る
  BCL_tool_prepare_param_uint8(3);
  BCL_tool_register_cmd_to_other_obc(5, APID_AOBC_CMD, (CMD_CODE)AOBC_Cmd_CODE_MM_START_TRANSITION);
}

#pragma section
