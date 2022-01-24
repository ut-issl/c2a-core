/**
 * @file
 * @brief CTCP, CTP, CCP のリストとしてのデータ構造を定義. 片方向リストとして実装されている
 */
#ifndef PACKET_LIST_H_
#define PACKET_LIST_H_

#include <stddef.h>

#include "common_tlm_cmd_packet.h"
#include "block_command_table.h"

// !!!!!!!!!!!!!!!!!!!!
// FIXME: CTCP, CTP, CCP 3つのパケットに対応させる大改修が必要．一旦 CTCP にしておくが，一時的
//        https://github.com/ut-issl/c2a-core/pull/210
// !!!!!!!!!!!!!!!!!!!!

/**
 * @struct PL_NodeTag
 * @brief  片方向リストを構成する各ノード
 */
struct PL_NodeTag
{
  CommonTlmCmdPacket packet;
  struct PL_NodeTag* next;
};
typedef struct PL_NodeTag PL_Node;

/**
 * @struct PacketList
 * @brief  パケットリスト本体
 * @note   データ構造としては 片方向リスト
 *         初期化では外部で事前に領域だけ確保しておいたPL_Node配列の先頭ポインタをstockに保存して連結リスト化, inactive_list_headにもコピーする.
           使うときにはinactive_listからPL_Node一つを取り出して, その領域をactive_listに挿入する.
           なおメンバーは全て private
 */
typedef struct
{
  uint16_t total_nodes_;        //!< 全ノード数 (static 確保)
  uint32_t executed_nodes_;     //!< 実行されたノード数
  uint16_t active_nodes_;       //!< 現在片方向リストに入っているノード数

  PL_Node* stock_;              //!< 確保されている領域全体の先頭
  PL_Node* inactive_list_head_; //!< 確保されている領域の内、使っていないものの先頭, stack
  PL_Node* active_list_head_;   //!< 確保されている領域の内、使っているものの先頭
  PL_Node* active_list_tail_;   //!< 確保されている領域の内、使っているものの末端
} PacketList;

/**
 * @enum  PL_ACK
 * @brief PacketList 関連操作のエラーコード
 * @note  uint8_t
 */
typedef enum
{
  PL_SUCCESS,            //!< 成功
  PL_LIST_FULL,          //!< PacketList が一杯 (inactive 無し)
  PL_TLC_PAST_TIME,      //!< 実行時間既に経過
  PL_TLC_ALREADY_EXISTS, //!< 同時刻に既に Node が存在
  PL_TLC_ON_TIME,        //!< 実行時刻丁度
  PL_TLC_NOT_YET,        //!< まだ実行時刻ではない
  PL_BC_INACTIVE_BLOCK,  //!< 無効な BC
  PL_BC_LIST_CLEARED,    //!< PL クリア (初期化, active 全削除) された
  PL_BC_TIME_ADJUSTED,   //!< 同時刻に Node があったため調整せれた
  PL_NO_SUCH_NODE        //!< そんな Node は無い
} PL_ACK;

/**
 * @brief static に確保された PL_Node 配列を受け取りその領域を使用して PL を初期化
 * @param[in] stock: 使用する PL_Node 配列
 * @param[in] size_t: 配列のサイズ
 * @param[out] pli: 初期化する PacketList
 * @return void
 */
void PL_initialize(PL_Node* stock, size_t size, PacketList* pli);

/**
 * @brief PacketList をクリア
 * @param[in] pli: クリアする PacketList
 * @return void
 * @note 全 active Node を削除して 全て inactive の stock にする
 */
void PL_clear_list(PacketList* pli);

/**
 * @brief PacketList で実行されたノード数を返す
 * @param[in] pli: PacketList
 * @return uint32_t: 実行されたノード数
 */
uint32_t PL_count_executed_nodes(const PacketList* pli);

