/**
 * @file
 * @brief BCTの実行周りの関数, パラメーター
 * @note  Block Command ExeInfoは
 *        Block Command Table (BCT) の実行側のパラメーターである.
 *        現状 Cmd のデータ保存側と実行側は分離されておらず, BCT のその実行状態はここに保存される.
 *        よって実行パラメーターは BCT と同数だけ (BCT_MAX_BLOCKS) 確保される.
 */
#ifndef BLOCK_COMMAND_EXECUTOR_H_
#define BLOCK_COMMAND_EXECUTOR_H_

#include "common_cmd_packet.h"
#include "block_command_table.h"

/**
 * @struct BCE_Params
 * @brief  BCT の実行パラメーターを保存する構造体
 */
typedef struct
{
  uint8_t is_active; //!< 有効かどうか．1: 有効, 0: 無効
  struct
  {
    uint16_t counter;  //!< interval の計算のための counter[cycle]
    uint16_t interval; //!< interval[cycle] ごとに実行される (大抵1)
    uint8_t  next_cmd; //!< 次に実行される cmd
  } rotate;
  struct
  {
    uint16_t call_num;              //!< Cmd_BCE_TIMELIMIT_COMBINE_BLOCKが呼ばれるたびにインクリメント
    uint16_t timeover_num;          //!< 時間制限に引っかかったらインクリメント
    uint8_t  last_timeover_cmd_pos; //!< 前回時間制限に引っかかったときのcmd pos
    uint8_t  worst_cmd_pos;         //!< 最悪ケースのcmd pos
  } timelimit_combine;
} BCE_Params;

/**
 * @struct BCE_Func
 * @brief  BCE_Params の getter, setter (private)
 * @note   公開されているが private 扱い. getter はラップされたものが公開されているのでそちらを使うこと
 *         存在する理由は中身が MRAM など三重冗長化されている場所にあることもあるため
 */
typedef struct
{
  BCE_Params* (*get_bc_exe_params_)(const bct_id_t block);
  void        (*set_bc_exe_params_)(const bct_id_t block, const BCE_Params* bc_params);
} BCE_Func;

/**
 * @struct BlockCommandExecutor
 * @brief  実行周り全体の struct
 */
typedef struct
{
  BCE_Params* bc_exe_params[BCT_MAX_BLOCKS];    //!< パラメタ．不揮発化出来るようにポインタで確保
  BCE_Func    bc_exe_func[BCT_MAX_BLOCKS];      //!< BCE_Params に対する操作関数．各 BC ごとに確保
} BlockCommandExecutor;

extern const BlockCommandExecutor* const block_command_executor;

/**
 * @brief BCE_Params の const getter
 * @param  block: BC の idx
 * @return BCE_Params
 */
const BCE_Params* BCE_get_bc_exe_params(const bct_id_t block);

/**
 * @brief BCE_Params にデフォルトの関数ポインタを入れる
 * @param  void
 * @return void
 */
void BCE_load_default_func_pointer(void);

/**
 * @brief 指定された block の BCE を初期化
 * @param[in] block: BC の idx
 * @return BCT_ACK
 */
BCT_ACK BCE_clear_block(const bct_id_t block);

/**
 * @brief block_command_table->pos の BC を activate する
 * @param  void
 * @return BCT_ACK
 */
BCT_ACK BCE_activate_block(void);

// 指定された BC を activate, inactivate
BCT_ACK BCE_activate_block_by_id(bct_id_t block);
BCT_ACK BCE_inactivate_block_by_id(bct_id_t block);

// 指定された BCE のうち rotator, combiner を initialize
BCT_ACK BCE_reset_rotator_info(const bct_id_t block);
BCT_ACK BCE_reset_combiner_info(const bct_id_t block);

/**
 * @brief BCT の length の getter
 * @note アサーションはされない．有効でない block の場合， 0: 無効 を返す
 * @param[in] block: BC の idx
 * @return is_active
 */
uint8_t BCE_is_active(const bct_id_t block);

/**
 * @brief BCE の params の指すポインタ & func を入れ替える
 * @param[in] block_a: 入れ替える BC
 * @param[in] block_b: 入れ替える BC
 * @return BCT_ACK 実行結果
 * @note ID1:HK:SRAM, ID2:EH:MRAM があった時に ID1:EH:MRAM, ID2:HK:SRAM が出来る (交換されるのはparam)
 *       `BCE_swap_contents` と組み合わせることで番号 - 中身はそのままで RAM だけを切り替えることが出来る
 *       必ず `BCT_swap_address` と共に使用すること
 */
BCT_ACK BCE_swap_address(const bct_id_t block_a, const bct_id_t block_b);

/**
 * @brief BCE の params の指す中身を入れ替える
 * @param[in] block_a: 入れ替える BC
 * @param[in] block_b: 入れ替える BC
 * @return BCT_ACK 実行結果
 * @note ID1:EH:MRAM, ID2:HK:SRAM があった時に ID1:HK:MRAM, ID2:EH:SRAM が出来る (交換されるのはparam)
 *       `BCE_swap_address` と組み合わせることで番号 - 中身はそのままで RAM だけを切り替えることが出来る
 *       必ず `BCT_swap_contents` と共に使用すること
 */
BCT_ACK BCE_swap_contents(const bct_id_t block_a, const bct_id_t block_b);

CCP_CmdRet Cmd_BCE_ACTIVATE_BLOCK(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCE_ACTIVATE_BLOCK_BY_ID(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCE_INACTIVATE_BLOCK_BY_ID(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCE_ROTATE_BLOCK(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCE_COMBINE_BLOCK(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCE_TIMELIMIT_COMBINE_BLOCK(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCE_RESET_ROTATOR_INFO(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCE_RESET_COMBINER_INFO(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_BCE_SET_ROTATE_INTERVAL(const CommonCmdPacket* packet);

#endif
