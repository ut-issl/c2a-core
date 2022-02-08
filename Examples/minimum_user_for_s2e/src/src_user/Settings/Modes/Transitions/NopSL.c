#pragma section REPRO
#include "NopSL.h"

#include "../../../TlmCmd/command_definitions.h"

#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/TimeManager/obc_time.h>

void BCL_load_nop_sl(void)
{
  BCL_tool_register_cmd(OBCT_sec2cycle(5), Cmd_CODE_MM_FINISH_TRANSITION);
}

#pragma section
