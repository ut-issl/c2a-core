#pragma section REPRO
#include "InitialTL.h"

#include "../../../Applications/app_registry.h"
#include "../../../TlmCmd/block_command_definitions.h"

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_initial_mode(void)
{
  BCL_tool_register_rotate ( 0, BC_AR_GS_RELATED_PROCESS);
  BCL_tool_register_app    ( 4, AR_TLC0_DISPATCHER);
  BCL_tool_register_combine( 6, BC_AC_TLM_CMD_HIRATE);
  BCL_tool_register_rotate (30, BC_AR_DRIVERS_UPDATE_INI);
  BCL_tool_register_app    (40, AR_EVENT_UTILITY);
  BCL_tool_register_app    (50, AR_ANOMALY_HANDLER);
  BCL_tool_register_rotate (95, BC_AR_DEBUG_DISPLAY_INI);
}

#pragma section
