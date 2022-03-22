#pragma section REPRO
/**
 * @file
* @brief CTCP, CTP, CCP のリストとしてのデータ構造を定義. 片方向リストとして実装されている
 */
#include "packet_list.h"

#include "../System/TimeManager/time_manager.h"
#include "common_tlm_cmd_packet.h"
#include "block_command_executor.h"
#include "block_command_table.h"
#include <src_user/Library/stdint.h>
#include <string.h>
#include "../Applications/timeline_command_dispatcher.h"


/**
 * @brief 未使用 Node を新規に取得
 * @note  取得されたものは使われることを想定（引数として与えられた pl の内部状態は更新される）
 * @param[in,out] pl: PacketList
 * @retval 未使用 Node
 * @retval NULL （PacketListが満杯の時）
 */
static PL_Node* PL_get_free_node_(PacketList* pl);

/**
 * @brief 先頭 Node を落とす
 * @param[in,out] pl: PacketList
 * @retval PL_SUCCESS: 成功
 * @retval PL_LIST_EMPTY: PacketList が空
 */
static PL_ACK PL_drop_head_(PacketList* pl);


PL_ACK PL_initialize(PL_Node* pl_node_stock,
                     void* packet_stock,
                     uint16_t node_num,
                     PL_PACKET_TYPE packet_type,
                     uint16_t packet_size,
                     PacketList* pl)
{
  pl->total_nodes_ = node_num;
  pl->packet_type_ = packet_type;
  pl->packet_size_ = packet_size;
  pl->executed_nodes_ = 0;

  switch (pl->packet_type_)
  {
  case PL_PACKET_TYPE_CTCP:
    if (pl->packet_size_ != (uint16_t)sizeof(CommonTlmCmdPacket)) return PL_PACKET_TYPE_ERR;
    break;
  case PL_PACKET_TYPE_CTP:
    if (pl->packet_size_ != (uint16_t)sizeof(CommonTlmPacket)) return PL_PACKET_TYPE_ERR;
    break;
  case PL_PACKET_TYPE_CCP:
    if (pl->packet_size_ != (uint16_t)sizeof(CommonCmdPacket)) return PL_PACKET_TYPE_ERR;
    break;
  default:
    // その他の場合は， assertion できない
    break;
  }

  pl->pl_node_stock_ = pl_node_stock;
  pl->packet_stock_ = packet_stock;
  PL_clear_list(pl);
  return PL_SUCCESS;
}


void PL_clear_list(PacketList* pl)
{
  uint16_t i;
  const uint16_t packet_size = pl->packet_size_;
  const uint16_t node_num = pl->total_nodes_;
  const uint8_t* packet_stock_head = (const uint8_t*)pl->packet_stock_;

  // PL_Node と packet の関連をつける
  for (i = 0; i < node_num; ++i)
  {
    pl->pl_node_stock_[i].packet = (void*)(packet_stock_head + packet_size * i);
  }

  // PL_node の配列を連結リストとして再定義
  for (i = 0; i < node_num - 1; ++i)
  {
    pl->pl_node_stock_[i].next = &(pl->pl_node_stock_[i + 1]);
  }
  pl->pl_node_stock_[node_num - 1].next = NULL;

  // 最初は全 node 無効化状態
  pl->active_nodes_ = 0;
  pl->inactive_list_head_ = pl->pl_node_stock_;
  pl->active_list_head_ = NULL;
  pl->active_list_tail_ = NULL;
}


uint32_t PL_count_executed_nodes(const PacketList* pl)
{
  // FIXME: HEWでWarningが出てしまう（gccではでない）ので，キャストしている
  return (uint32_t)pl->executed_nodes_;
}


uint16_t PL_count_active_nodes(const PacketList* pl)
{
  // FIXME: HEWでWarningが出てしまう（gccではでない）ので，キャストしている
  return (uint16_t)pl->active_nodes_;
}


uint16_t PL_count_inactive_nodes(const PacketList* pl)
{
  return (uint16_t)(pl->total_nodes_ - pl->active_nodes_);
}


uint16_t PL_get_total_node_num(const PacketList* pl)
{
  // FIXME: HEWでWarningが出てしまう（gccではでない）ので，キャストしている
  return (uint16_t)pl->total_nodes_;
}


uint16_t PL_get_packet_size(const PacketList* pl)
{
  // FIXME: HEWでWarningが出てしまう（gccではでない）ので，キャストしている
  return (uint16_t)pl->packet_size_;
}


PL_PACKET_TYPE PL_get_packet_type(const PacketList* pl)
{
  // FIXME: HEWでWarningが出てしまう（gccではでない）ので，キャストしている
  return (PL_PACKET_TYPE)pl->packet_type_;
}


int PL_is_empty(const PacketList* pl)
{
  return (pl->active_list_head_ == NULL);
}


