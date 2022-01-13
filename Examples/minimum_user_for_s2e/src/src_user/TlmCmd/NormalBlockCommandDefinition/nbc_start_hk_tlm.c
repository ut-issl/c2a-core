#pragma section REPRO
#include "nbc_header.h"

#include "../block_command_definitions.h"
#include "../command_definitions.h"
#include "../telemetry_definitions.h"

#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_start_hk_tlm(void)
{
  BCL_tool_prepare_param_uint8(0x40);
  BCL_tool_prepare_param_uint8(Tlm_CODE_HK);
  BCL_tool_prepare_param_uint8(1);
  BCL_tool_register_cmd(1, Cmd_CODE_GENERATE_TLM);

  BCL_tool_register_deploy (10, BC_HK_CYCLIC_TLM, TL_ID_DEPLOY_TLM);
}

#pragma section
