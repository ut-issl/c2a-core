/**
 * @file
 * @brief CTCP, CTP, CCP のリストとしてのデータ構造を定義. 片方向リストとして実装されている
 */
#ifndef PACKET_LIST_H_
#define PACKET_LIST_H_

#include <stddef.h>

#include "common_cmd_packet.h"
#include "block_command_table.h"


// 自己参照用
typedef struct PL_Node PL_Node;


/**
 * @enum  PL_PACKET_TYPE
 * @brief PacketList に格納される packet の型情報
 * @note  uint8_t を想定
 * @note  基本的に PacketList は任意の packet 型を格納できるようにしているが，特定のメソッドが特定の型を要求するため，作っている
 *        以下で定義されている型については，それを指定して PL_initialize を呼び出すことを推奨する（内部でアサーションなどもかかるので）
 */
typedef enum
{
  PL_PACKET_TYPE_CTCP,    //!< CommonTlmCmdPacket
  PL_PACKET_TYPE_CTP,     //!< CommonTlmPacket
  PL_PACKET_TYPE_CCP,     //!< CommonCmdPacket
  PL_PACKET_TYPE_OTHER    //!< その他のパケット
} PL_PACKET_TYPE;

/**
 * @enum  PL_ACK
 * @brief PacketList 関連操作のエラーコード
 * @note  uint8_t を想定
 */