int PL_is_full(const PacketList* pl)
{
  return (pl->inactive_list_head_ == NULL);
}


const PL_Node* PL_get_head(const PacketList* pli)
{
  return pli->active_list_head_;
}


const PL_Node* PL_get_tail(const PacketList* pli)
{
  return pli->active_list_tail_;
}


const PL_Node* PL_get_next(const PL_Node* node)
{
  return (node != NULL) ? node->next : NULL;
}


PL_ACK PL_push_front(PacketList* pl, const void* packet)
{
  PL_Node* new_pl_node = PL_get_free_node_(pl);
  if (new_pl_node == NULL) return PL_LIST_FULL;

  memcpy(new_pl_node->packet, packet, pl->packet_size_);

  new_pl_node->next = pl->active_list_head_;
  pl->active_list_head_ = new_pl_node;

  if (pl->active_list_tail_ == NULL)
  {
    pl->active_list_tail_ = new_pl_node;
  }

  ++pl->active_nodes_;

  return PL_SUCCESS;
}


PL_ACK PL_push_back(PacketList* pl, const void* packet)
{
  PL_Node* new_pl_node = PL_get_free_node_(pl);
  if (new_pl_node == NULL) return PL_LIST_FULL;

  memcpy(new_pl_node->packet, packet, pl->packet_size_);

  new_pl_node->next = NULL;

  if (pl->active_list_tail_ == NULL)
  {
    pl->active_list_head_ = new_pl_node;
  }
  else
  {
    pl->active_list_tail_->next = new_pl_node;
  }

  pl->active_list_tail_ = new_pl_node;
  ++pl->active_nodes_;

  return PL_SUCCESS;
}


PL_ACK PL_insert_after(PacketList* pl, PL_Node* pos, const void* packet)
{
  PL_Node* new_pl_node;

  if (pos == NULL) return PL_NO_SUCH_NODE;

  if (pos == pl->active_list_tail_) return PL_push_back(pl, packet);

  new_pl_node = PL_get_free_node_(pl);
  if (new_pl_node == NULL) return PL_LIST_FULL;

  memcpy(new_pl_node->packet, packet, pl->packet_size_);

  new_pl_node->next = pos->next;
  pos->next = new_pl_node;

  ++pl->active_nodes_;

  return PL_SUCCESS;
}


PL_ACK PL_drop_executed(PacketList* pl)
{
  PL_ACK ack = PL_drop_head_(pl);
  if (ack == PL_SUCCESS)
  {
    ++(pl->executed_nodes_);
  }

  return ack;
}


PL_ACK PL_drop_node(PacketList* pl, PL_Node* prev, PL_Node* current)
{
  if (current == NULL) return PL_NO_SUCH_NODE;
  if (pl->active_list_head_ == NULL) return PL_LIST_EMPTY;
  if (prev == NULL)
  {
    if (pl->active_list_head_ == current)
    {
      return PL_drop_head_(pl);
    }
    else
    {
      return PL_NO_SUCH_NODE;
    }
  }

  prev->next = current->next;
  if (pl->active_list_tail_ == current)
  {
    pl->active_list_tail_ = prev;
  }

  current->next = pl->inactive_list_head_;
  pl->inactive_list_head_ = current;
  --pl->active_nodes_;

  return PL_SUCCESS;
}


// FIXME: PL にあるのがおかしくて， TL 周りのどこかで検索して， insert を実行すべき
PL_ACK PL_insert_tl_cmd(PacketList* pl, const CommonCmdPacket* packet, cycle_t now)
{
  cycle_t head, tail;
  cycle_t planed = CCP_get_ti(packet);

  if (pl->packet_type_ != PL_PACKET_TYPE_CCP) return PL_PACKET_TYPE_ERR;

  if (now > planed) return PL_TLC_PAST_TIME;
  if (PL_is_full(pl)) return PL_LIST_FULL;
  if (PL_is_empty(pl)) return PL_push_front(pl, packet);

  // 以下，他コマンドが登録されているとき
  head = CCP_get_ti( (const CommonCmdPacket*)(PL_get_head(pl)->packet) );
  tail = CCP_get_ti( (const CommonCmdPacket*)(PL_get_tail(pl)->packet) );

  if (tail < planed) // 他のどれより遅い
  {
    return PL_push_back(pl, packet);
  }
  else if (head > planed) // 他のどれより早い
  {
    return PL_push_front(pl, packet);
  }
  else if (head == planed || tail == planed) // 時刻指定が等しい
  {
    return PL_TLC_ALREADY_EXISTS;
  }
  else // 他コマンドの間に挿入操作が必要（最低でもすでに 2 個登録済みなはず）
  {
    uint16_t i;
    PL_Node* prev = (PL_Node*)PL_get_head(pl); // const_cast
    PL_Node* curr = prev->next;

    // 挿入場所探索
    for (i = 1; i < pl->active_nodes_; ++i)
    {
      cycle_t curr_ti = CCP_get_ti( (const CommonCmdPacket*)(curr->packet) );

      if (curr_ti < planed)
      {
        prev = curr;
        curr = prev->next;
      }
      else if (curr_ti > planed) // 挿入場所発見
      {
        PL_insert_after(pl, prev, packet);
        return PL_SUCCESS;
      }
      else // 既登録コマンドと時刻指定が等しい
      {
        return PL_TLC_ALREADY_EXISTS;
      }
    }
  }

  // NOT REACHED
  return PL_NO_SUCH_NODE;
}


