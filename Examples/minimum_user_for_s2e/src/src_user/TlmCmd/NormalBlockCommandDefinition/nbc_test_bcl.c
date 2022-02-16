#pragma section REPRO
/**
 * @file
 * @brief BCLの prepare param が上手くいくかどうか確認. あとで消す
 */
#include "nbc_header.h"

#include <src_core/TlmCmd/block_command_loader.h>

#include "../command_definitions.h"
#include "../block_command_definitions.h"

void BCL_load_test_bcl(void)
{
  double unixtime = 12345.6789;
  BCL_tool_prepare_param_double(unixtime);
  BCL_tool_prepare_param_uint32(0);
  BCL_tool_prepare_param_uint32(0);
  BCL_tool_register_cmd(1, Cmd_CODE_TMGR_UPDATE_UNIXTIME);

  BCL_tool_prepare_param_double(unixtime+2000000);
  BCL_tool_prepare_param_uint32(0);
  BCL_tool_prepare_param_uint32(0);
  BCL_tool_register_cmd(2, Cmd_CODE_TMGR_UPDATE_UNIXTIME);

  BCL_tool_prepare_param_double(unixtime+3000000);
  BCL_tool_prepare_param_uint32(0);
  BCL_tool_prepare_param_uint32(0);
  BCL_tool_register_cmd(3, Cmd_CODE_TMGR_UPDATE_UNIXTIME);
}

#pragma section
