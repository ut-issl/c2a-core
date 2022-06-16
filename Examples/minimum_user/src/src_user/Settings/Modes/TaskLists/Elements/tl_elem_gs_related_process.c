#pragma section REPRO
#include "tl_elem_gs_related_process.h"

#include "../../../../TlmCmd/block_command_definitions.h"
#include "../../../../Applications/app_registry.h"

#include <src_core/TlmCmd/block_command_loader.h>

void BCL_load_tl_elem_gs_related_process(void)
{
  BCL_tool_register_app(0, AR_DI_GS_CMD_PH);
  BCL_tool_register_app(1, AR_GSC_DISPATCHER);
}

#pragma section