// FIXME: PL にあるのがおかしくて， BC 周りのどこかでやるべき
PL_ACK PL_deploy_block_cmd(PacketList* pl, const bct_id_t block, cycle_t start_at)
{
  uint8_t i;
  uint16_t j;
  int is_cleared = 0; // リスト強制クリアの記録用変数
  uint32_t adj = 0;   // 時刻調整の累積量保存用変数
  uint8_t bc_length;

  if (pl->packet_type_ != PL_PACKET_TYPE_CCP) return PL_PACKET_TYPE_ERR;

  if (block >= BCT_MAX_BLOCKS) return PL_BC_INACTIVE_BLOCK;
  if (!BCE_is_active(block)) return PL_BC_INACTIVE_BLOCK;

  bc_length = BCT_get_bc_length(block);

  // リストにブロック全体を登録する余裕がない場合
  if (PL_count_inactive_nodes(pl) < bc_length)
  {
    // リストをクリアし強制的に空き領域を確保する
    PL_clear_list(pl);
    is_cleared = 1;
  }

  for (i = 0; i < bc_length; ++i)
  {
    static CommonCmdPacket temp_; // サイズが大きいため静的領域に確保
    BCT_Pos pos;
    PL_ACK ack = PL_SUCCESS;

    // コマンドを読みだし、TLCとして実行時刻を設定
    BCT_make_pos(&pos, block, i);
    BCT_load_cmd(&pos, &temp_);
    CCP_set_ti(&temp_, (cycle_t)(start_at + adj + CCP_get_ti(&temp_)));
    CCP_set_exec_type(&temp_, CCP_EXEC_TYPE_TL0); // BLC -> TLC     // FIXME: TaskListもTL012もすべて CCP_EXEC_TYPE_TL0 になってしまうが，わかりにくくない？

    for (j = 0; j <= pl->active_nodes_; ++j)
    {
      // コマンドをTLCに登録を試みる
      ack = TLCD_insert_tl_cmd_strictly(pl, &temp_, start_at);
      if (ack != PL_TLC_ALREADY_EXISTS) break;    // PL_SUCCESS なはず． TODO: 一応 event 発行しておく？

      // 同一時刻で既に登録されていた場合は時刻をずらして再登録
      CCP_set_ti(&temp_, CCP_get_ti(&temp_) + 1);
      ++adj; // 累積調整時間を更新する
    }
    if (ack != PL_SUCCESS) return ack;
  }

  // リストの強制クリアを実施した場合
  if (is_cleared == 1) return PL_BC_LIST_CLEARED;
  // 時刻調整を行った場合
  if (adj != 0) return PL_BC_TIME_ADJUSTED;

  return PL_SUCCESS;
}


PL_ACK PL_check_tl_cmd(const PacketList* pl, cycle_t time)
{
  if (pl->packet_type_ != PL_PACKET_TYPE_CCP) return PL_PACKET_TYPE_ERR;

  if (!PL_is_empty(pl))
  {
    const CommonCmdPacket* packet = (const CommonCmdPacket*)(pl->active_list_head_->packet);
    cycle_t planed = CCP_get_ti(packet);

    if (time == planed) return PL_TLC_ON_TIME;
    if (time > planed)  return PL_TLC_PAST_TIME;
  }

  return PL_TLC_NOT_YET;
}


static PL_Node* PL_get_free_node_(PacketList* pl)
{
  PL_Node* free_node = pl->inactive_list_head_;

  if (free_node == NULL) return NULL;

  pl->inactive_list_head_ = pl->inactive_list_head_->next;

  return free_node;
}


static PL_ACK PL_drop_head_(PacketList* pl)
{
  PL_Node* temp;

  if (pl->active_list_head_ == NULL) return PL_LIST_EMPTY;

  temp = pl->active_list_head_;
  pl->active_list_head_ = temp->next;

  if (pl->active_list_head_ == NULL)
  {
    pl->active_list_tail_ = NULL;
  }

  temp->next = pl->inactive_list_head_;
  pl->inactive_list_head_ = temp;

  --pl->active_nodes_;
  return PL_SUCCESS;
}

#pragma section
