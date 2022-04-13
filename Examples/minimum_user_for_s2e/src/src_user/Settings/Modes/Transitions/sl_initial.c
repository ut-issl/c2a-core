#pragma section REPRO
#include "sl_initial.h"

#include "../../../TlmCmd/block_command_definitions.h"
#include "../../../TlmCmd/command_definitions.h"

#include "../../../Drivers/Com/gs.h"

#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/System/TimeManager/obc_time.h>

void BCL_load_sl_start_up_to_initial(void)
{
  BCL_tool_register_deploy(OBCT_sec2cycle(1), BC_HK_CYCLIC_TLM, TLCD_ID_DEPLOY_TLM);

  BCL_tool_prepare_param_uint8((uint8_t)GS_PORT_TYPE_UART);
  BCL_tool_register_cmd(OBCT_sec2cycle(2), Cmd_CODE_DI_GS_SET_INFO);

  BCL_tool_register_cmd(OBCT_sec2cycle(10), Cmd_CODE_MM_FINISH_TRANSITION);
}

#pragma section
