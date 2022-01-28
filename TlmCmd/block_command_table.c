#pragma section REPRO
/**
 * @file
 * @brief BCTの定義, BCT周りの関数
 */

#include "block_command_table.h"

#include "string.h" // for memcpy

#include "block_command_executor.h"

#include <src_user/TlmCmd/block_command_user_settings.h>
#include <src_user/TlmCmd/block_command_definitions.h>
#include "packet_handler.h"
#include "command_analyze.h"
#include <src_user/TlmCmd/command_definitions.h>
#include "../System/TimeManager/time_manager.h"
#include "../Library/endian_memcpy.h"
#include "../System/WatchdogTimer/watchdog_timer.h"
#include "common_cmd_packet_util.h"

// FIXME: この include は依存的にダメなので， TCP → Space Packet 大工事が終わったら直す
#include "./Ccsds/space_packet.h"
#include "./Ccsds/cmd_space_packet.h"

static BlockCommandTable block_command_table_;
const BlockCommandTable* const block_command_table = &block_command_table_;

/**
 * @brief BCT_Func にデフォルトの関数ポインタを入れる
 */
static void BCT_load_default_func_pointer_(void);

// 以下4つは default (SRAM, 冗長無し) の getter, setter
static BCT_CmdData* BC_get_bc_cmd_data_default_(const BCT_Pos* pos);
static uint8_t BC_get_bc_length_default_(const bct_id_t block);
static void BC_set_bc_cmd_data_default_(const BCT_Pos* pos, const BCT_CmdData* value);
static void BC_set_bc_length_default_(const bct_id_t block, uint8_t length);

// BCT_Func をラップした static な getter, setter. なので 返り値に const がつかない
static BCT_CmdData* BCT_get_bc_cmd_data_(const BCT_Pos* pos);
static void BCT_set_bc_cmd_data_(const BCT_Pos* pos, const BCT_CmdData* value);
static void BCT_set_bc_length_(const bct_id_t block, uint8_t length);

/**
 * @brief pos, pos_stored を initialize
 */
static void BCT_clear_pos_(void);

static BCT_ACK BCT_set_position_(const BCT_Pos* pos);

/**
 * @brief pos の指す場所に packet の内容を保存 (TCP_PRM_HDR_LEN の長さも考慮)
 */
static BCT_ACK BCT_save_cmd_(const BCT_Pos* pos, const CommonCmdPacket* packet);

void BCT_initialize(void)
{
  bct_id_t i;

  BCT_load_default_func_pointer_();
  BCE_load_default_func_pointer();
  BCUS_load_user_settings(&block_command_table_, (BlockCommandExecutor*)block_command_executor); // const_cast

  for (i = 0; i < BCT_MAX_BLOCKS; ++i)
  {
    if (!BCUS_bc_should_clear(i)) continue;

    BCT_clear_block(i);
    WDT_clear_wdt();      // 念の為？
  }

  BCT_clear_pos_();

  BC_load_defaults();
}

static void BCT_load_default_func_pointer_(void)
{
  bct_id_t block;
  for (block = 0; block < BCT_MAX_BLOCKS; ++block)
  {
    block_command_table_.func[block].get_bc_cmd_data_ = BC_get_bc_cmd_data_default_;
    block_command_table_.func[block].get_bc_length_ = BC_get_bc_length_default_;

    block_command_table_.func[block].set_bc_cmd_data_ = BC_set_bc_cmd_data_default_;
    block_command_table_.func[block].set_bc_length_ = BC_set_bc_length_default_;
  }
}

static BCT_CmdData* BC_get_bc_cmd_data_default_(const BCT_Pos* pos)
{
  return &block_command_table_.blocks[pos->block]->cmds[pos->cmd];
}

static void BC_set_bc_cmd_data_default_(const BCT_Pos* pos, const BCT_CmdData* value)
{
  BCT_CmdData* cmd_data = &block_command_table_.blocks[pos->block]->cmds[pos->cmd];
  if ((unsigned char*)cmd_data == (const unsigned char*)value) return;
  memcpy(cmd_data, value, sizeof(BCT_CmdData));
}

static uint8_t BC_get_bc_length_default_(const bct_id_t block)
{
  return block_command_table_.blocks[block]->length;
}

static void BC_set_bc_length_default_(const bct_id_t block, uint8_t length)
{
  block_command_table_.blocks[block]->length = length;
}