/**
 * @brief PacketList で有効な(実行待ち)ノード数を返す
 * @param[in] pli: PacketList
 * @return uint16_t: 有効な(実行待ち)ノード数
 */
uint16_t PL_count_active_nodes(const PacketList* pli);

/**
 * @brief PacketList で使用されていないノード数を返す
 * @param[in] pli: PacketList
 * @return uint16_t: 使用されていないノード数
 */
uint16_t PL_count_inactive_nodes(const PacketList* pli);

/**
 * @brief PacketList が空かどうか
 * @param[in] pli: PacketList
 * @return int 1: True 0: False
 */
int PL_is_empty(const PacketList* pli);

/**
 * @brief PacketList が一杯かどうか
 * @param[in] pli: PacketList
 * @return int 1: True 0: False
 */
int PL_is_full(const PacketList* pli);

/**
 * @brief PacketList の active 先頭ノードを取得
 * @param[in] pli: PacketList
 * @return const PL_Node*
 */
const PL_Node* PL_get_head(const PacketList* pli);

/**
 * @brief PacketList の active 末端ノードを取得
 * @param[in] pli: PacketList
 * @return const PL_Node*
 */
const PL_Node* PL_get_tail(const PacketList* pli);

/**
 * @brief 次のNode を取得
 * @param[in] node: 現ノード
 * @return const PL_Node*
 */
const PL_Node* PL_get_next(const PL_Node* node);

/**
 * @brief PacketList の先頭に packet を挿入
 * @param[in] pli: PacketList
 * @param[in] packet: 挿入する packet
 * @return PL_ACK
 */
PL_ACK PL_push_front(PacketList* pli, const CommonTlmCmdPacket* packet);

/**
 * @brief PacketList の末尾に packet を挿入
 * @param[in] pli: PacketList
 * @param[in] packet: 挿入する packet
 * @return PL_ACK
 */
PL_ACK PL_push_back(PacketList* pli, const CommonTlmCmdPacket* packet);

/**
 * @brief ある Node の直後に packet を挿入
 * @param[in] pli: PacketList
 * @param[in] pos: 直後に挿入される packet
 * @param[in] packet: 挿入する packet
 * @return PL_ACK
 */
PL_ACK PL_insert_after(PacketList* pli, PL_Node* pos, const CommonTlmCmdPacket* packet);

/**
 * @brief ある Node の直後に packet を挿入
 * @param[in] pli: PacketList
 * @param[in] packet: 挿入する packet
 * @param[in] now: これの指定実行時間
 * @return PL_ACK
 * @note TaskList も TimeLine もこれを使うので now は uint32_t
 */
PL_ACK PL_insert_tl_cmd(PacketList* pli, const CommonTlmCmdPacket* packet, uint32_t now);

/**
 * @brief PacketList 上に BC を展開する
 * @param[in] pli: PacketList
 * @param[in] block: 展開する BC の ID
 * @param[in] start_at: 開始基準時刻
 * @return PL_ACK
 * @note TaskList も TimeLine もこれを使うので start_at は uint32_t
 */
PL_ACK PL_deploy_block_cmd(PacketList* pli, const bct_id_t block, uint32_t start_at);

/**
 * @brief PacketList の先頭と time を比較
 * @param[in] pli: PacketList
 * @param[in] time: 比較する時刻
 * @return PL_ACK
 * @note TaskList も TimeLine もこれを使うので start_at は time
 */
PL_ACK PL_check_tl_cmd(PacketList* pli, uint32_t time);

/**
 * @brief 先頭ノードを落とす
 * @param[in] pli: PacketList
 * @return void
 */
void PL_drop_executed(PacketList* pli);

/**
 * @brief 指定されたノードを落とす
 * @param[in] pli: PacketList
 * @param[in] prev: 落とす直前 Node
 * @param[in] current: 落とす Node
 * @return void
 */
void PL_drop_node(PacketList* pli, PL_Node* prev, PL_Node* current);

#endif
