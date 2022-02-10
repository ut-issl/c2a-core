#pragma section REPRO
#include "tlm_cmd_hirate.h"

#include "../../../../Applications/app_registry.h"
#include "../../../../TlmCmd/block_command_definitions.h"

#include <src_core/TlmCmd/block_command_loader.h>


void BCL_load_tlm_cmd_hirate(void)
{
  BCL_tool_register_app    (0, AR_RTC_DISPATCHER);
  BCL_tool_register_app    (1, AR_TLC1_DISPATCHER);
  BCL_tool_register_app    (2, AR_TLC2_DISPATCHER);
  BCL_tool_register_app    (3, AR_DI_GS_MST_PH);
  BCL_tool_register_app    (4, AR_DI_GS_RPT_PH);
}

#pragma section