static BCT_CmdData* BCT_get_bc_cmd_data_(const BCT_Pos* pos)
{
  return block_command_table_.func[pos->block].get_bc_cmd_data_(pos);
}

static void BCT_set_bc_cmd_data_(const BCT_Pos* pos, const BCT_CmdData* value)
{
  block_command_table_.func[pos->block].set_bc_cmd_data_(pos, value);
}

static void BCT_set_bc_length_(const bct_id_t block, uint8_t length)
{
  block_command_table_.func[block].set_bc_length_(block, length);
}

const BCT_CmdData* BCT_get_bc_cmd_data(const BCT_Pos* pos)
{
  return block_command_table_.func[pos->block].get_bc_cmd_data_(pos);
}

uint8_t BCT_get_bc_length(const bct_id_t block)
{
  if (block >= BCT_MAX_BLOCKS) return 0;
  return block_command_table_.func[block].get_bc_length_(block);
}

static void BCT_clear_pos_(void)
{
  block_command_table_.pos.block = 0;
  block_command_table_.pos.cmd   = 0;
  block_command_table_.pos_stored.block = 0;
  block_command_table_.pos_stored.cmd   = 0;
}

static BCT_ACK BCT_set_position_(const BCT_Pos* pos)
{
  BCT_ACK ack = BCT_check_position(pos);

  if (ack != BCT_SUCCESS) return ack;

  block_command_table_.pos = *pos;

  return BCT_SUCCESS;
}

BCT_ACK BCT_check_position(const BCT_Pos* pos)
{
  if (pos->block >= BCT_MAX_BLOCKS)
  {
    return BCT_INVALID_BLOCK_NO;
  }
  if (pos->cmd >= BCT_MAX_CMD_NUM)
  {
    return BCT_INVALID_CMD_NO;
  }

  return BCT_SUCCESS;
}

BCT_ACK BCT_register_cmd(const CommonCmdPacket* packet)
{
  BCT_ACK ack = BCT_check_position(&block_command_table_.pos);
  uint8_t length;

  if (ack != BCT_SUCCESS) return ack;

  length = BCT_get_bc_length(block_command_table_.pos.block);

  // 2018/06/20 コメント追記
  // 現在登録されているコマンド数よりも大きければエラー
  // つまり，登録されているテーブルの上書きはできるが，
  // 未登録のセルを飛ばして，不連続に登録することはできない，はず．
  if (block_command_table_.pos.cmd > length)
  {
    return BCT_ISORATED_CMD;
  }

  ack = BCT_save_cmd_(&block_command_table_.pos, packet);

  if (ack != BCT_SUCCESS)
  {
    return ack;
  }

  // 連続登録に備え、ポインタ位置を進める。上限超過は次回登録時リジェクトされる。
  ++(block_command_table_.pos.cmd);

  // ブロック末尾へのコマンド追加の場合はブロック長を更新する
  if (length < block_command_table_.pos.cmd)
  {
    length = block_command_table_.pos.cmd;
    BCT_set_bc_length_(block_command_table_.pos.block, length);
  }

  return BCT_SUCCESS;
}

BCT_ACK BCT_overwrite_cmd(const BCT_Pos* pos, const CommonCmdPacket* packet)
{
  BCT_ACK ack = BCT_check_position(pos);

  if (ack != BCT_SUCCESS) return ack;
  if (BCT_get_bc_length(pos->block) <= pos->cmd) return BCT_ISORATED_CMD;

  return BCT_save_cmd_(pos, packet);
}

// FIXME: 以下複数関数にわたって BlockCmdCmdData と CommonCmdPacket の変換が，現在castによって行われているが， CommonCmdPacket はユーザー定義なため，それ（=TCP以外）に対応できるようにする
//        同様の理由で memcpy なども対応する必要がある．
// FIXME: CTCP, SpacePacket 整理で直す
static BCT_ACK BCT_save_cmd_(const BCT_Pos* pos, const CommonCmdPacket* packet)
{
  // FIXME: TCPに依存しない，つまりCTCPに依存するコードにする
  BCT_ACK ack = BCT_check_position(pos);
  if (ack != BCT_SUCCESS) return ack;

  // パケット全長が想定最大長を超えている場合は異常判定
  if (CCP_get_packet_len(packet) > BCT_CMD_MAX_LENGTH) return BCT_CMD_TOO_LONG;

  // 格納可能なパケットなら内容をコピーし保存
  BCT_set_bc_cmd_data_(pos, (BCT_CmdData*)packet->packet); // const_cast

  return BCT_SUCCESS;
}

