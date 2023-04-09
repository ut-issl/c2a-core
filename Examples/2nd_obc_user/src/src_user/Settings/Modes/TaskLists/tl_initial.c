#pragma section REPRO
#include "tl_initial.h"

#include "../../../Applications/app_registry.h"
#include "../../../TlmCmd/block_command_definitions.h"

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_tl_initial(void)
{
  BCL_tool_register_combine( 0, BC_AR_GS_RELATED_PROCESS);
  BCL_tool_register_app    ( 8, AR_TLC_DISPATCHER_GS);
  BCL_tool_register_combine(10, BC_AC_TLM_CMD_HIRATE);
  BCL_tool_register_app    (40, AR_EVENT_UTILITY);
  BCL_tool_register_rotate (95, BC_AR_DEBUG_DISPLAY_INI);
}

#pragma section
