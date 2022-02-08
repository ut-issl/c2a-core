#pragma section REPRO

#include "block_command_user_settings.h"

static BCT_Table  BCUS_bc_table_[BCT_MAX_BLOCKS];      //!< BCT の blocks の本体
static BCE_Params BCUS_bc_exe_params_[BCT_MAX_BLOCKS]; //!< BCE_Params の本体

void BCUS_load_user_settings(BlockCommandTable* block_command_table, BlockCommandExecutor* block_command_executor)
{
  bct_id_t block;

  for (block = 0; block < BCT_MAX_BLOCKS; ++block)
  {
    block_command_table->blocks[block] = &BCUS_bc_table_[block];
    block_command_executor->bc_exe_params[block] = &BCUS_bc_exe_params_[block];
  }
}

int BCUS_bc_should_clear(const bct_id_t block)
{
  (void)block;
  return 1;
}

#pragma section