BCT_ACK BCT_load_cmd(const BCT_Pos* pos, CommonCmdPacket* packet)
{
  BCT_ACK ack = BCT_check_position(pos);
  BCT_CmdData* bc_cmddata;

  if (ack != BCT_SUCCESS) return ack;
  bc_cmddata = BCT_get_bc_cmd_data_(pos);
  memcpy(&packet->packet, bc_cmddata, BCT_CMD_MAX_LENGTH);   // FIXME: CTCPがTCP依存なので直す

  return BCT_SUCCESS;
}

BCT_ACK BCT_copy_bct(const bct_id_t dst_block, const bct_id_t src_block)
{
  uint8_t cmd;
  uint8_t length = BCT_get_bc_length(src_block);

  if (dst_block >= BCT_MAX_BLOCKS || src_block >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;
  if (BCE_is_active(dst_block)) return BCT_INVALID_BLOCK_NO;

  BCT_clear_block(dst_block);
  BCT_set_bc_length_(dst_block, BCT_get_bc_length(src_block));
  for (cmd = 0; cmd < length; ++cmd)
  {
    BCT_Pos dst_pos, src_pos;
    BCT_make_pos(&dst_pos, dst_block, cmd);
    BCT_make_pos(&src_pos, src_block, cmd);
    BCT_set_bc_cmd_data_(&dst_pos, BCT_get_bc_cmd_data(&src_pos));
  }

  return BCT_SUCCESS;
}

CMD_CODE BCT_get_id(const bct_id_t block, const uint8_t cmd)
{
  BCT_Pos pos;
  BCT_make_pos(&pos, block, cmd);
  if (BCT_check_position(&pos) != BCT_SUCCESS) return Cmd_CODE_MAX;

  return CCP_get_id((CommonCmdPacket*)BCT_get_bc_cmd_data_(&pos));
}

cycle_t BCT_get_ti(const bct_id_t block, const uint8_t cmd)
{
  BCT_Pos pos;
  BCT_make_pos(&pos, block, cmd);
  if (BCT_check_position(&pos) != BCT_SUCCESS) return 0;

  return CCP_get_ti((CommonCmdPacket*)BCT_get_bc_cmd_data_(&pos));
}

const uint8_t* BCT_get_param_head(const bct_id_t block, const uint8_t cmd)
{
  BCT_Pos pos;
  BCT_make_pos(&pos, block, cmd);
  if (BCT_check_position(&pos) != BCT_SUCCESS)
  {
    BCT_make_pos(&pos, 0, 0);
    return CCP_get_param_head((CommonCmdPacket*)BCT_get_bc_cmd_data_(&pos));
  }

  return CCP_get_param_head((CommonCmdPacket*)BCT_get_bc_cmd_data_(&pos));
}

/* 2018/11/27
 * 地上局とOBC内部のBCのポインタが混ざっておかしくならないようにするための改修
 * 詳細はC2Aドキュメント 「ブロックコマンド追加方法.md」
 */
void BCT_store_pos(void)
{
  block_command_table_.pos_stored.block = block_command_table_.pos.block;
  block_command_table_.pos_stored.cmd   = block_command_table_.pos.cmd  ;
  return;
}

void BCT_restore_pos(void)
{
  block_command_table_.pos.block = block_command_table_.pos_stored.block;
  block_command_table_.pos.cmd   = block_command_table_.pos_stored.cmd;
  return;
}

BCT_ACK BCT_make_pos(BCT_Pos* pos, const bct_id_t block, const uint8_t cmd)
{
  pos->block = block;
  pos->cmd = cmd;

  return BCT_check_position(pos);
}

BCT_ACK BCT_swap_address(const bct_id_t block_a, const bct_id_t block_b)
{
  BCT_Table* tmp_bct_table;
  BCT_Func   tmp_bct_func;

  if (block_a >= BCT_MAX_BLOCKS || block_b >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;
  if (BCE_is_active(block_a) || BCE_is_active(block_b)) return BCT_DEFECTIVE_BLOCK;

  tmp_bct_table = block_command_table_.blocks[block_a];
  block_command_table_.blocks[block_a] = block_command_table_.blocks[block_b];
  block_command_table_.blocks[block_b] = tmp_bct_table;

  memcpy(&tmp_bct_func, &block_command_table_.func[block_a], sizeof(BCT_Func));
  memcpy(&block_command_table_.func[block_a], &block_command_table_.func[block_b], sizeof(BCT_Func));
  memcpy(&block_command_table_.func[block_b], &tmp_bct_func, sizeof(BCT_Func));

  return BCT_SUCCESS;
}

BCT_ACK BCT_swap_contents(const bct_id_t block_a, const bct_id_t block_b)
{
  int i;

  uint8_t     tmp_length, length_a, length_b;
  BCT_CmdData tmp_bct_cmddata;

  if (block_a >= BCT_MAX_BLOCKS || block_b >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;
  if (BCE_is_active(block_a) || BCE_is_active(block_b)) return BCT_DEFECTIVE_BLOCK;

  length_a = BCT_get_bc_length(block_a);
  length_b = BCT_get_bc_length(block_b);
  BCT_set_bc_length_(block_a, length_b);
  BCT_set_bc_length_(block_b, length_a);

  tmp_length = (uint8_t)((length_a >= length_b) ? length_a : length_b);

  for (i = 0; i < tmp_length; ++i)
  {
    BCT_Pos pos_a, pos_b;
    BCT_CmdData* cmd_a;
    BCT_CmdData* cmd_b;

    BCT_make_pos(&pos_a, block_a, (uint8_t)i);
    BCT_make_pos(&pos_b, block_b, (uint8_t)i);

    cmd_a = BCT_get_bc_cmd_data_(&pos_a);
    cmd_b = BCT_get_bc_cmd_data_(&pos_b);
    memcpy(&tmp_bct_cmddata, cmd_a, sizeof(BCT_CmdData));
    BCT_set_bc_cmd_data_(&pos_a, cmd_b);
    BCT_set_bc_cmd_data_(&pos_b, &tmp_bct_cmddata);

    WDT_clear_wdt(); // 時間がかかるので
  }

  return BCT_SUCCESS;
}

CCP_EXEC_STS BCT_convert_bct_ack_to_ctcp_exec_sts(BCT_ACK ack)
{
  switch (ack)
  {
  case BCT_SUCCESS:
    return CCP_EXEC_SUCCESS;

  case BCT_INVALID_BLOCK_NO:
    return CCP_EXEC_ILLEGAL_PARAMETER;

  case BCT_INVALID_CMD_NO:
    return CCP_EXEC_CMD_NOT_DEFINED;

  case BCT_DEFECTIVE_BLOCK:
    return CCP_EXEC_ILLEGAL_CONTEXT;

  case BCT_CMD_TOO_LONG:
    return CCP_EXEC_ILLEGAL_PARAMETER;

  case BCT_BC_FULL:
    return CCP_EXEC_ILLEGAL_CONTEXT;

  case BCT_ZERO_PERIOD:
    return CCP_EXEC_ILLEGAL_PARAMETER;

  default:
    return CCP_EXEC_UNKNOWN;
  }
}

CCP_EXEC_STS Cmd_BCT_CLEAR_BLOCK(const CommonCmdPacket* packet)
{
  bct_id_t block;
  BCT_ACK  ack;

  if (CCP_get_param_len(packet) != SIZE_OF_BCT_ID_T)
  {
    // パラメータはブロック番号
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // パラメータを読み出し。
  endian_memcpy(&block, CCP_get_param_head(packet), SIZE_OF_BCT_ID_T);

  // 指定されたブロック番号のクリアを実行。
  ack = BCT_clear_block(block);

  return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
}

BCT_ACK BCT_clear_block(const bct_id_t block)
{
  if (block >= BCT_MAX_BLOCKS) return BCT_INVALID_BLOCK_NO;

  block_command_table_.pos.block = block;
  block_command_table_.pos.cmd = 0;
  BCT_set_bc_length_(block, 0);

  BCE_clear_block(block);

  return BCT_SUCCESS;
}

CCP_EXEC_STS Cmd_BCT_SET_BLOCK_POSITION(const CommonCmdPacket* packet)
{
  const unsigned char* param = CCP_get_param_head(packet);
  BCT_Pos pos;
  BCT_ACK ack;

  if (CCP_get_param_len(packet) != (SIZE_OF_BCT_ID_T + 1))
  {
    // パラメータはブロック番号とコマンド番号1Byte。
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // パラメータを読み出し
  endian_memcpy(&pos.block, param, SIZE_OF_BCT_ID_T);
  pos.cmd = param[SIZE_OF_BCT_ID_T];

  ack = BCT_set_position_(&pos);

  return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
}

CCP_EXEC_STS Cmd_BCT_COPY_BCT(const CommonCmdPacket* packet)
{
  const unsigned char* param = CCP_get_param_head(packet);
  uint16_t dst_block, src_block;
  BCT_ACK ack;

  if (CCP_get_param_len(packet) != 2 * SIZE_OF_BCT_ID_T) return CCP_EXEC_ILLEGAL_LENGTH;
  endian_memcpy(&dst_block, param, SIZE_OF_BCT_ID_T);
  endian_memcpy(&src_block, param + SIZE_OF_BCT_ID_T, SIZE_OF_BCT_ID_T);

  ack = BCT_copy_bct(dst_block, src_block);
  return BCT_convert_bct_ack_to_ctcp_exec_sts(ack);
}

CCP_EXEC_STS Cmd_BCT_OVERWRITE_CMD(const CommonCmdPacket* packet)
{
  CMD_CODE cmd_id = (CMD_CODE)CCP_get_param_from_packet(packet, 0, uint16_t);
  cycle_t  ti     = (cycle_t)CCP_get_param_from_packet(packet, 1, uint32_t);
  bct_id_t block  = (bct_id_t)CCP_get_param_from_packet(packet, 2, bct_id_t);
  uint8_t  cmd    = CCP_get_param_from_packet(packet, 3, uint8_t);

  BCT_Pos  pos;
  BCT_CmdData new_bct_cmddata; // FIXME: BCT_CmdData <-> CTCP
  // FIXME: TCP → SpacePacket 大工事が終わったら直す
  //        CCP ならまだしも CSP 依存はやばい
  uint8_t new_cmd_param[BCT_CMD_MAX_LENGTH - SP_PRM_HDR_LEN - CSP_SND_HDR_LEN];   // いったんここにparamをコピーする, FIXME: TCPに依存させないように
  uint16_t real_param_len = CCP_get_param_len(packet);
  uint16_t min_cmd_param_len = CA_get_cmd_param_min_len(Cmd_CODE_BCT_OVERWRITE_CMD);
  uint16_t max_cmd_param_len = min_cmd_param_len + sizeof(new_cmd_param);
  uint16_t cmd_param_len;

  // raw なので引数長チェック
  if (real_param_len < min_cmd_param_len || real_param_len > max_cmd_param_len) return CCP_EXEC_ILLEGAL_LENGTH;

  cmd_param_len = real_param_len - min_cmd_param_len;
  CCP_get_raw_param_from_packet(packet, new_cmd_param, cmd_param_len);

  BCT_make_pos(&pos, block, cmd);
  CCP_form_tlc((CommonCmdPacket*)&new_bct_cmddata, ti, cmd_id, new_cmd_param, cmd_param_len);
  BCT_overwrite_cmd(&pos, (CommonCmdPacket*)&new_bct_cmddata);

  return CCP_EXEC_SUCCESS;
}

// 長さ10のBCにNOPを登録するコマンド. 使用前提が狭すぎるか??
// パス運用時に使用するので, 一応厳密にしておいたほうがいい気もする.
CCP_EXEC_STS Cmd_BCT_FILL_NOP(const CommonCmdPacket* packet)
{
  static CommonCmdPacket temp_packet_;
  cycle_t ti;
  uint8_t num_nop = CCP_get_param_from_packet(packet, 0, uint8_t);

  if (num_nop > 10 || num_nop < 1) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (block_command_table_.pos.cmd + num_nop != 10) return CCP_EXEC_ILLEGAL_CONTEXT;

  for (ti = 11 - (cycle_t)num_nop; ti < 11; ++ti)
  {
    CCP_form_tlc(&temp_packet_, ti, Cmd_CODE_NOP, NULL, 0);
    BCT_register_cmd(&temp_packet_);
  }

  return CCP_EXEC_SUCCESS;
}

#pragma section
