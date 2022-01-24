#pragma section REPRO
/**
 * @file
 * @brief BCTの実行周りの関数, パラメーター
 * @note  Block Command ExeInfoは
 *        Block Command Table (BCT) の実行側のパラメーターである.
 *        現状 Cmd のデータ保存側と実行側は分離されておらず, BCT のその実行状態はここに保存される.
 *        よって実行パラメーターは BCT と同数だけ (BCT_MAX_BLOCKS) 確保される.
 */

#include <string.h>

#include "block_command_executor.h"
#include "packet_handler.h"
#include "../Library/endian_memcpy.h"
#include "../System/TimeManager/time_manager.h"
#include "common_cmd_packet_util.h"

static CTCP packet_;

static BlockCommandExecutor block_command_executor_;
const BlockCommandExecutor* const block_command_executor = &block_command_executor_;

// default (SRAM, 冗長無し) の getter, setter
static BCE_Params* BCE_get_bc_exe_params_default_(const bct_id_t block);
static void BCE_set_bc_exe_params_default_(const bct_id_t block, const BCE_Params* bc_exe_params);

// BlockCmdExeFunc をラップした static getter, setter
// staticであるので，const をつけていない
// user実装次第では，三重冗長化された不揮発メモリ上のBCTを扱う可能性もあるため，
// 取得したポインタを介して値を更新した場合，setterを呼び出す．
// block の引数アサーションはしていない
static BCE_Params* BCE_get_bc_exe_params_(const bct_id_t block);
static void BCE_set_bc_exe_params_(const bct_id_t block, const BCE_Params* bc_exe_params);

/**
 * @brief rotator の実行主体
 * @param[in] block: BC の idx
 * @return CCP_EXEC_STS
 * @note  rotator はひたすらその BC に含まれる Cmd をループで実行し続ける
 *        interval[cycle] ごとに 1つの Cmd が実行される.
 */
static CCP_EXEC_STS BCT_rotate_block_cmd_(bct_id_t block);

/**
 * @brief BC をまとめて一括で実行する
 * @param[in] block: BC の idx
 * @return CCP_EXEC_STS
 * @note  BC の内部で BC を実行する時など
 */
static CCP_EXEC_STS BCT_combine_block_cmd_(bct_id_t block);

/**
 * @brief BC をまとめて一括で実行する
 * @param[in] block: BC の idx
 * @param[in] limit_step: 実行制限時間 [step]
 * @return CCP_EXEC_STS
 * @note 時間を制限を設けてBCを実行したい時など
 */
static CCP_EXEC_STS BCT_timelimit_combine_block_cmd_(bct_id_t block, step_t limit_step);

/**
 * @brief 時間制限付きの combiner
 * @param[in] block: BC の idx
 * @return BCE_Params*
 */
static BCE_Params* BCE_get_bc_exe_params_default_(const bct_id_t block)
{
  return block_command_executor_.bc_exe_params[block];
}

static void BCE_set_bc_exe_params_default_(const bct_id_t block, const BCE_Params* bc_exe_params)
{
  if ((unsigned char*)block_command_executor_.bc_exe_params[block] == (const unsigned char*)bc_exe_params) return;
  memcpy(block_command_executor_.bc_exe_params[block], bc_exe_params, sizeof(BCE_Params));
}

static void BCE_set_bc_exe_params_(const bct_id_t block, const BCE_Params* bc_exe_params)
{
  block_command_executor_.bc_exe_func[block].set_bc_exe_params_(block, bc_exe_params);
}

static BCE_Params* BCE_get_bc_exe_params_(const bct_id_t block)
{
  return block_command_executor_.bc_exe_func[block].get_bc_exe_params_(block);
}

const BCE_Params* BCE_get_bc_exe_params(const bct_id_t block)
{
  return block_command_executor_.bc_exe_func[block].get_bc_exe_params_(block);
}

