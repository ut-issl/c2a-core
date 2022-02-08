#pragma section REPRO
#include "block_command_definitions.h"
#include <src_core/TlmCmd/block_command_loader.h>
#include <src_core/TlmCmd/block_command_table.h>
#include <string.h> // for memcpy
#include "command_definitions.h"

#include "../Settings/Modes/Transitions/InitialSL.h"
#include "../Settings/Modes/Transitions/NopSL.h"
#include "../Settings/Modes/TaskLists/InitialTL.h"
#include "../Settings/Modes/TaskLists/Composition/tlm_cmd_hirate.h"
#include "../Settings/Modes/TaskLists/Composition/debug_display.h"
#include "../Settings/Modes/TaskLists/Composition/drivers_update.h"
#include "../Settings/Modes/TaskLists/Composition/gs_related_process.h"
#include "./NormalBlockCommandDefinition/nbc_header.h"

/**
 * @brief
 * 各ブロックコマンドIDに中身の初期値をロードしていく
 */
void BC_load_defaults(void)
{
  // Block Cmds for Mode Transition
  BCL_load_bc(BC_SL_START_UP_TO_INITIAL,    BCL_load_start_up_to_initial);
  BCL_load_bc(BC_SL_NOP,                    BCL_load_nop_sl);

  // Block Cmds for TaskList
  // こいつは TDSP_initialize() で展開される
  BCL_load_bc(BC_TL_START_UP,               BCL_load_initial_mode);
  BCL_load_bc(BC_TL_INITIAL,                BCL_load_initial_mode);

  // Block Cmds for Application Rotation / Combination
  BCL_load_bc(BC_AR_DRIVERS_UPDATE_INI,     BCL_load_drivers_update_initial);
  BCL_load_bc(BC_AR_DEBUG_DISPLAY_INI,      BCL_load_debug_display_initial);
  BCL_load_bc(BC_AR_GS_RELATED_PROCESS,     BCL_load_gs_related_process);
  BCL_load_bc(BC_AC_TLM_CMD_HIRATE,         BCL_load_tlm_cmd_hirate);

  // Normal Block Cmds

  // === CDH ===
  BCL_load_bc(BC_HK_CYCLIC_TLM,             BCL_load_start_hk_tlm);

  // === TEST ===
  BCL_load_bc(BC_TEST_EH_RESPOND,           BCL_load_respond_eh_for_test);
}

#pragma section
