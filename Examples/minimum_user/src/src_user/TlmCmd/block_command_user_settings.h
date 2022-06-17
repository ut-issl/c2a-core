#ifndef BLOCK_COMMAND_USER_SETTINGS_H_
#define BLOCK_COMMAND_USER_SETTINGS_H_

#include <src_core/TlmCmd/block_command_table.h>
#include <src_core/TlmCmd/block_command_executor.h>

#define BCUS_MRAM_BCT_BLOCKS (10) // MRAMに確保する

/**
 * @brief BCT, BCE に関してのメモリ確保や getter, setter を user settings として上書きする
 * @param[in] block_command_table: 初期化する BlockCommandTable*
 * @param[in] block_command_executor: 初期化する BlockCommandExecutor*
 * @note SRAM, 単一ではなく MRAM上, 三重冗長で置くなどの際に上書きで使用
 */
void BCUS_load_user_settings(BlockCommandTable* block_command_table, BlockCommandExecutor* block_command_executor);

/**
 * @brief その block をクリアすべきかどうか判別する
 * @param[in] block: BC の id
 * @return 0: No, 1: Yes
 * @note 不揮発であれば clear しない. 揮発であれば clear する.
 */
int BCUS_bc_should_clear(const bct_id_t block);

#endif