void BCE_load_default_func_pointer(void)
{
  bct_id_t block;
  for (block = 0; block < BCT_MAX_BLOCKS; ++block)
  {
    block_command_executor_.bc_exe_func[block].get_bc_exe_params_ = BCE_get_bc_exe_params_default_;
    block_command_executor_.bc_exe_func[block].set_bc_exe_params_ = BCE_set_bc_exe_params_default_;
  }
}

BCT_ACK BCE_clear_block(const bct_id_t block)
{
  BCE_Params* bc_exe_params;
  if (block >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;

  bc_exe_params = BCE_get_bc_exe_params_(block);
  bc_exe_params->is_active = 0;
  BCE_set_bc_exe_params_(block, bc_exe_params);

  BCE_reset_rotator_info(block);
  BCE_reset_combiner_info(block);

  return BCT_SUCCESS;
}

CCP_EXEC_STS Cmd_BCT_ACTIVATE_BLOCK(const CommonCmdPacket* packet)
{
  BCT_ACK ack;
  (void)packet;

  ack = BCE_activate_block();

  return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
}

BCT_ACK BCE_activate_block(void)
{
  bct_id_t block = block_command_table->pos.block;
  BCE_Params* bc_exe_params;

  if (block >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;

  bc_exe_params = BCE_get_bc_exe_params_(block);

  if (BCT_get_bc_length(block) != block_command_table->pos.cmd)
  {
    return BCT_DEFECTIVE_BLOCK;
  }

  bc_exe_params->is_active = 1;

  BCE_set_bc_exe_params_(block, bc_exe_params);

  return BCT_SUCCESS;
}

CCP_EXEC_STS Cmd_BCT_ACTIVATE_BLOCK_BY_ID(const CommonCmdPacket* packet)
{
  bct_id_t block;
  BCT_ACK ack;

  if (CCP_get_param_len(packet) != SIZE_OF_BCT_ID_T)
  {
    // パラメータはブロック番号
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  endian_memcpy(&block, CCP_get_param_head(packet), SIZE_OF_BCT_ID_T);
  ack = BCE_activate_block_by_id(block);

  return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
}

CCP_EXEC_STS Cmd_BCT_INACTIVATE_BLOCK_BY_ID(const CommonCmdPacket* packet)
{
  bct_id_t block;
  BCT_ACK ack;

  if (CCP_get_param_len(packet) != SIZE_OF_BCT_ID_T)
  {
    // パラメータはブロック番号
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  endian_memcpy(&block, CCP_get_param_head(packet), SIZE_OF_BCT_ID_T);

  ack = BCE_inactivate_block_by_id(block);

  return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
}

BCT_ACK BCE_activate_block_by_id(bct_id_t block)
{
  BCE_Params* bc_exe_params;

  if (block < 0 || block >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;

  bc_exe_params = BCE_get_bc_exe_params_(block);
  bc_exe_params->is_active = 1;
  BCE_set_bc_exe_params_(block, bc_exe_params);

  return BCT_SUCCESS;
}

BCT_ACK BCE_inactivate_block_by_id(bct_id_t block)
{
  BCE_Params* bc_exe_params;

  if (block < 0 || block >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;

  bc_exe_params = BCE_get_bc_exe_params_(block);
  bc_exe_params->is_active = 0;
  BCE_set_bc_exe_params_(block, bc_exe_params);

  return BCT_SUCCESS;
}

CCP_EXEC_STS Cmd_BCT_ROTATE_BLOCK(const CommonCmdPacket* packet)
{
  bct_id_t block;

  if (CCP_get_param_len(packet) != SIZE_OF_BCT_ID_T)
  {
    // パラメータはブロック番号
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // パラメータを読み出し。
  endian_memcpy(&block, CCP_get_param_head(packet), SIZE_OF_BCT_ID_T);

  return BCT_rotate_block_cmd_(block);
}

static CCP_EXEC_STS BCT_rotate_block_cmd_(bct_id_t block)
{
  CCP_EXEC_STS ack;
  BCE_Params* bc_exe_params;
  BCT_Pos pos;

  if (block >= BCT_MAX_BLOCKS) return BCT_convert_bct_ack_to_ctcp_exec_sts(BCT_INVALID_BLOCK_NO);

  bc_exe_params = BCE_get_bc_exe_params_(block);
  if (!bc_exe_params->is_active) return CCP_EXEC_ILLEGAL_CONTEXT;
  if (bc_exe_params->rotate.interval == 0) return CCP_EXEC_ILLEGAL_CONTEXT;

  ++bc_exe_params->rotate.counter;
  if (bc_exe_params->rotate.counter < bc_exe_params->rotate.interval)
  {
    BCE_set_bc_exe_params_(block, bc_exe_params);
    return CCP_EXEC_SUCCESS; // スキップ
  }

  bc_exe_params->rotate.counter = 0;

  ++bc_exe_params->rotate.next_cmd;
  bc_exe_params->rotate.next_cmd %= BCT_get_bc_length(block);

  BCE_set_bc_exe_params_(block, bc_exe_params);

  BCT_make_pos(&pos, block, bc_exe_params->rotate.next_cmd);
  BCT_load_cmd(&pos, &packet_);
  ack = PH_dispatch_command(&packet_);

  return ack;
}

CCP_EXEC_STS Cmd_BCT_COMBINE_BLOCK(const CommonCmdPacket* packet)
{
  bct_id_t block;

  if (CCP_get_param_len(packet) != SIZE_OF_BCT_ID_T)
  {
    // パラメータはブロック番号
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // パラメータを読み出し。
  endian_memcpy(&block, CCP_get_param_head(packet), SIZE_OF_BCT_ID_T);

  return BCT_combine_block_cmd_(block);
}

static CCP_EXEC_STS BCT_combine_block_cmd_(bct_id_t block)
{
  uint8_t cmd;
  CCP_EXEC_STS ack;
  uint8_t length;

  if (block >= BCT_MAX_BLOCKS) return BCT_convert_bct_ack_to_ctcp_exec_sts(BCT_INVALID_BLOCK_NO);

  length = BCT_get_bc_length(block);

  if (!BCE_is_active(block)) return CCP_EXEC_ILLEGAL_CONTEXT;

  for (cmd = 0; cmd < length; ++cmd)
  {
    BCT_Pos pos;
    pos.block = block;
    pos.cmd = cmd;
    BCT_load_cmd(&pos, &packet_);
    ack = PH_dispatch_command(&packet_);

    if (ack != CCP_EXEC_SUCCESS) return ack;
  }

  return CCP_EXEC_SUCCESS;
}

// 2019/10/01 追加
// 時間制限付きコンバイナ
// （時間が来たら打ち切り．したがって，必ず設定時間はすぎる）
CCP_EXEC_STS Cmd_BCT_TIMELIMIT_COMBINE_BLOCK(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  bct_id_t block;
  step_t  limit_step;

  if (CCP_get_param_len(packet) != SIZE_OF_BCT_ID_T + 1)
  {
    // パラメータはブロック番号 + 制限時間 [step]
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // パラメータを読み出し。
  endian_memcpy(&block, param, SIZE_OF_BCT_ID_T);
  limit_step = param[SIZE_OF_BCT_ID_T];

  return BCT_timelimit_combine_block_cmd_(block, limit_step);
}

static CCP_EXEC_STS BCT_timelimit_combine_block_cmd_(bct_id_t block, step_t limit_step)
{
  uint8_t cmd;
  uint8_t length;
  CCP_EXEC_STS ack;
  BCE_Params* bc_exe_params;

  ObcTime start = TMGR_get_master_clock();
  ObcTime finish;
  step_t diff;

  if (block >= BCT_MAX_BLOCKS) return BCT_convert_bct_ack_to_ctcp_exec_sts(BCT_INVALID_BLOCK_NO);

  bc_exe_params = BCE_get_bc_exe_params_(block);
  if (!bc_exe_params->is_active) return CCP_EXEC_ILLEGAL_CONTEXT;

  ++bc_exe_params->timelimit_combine.call_num;
  length = BCT_get_bc_length(block);

  // ↓ちょっとこれ微妙かも...
  // 値が増えないように，割合が変わらないように適当に下げてる感じ
  if (bc_exe_params->timelimit_combine.call_num >= 0xFFFF - 16)
  {
    bc_exe_params->timelimit_combine.call_num /= 8;
    bc_exe_params->timelimit_combine.timeover_num /= 8;
  }

  for (cmd = 0; cmd < length; ++cmd)
  {
    BCT_Pos pos;
    pos.block = block;
    pos.cmd = cmd;
    BCT_load_cmd(&pos, &packet_);
    ack = PH_dispatch_command(&packet_);
    if (ack != CCP_EXEC_SUCCESS)
    {
      BCE_set_bc_exe_params_(block, bc_exe_params);
      return ack;
    }

    // 時間判定
    finish = TMGR_get_master_clock();
    diff = OBCT_diff_in_step(&start, &finish);
    if (diff >= limit_step)
    {
      // 途中で中断
      ++bc_exe_params->timelimit_combine.timeover_num;
      bc_exe_params->timelimit_combine.last_timeover_cmd_pos = cmd;
      if (bc_exe_params->timelimit_combine.last_timeover_cmd_pos < bc_exe_params->timelimit_combine.worst_cmd_pos)
      {
        bc_exe_params->timelimit_combine.worst_cmd_pos = bc_exe_params->timelimit_combine.last_timeover_cmd_pos;
      }

      BCE_set_bc_exe_params_(block, bc_exe_params);
      return CCP_EXEC_SUCCESS;    // 異常ではないのでこれを返す
    }
  }

  BCE_set_bc_exe_params_(block, bc_exe_params);

  // 最後まで実行できた
  return CCP_EXEC_SUCCESS;
}

BCT_ACK BCE_reset_rotator_info(const bct_id_t block)
{
  BCE_Params* bc_exe_params;
  if (block >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;

  bc_exe_params = BCE_get_bc_exe_params_(block);

  bc_exe_params->rotate.counter = 0;
  bc_exe_params->rotate.interval = 1;
  bc_exe_params->rotate.next_cmd = 0;

  BCE_set_bc_exe_params_(block, bc_exe_params);

  return BCT_SUCCESS;
}

BCT_ACK BCE_reset_combiner_info(const bct_id_t block)
{
  BCE_Params* bc_exe_params;
  if (block >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;

  bc_exe_params = BCE_get_bc_exe_params_(block);

  bc_exe_params->timelimit_combine.call_num = 0;
  bc_exe_params->timelimit_combine.timeover_num = 0;
  bc_exe_params->timelimit_combine.last_timeover_cmd_pos = 0xFF;
  bc_exe_params->timelimit_combine.worst_cmd_pos = 0xFF;

  BCE_set_bc_exe_params_(block, bc_exe_params);

  return BCT_SUCCESS;
}

uint8_t BCE_is_active(const bct_id_t block)
{
  if (block >= BCT_MAX_BLOCKS) return 0;
  return (uint8_t)BCE_get_bc_exe_params(block)->is_active;
}

BCT_ACK BCE_swap_address(const bct_id_t block_a, const bct_id_t block_b)
{
  BCE_Func tmp_bce_func;
  BCE_Params* tmp_params_pointer;

  if (block_a >= BCT_MAX_BLOCKS || block_b >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;
  if (BCE_is_active(block_a) || BCE_is_active(block_b)) return BCT_DEFECTIVE_BLOCK;

  tmp_params_pointer = block_command_executor_.bc_exe_params[block_a];
  block_command_executor_.bc_exe_params[block_a] = block_command_executor_.bc_exe_params[block_b];
  block_command_executor_.bc_exe_params[block_b] = tmp_params_pointer;

  memcpy(&tmp_bce_func, &block_command_executor_.bc_exe_func[block_a], sizeof(BCE_Func));
  memcpy(&block_command_executor_.bc_exe_func[block_a], &block_command_executor_.bc_exe_func[block_b], sizeof(BCE_Func));
  memcpy(&block_command_executor_.bc_exe_func[block_b], &tmp_bce_func, sizeof(BCE_Func));

  return BCT_SUCCESS;
}

BCT_ACK BCE_swap_contents(const bct_id_t block_a, const bct_id_t block_b)
{
  BCE_Params* params_a;
  BCE_Params* params_b;
  BCE_Params  tmp_params;

  if (block_a >= BCT_MAX_BLOCKS || block_b >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;
  if (BCE_is_active(block_a) || BCE_is_active(block_b)) return BCT_DEFECTIVE_BLOCK;

  params_a = BCE_get_bc_exe_params_(block_a);
  params_b = BCE_get_bc_exe_params_(block_b);

  memcpy(&tmp_params, params_a, sizeof(BCE_Params));
  BCE_set_bc_exe_params_(block_a, params_b);
  BCE_set_bc_exe_params_(block_b, &tmp_params);

  return BCT_SUCCESS;
}

CCP_EXEC_STS Cmd_BCT_RESET_ROTATOR_INFO(const CommonCmdPacket* packet)
{
  bct_id_t block;

  if (CCP_get_param_len(packet) != SIZE_OF_BCT_ID_T)
  {
    // パラメータはブロック番号
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // パラメータを読み出し。
  endian_memcpy(&block, CCP_get_param_head(packet), SIZE_OF_BCT_ID_T);

  return BCT_convert_bct_ack_to_ctcp_exec_sts(BCE_reset_rotator_info(block));
}

CCP_EXEC_STS Cmd_BCT_RESET_COMBINER_INFO(const CommonCmdPacket* packet)
{
  bct_id_t block;

  if (CCP_get_param_len(packet) != SIZE_OF_BCT_ID_T)
  {
    // パラメータはブロック番号
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // パラメータを読み出し。
  endian_memcpy(&block, CCP_get_param_head(packet), SIZE_OF_BCT_ID_T);

  return BCT_convert_bct_ack_to_ctcp_exec_sts(BCE_reset_combiner_info(block));
}

// 長さ10のBCにNOPを登録するコマンド. 使用前提が狭すぎるか??
// パス運用時に使用するので, 一応厳密にしておいたほうがいい気もする.
CCP_EXEC_STS Cmd_BCT_FILL_NOP(const CommonCmdPacket* packet)
{
  cycle_t num_nop;
  cycle_t ti;

  num_nop = (cycle_t)CCP_get_param_head(packet)[0];

  if (num_nop > 10 || num_nop < 1) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (block_command_table->pos.cmd + num_nop != 10) return CCP_EXEC_ILLEGAL_CONTEXT;

  for (ti = 11 - num_nop; ti < 11; ++ti)
  {
    CCP_form_tlc(&packet_, ti, Cmd_CODE_NOP, NULL, 0);
    BCT_register_cmd(&packet_);
  }

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_BCT_SET_ROTATE_INTERVAL(const CommonCmdPacket* packet)
{
  const unsigned char* param = CCP_get_param_head(packet);
  bct_id_t block;
  uint16_t interval;
  BCE_Params* bc_exe_params;

  if (CCP_get_param_len(packet) != (SIZE_OF_BCT_ID_T + 2))
  {
    // パラメータはブロック番号2Byte＋周期2Byte = 4Bytes
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // パラメータを読み出し。
  endian_memcpy(&block, param, SIZE_OF_BCT_ID_T);
  endian_memcpy(&interval, param + SIZE_OF_BCT_ID_T, 2);

  if (interval == 0 || block >= BCT_MAX_BLOCKS)
  {
    // 0で割りに行くのでここではじく
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  bc_exe_params = BCE_get_bc_exe_params_(block);
  bc_exe_params->rotate.interval = interval;
  BCE_set_bc_exe_params_(block, bc_exe_params);

  return CCP_EXEC_SUCCESS;
}

#pragma section