typedef enum
{
  PL_SUCCESS,            //!< 成功
  PL_LIST_FULL,          //!< PacketList が満杯 (inactive 無し)
  PL_LIST_EMPTY,         //!< PacketList が空 (active 無し)
  PL_PACKET_TYPE_ERR,    //!< PL_PACKET_TYPE 関連エラー
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
 * @struct PL_Node
 * @brief  片方向リストを構成する各 Node
 */
struct PL_Node
{
  void* packet;           //!< 片方向リストに格納される packet． どのような型でも良いように， void．（基本的には CTCP, CTP, CCP を想定）
  struct PL_Node* next;   //!< 次の Node （片方向リスト）
};

/**
 * @struct PacketList
 * @brief  パケットリスト本体
 * @note   データ構造としては 片方向リスト
 *         初期化では外部で事前に領域だけ確保しておいた PL_Node 配列の先頭ポインタを stock に保存して連結リスト化, inactive_list_head にもコピーする.
 *         使うときには inactive_list から PL_Node 一つを取り出して, その領域を active_list に挿入する.
 *         なおメンバーは全て private
 */
typedef struct
{
  uint16_t total_nodes_;        //!< 全ノード数 (static 確保)
  uint32_t executed_nodes_;     //!< 実行（コマンド） or 配送（テレメ）されたノード数     // FIXME: TODO: テレメのときもカウントアップしてるか確認
  uint16_t active_nodes_;       //!< 現在片方向リストに入っているノード数
  uint16_t packet_size_;        //!< PL_Node->packet の型サイズ
  PL_PACKET_TYPE packet_type_;  //!< 保持する packet の型情報．PL_PACKET_TYPE を参照

  PL_Node* pl_node_stock_;      //!< 確保されている PL_Node 領域（配列）全体の先頭
  void*    packet_stock_;       //!< 確保されている packet 領域（配列）全体の先頭
  PL_Node* inactive_list_head_; //!< 確保されている領域の内，使っていないものの先頭
  PL_Node* active_list_head_;   //!< 確保されている領域の内，使っているものの先頭
  PL_Node* active_list_tail_;   //!< 確保されている領域の内，使っているものの末端
} PacketList;


/**
 * @brief static に確保された PL_Node 配列と packet 配列を受け取りその領域を使用して PL を初期化
 * @param[in] pl_node_stock: 使用する PL_Node 配列
 * @param[in] packet_stock: PL_Node として使用する packet の配列（メモリ確保用）
 * @param[in] node_num: PL_Node の数
 * @param[in] packet_type: 保持する packet の型情報．PL_PACKET_TYPE を参照
 * @param[in] packet_size: 使用する packet の型サイズ
 * @param[in,out] pl: 初期化する PacketList
 * @retval PL_SUCCESS: 成功
 * @retval PL_PACKET_TYPE_ERR: 型関連エラー
 */
PL_ACK PL_initialize(PL_Node* pl_node_stock,
                     void* packet_stock,
                     uint16_t node_num,
                     PL_PACKET_TYPE packet_type,
                     uint16_t packet_size,
                     PacketList* pl);

/**
 * @brief PacketList をクリア
 * @param[in,out] pl: クリアする PacketList
 * @return void
 * @note 全 active Node を削除して 全て inactive の stock にする
 */
void PL_clear_list(PacketList* pl);

/**
 * @brief PacketList で実行された Node 数を返す
 * @param[in] pl: PacketList
 * @return 実行された Node 数
 */
uint32_t PL_count_executed_nodes(const PacketList* pl);

/**
 * @brief PacketList で有効な（実行待ち） Node 数を返す
 * @param[in] pl: PacketList
 * @return 有効な（実行待ち） Node 数
 */
uint16_t PL_count_active_nodes(const PacketList* pl);

/**
 * @brief PacketList で使用されていない Node 数を返す
 * @param[in] pl: PacketList
 * @return 使用されていない Node 数
 */
uint16_t PL_count_inactive_nodes(const PacketList* pl);

/**
 * @brief PacketList の全 Node 数を返す
 * @param[in] pl: PacketList
 * @return 全 Node 数
 */
uint16_t PL_get_total_node_num(const PacketList* pl);

/**
 * @brief PacketList で使用される packet の型サイズを返す
 * @param[in] pl: PacketList
 * @return packet の型サイズ
 */
uint16_t PL_get_packet_size(const PacketList* pl);

/**
 * @brief PacketList で使用される packet の型情報 PL_PACKET_TYPE を返す
 * @param[in] pl: PacketList
 * @return packet の型情報
 */
PL_PACKET_TYPE PL_get_packet_type(const PacketList* pl);

/**
 * @brief PacketList が空かどうか
 * @param[in] pl: PacketList
 * @retval 1: True
 * @retval 0: False
 */
int PL_is_empty(const PacketList* pl);

/**
 * @brief PacketList が満杯かどうか
 * @param[in] pl: PacketList
 * @retval 1: True
 * @retval 0: False
 */
int PL_is_full(const PacketList* pl);

/**
 * @brief PacketList の active な先頭 Node を取得
 * @param[in] pl: PacketList
 * @return active な先頭 Node
 */
const PL_Node* PL_get_head(const PacketList* pl);

/**
 * @brief PacketList の active な末端 Node を取得
 * @param[in] pl: PacketList
 * @return active な末端 Node
 */
const PL_Node* PL_get_tail(const PacketList* pl);

/**
 * @brief 現 Node から次の Node を取得
 * @param[in] node: 現 Node
 * @retval 次の Node
 * @retval NULL （現 Node が末尾の場合）
 */
const PL_Node* PL_get_next(const PL_Node* node);

/**
 * @brief PacketList の先頭に packet を挿入
 * @param[in] pl: PacketList
 * @param[in] packet: 挿入する packet
 * @retval PL_SUCCESS: 成功
 * @retval PL_LIST_FULL: PacketList が満杯
 */
PL_ACK PL_push_front(PacketList* pl, const void* packet);

/**
 * @brief PacketList の末尾に packet を挿入
 * @param[in] pl: PacketList
 * @param[in] packet: 挿入する packet
 * @retval PL_SUCCESS: 成功
 * @retval PL_LIST_FULL: PacketList が満杯
 */
PL_ACK PL_push_back(PacketList* pl, const void* packet);

/**
 * @brief ある Node の直後に packet を挿入
 * @param[in] pl: PacketList
 * @param[in] pos: 直後に挿入される packet
 * @param[in] packet: 挿入する packet
 * @retval PL_SUCCESS: 成功
 * @retval PL_LIST_FULL: PacketList が満杯
 * @retval PL_NO_SUCH_NODE: pos で指定したような Node は存在しない
 */
PL_ACK PL_insert_after(PacketList* pl, PL_Node* pos, const void* packet);

/**
 * @brief 先頭 Node を落とす
 * @param[in] pl: PacketList
 * @retval PL_SUCCESS: 成功
 * @retval PL_LIST_EMPTY: PacketList が空
 */
PL_ACK PL_drop_executed(PacketList* pl);

/**
 * @brief 指定された Node を落とす
 * @param[in] pl: PacketList
 * @param[in] prev: 落とす直前 Node． current が先頭の場合は NULL
 * @param[in] current: 落とす Node
 * @retval PL_SUCCESS: 成功
 * @retval PL_LIST_EMPTY: PacketList が空
 * @retval PL_NO_SUCH_NODE: 引数で指定したような Node は存在しない
 */
PL_ACK PL_drop_node(PacketList* pl, PL_Node* prev, PL_Node* current);


// 以下，特定の packet を想定した PacketList の関数

/**
 * @brief CCP が時系列に並ぶように CCP を挿入する
 * @note TimeLine だけでなく TaskList もこれを使い，その場合， now は step_t になることに注意
 * @param[in,out] pl: CCP を挿入する PacketList
 * @param[in] packet: 挿入する CCP
 * @param[in] now: 基準時刻 (TimeLine なら現在時刻， TaskList なら現在 step)
 * @retval PL_SUCCESS: 成功
 * @retval PL_LIST_FULL: PacketList が満杯
 * @retval PL_TLC_PAST_TIME: 実行時間がすでに過ぎている
 * @retval PL_TLC_ALREADY_EXISTS: 指定した実行時間にはすでにコマンドが登録されている
 * @retval PL_NO_SUCH_NODE: 何かがおかしい
 * @retval PL_PACKET_TYPE_ERR: 指定した PacketList の packet が CCP ではない
 */
PL_ACK PL_insert_tl_cmd(PacketList* pl, const CommonCmdPacket* packet, cycle_t now);

/**
 * @brief PacketList 上に BC を展開する
 * @note TimeLine だけでなく TaskList もこれを使い，その場合， start_at は step_t になることに注意
 * @param[in,out] pl: BC を展開する PacketList
 * @param[in] block: 展開する BC の ID
 * @param[in] start_at: 開始基準時刻
 * @retval PL_SUCCESS: 成功
 * @retval PL_BC_INACTIVE_BLOCK: block が不正
 * @retval PL_BC_LIST_CLEARED: PacketList の空き容量が不足していたため，強制的に clear した場合
 * @retval PL_BC_TIME_ADJUSTED: 時間調整が施された場合
 * @retval PL_PACKET_TYPE_ERR: 指定した PacketList の packet が CCP ではない
 */
PL_ACK PL_deploy_block_cmd(PacketList* pl, const bct_id_t block, cycle_t start_at);

/**
 * @brief PacketList の先頭と time を比較
 * @note TimeLine だけでなく TaskList もこれを使い，その場合， time は step_t になることに注意
 * @param[in] pl: PacketList
 * @param[in] time: 比較する時刻
 * @retval PL_TLC_ON_TIME: ちょうど
 * @retval PL_TLC_PAST_TIME: 過去
 * @retval PL_TLC_NOT_YET: まだ指定時刻になっていない or PacketList が空
 * @retval PL_PACKET_TYPE_ERR: 指定した PacketList の packet が CCP ではない
 */
PL_ACK PL_check_tl_cmd(const PacketList* pl, cycle_t time);

#endif
