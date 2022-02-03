/**
 * @file
* @brief PacketList の Util．User は基本こちらを使い， PL を直接使わない．
 */
#ifndef PACKET_LIST_UTIL_H_
#define PACKET_LIST_UTIL_H_

#include "packet_list.h"
#include "common_tlm_cmd_packet.h"

/**
 * @brief static に確保された PL_Node 配列と CTCP 配列を受け取りその領域を使用して PL を初期化
 * @param[in] pl_node_stock: 使用する PL_Node 配列
 * @param[in] packet_stock: PL_Node として使用する CTCP の配列（メモリ確保用）
 * @param[in] node_num: PL_Node の数
 * @param[out] pl: 初期化する PacketList
 * @retval PL_SUCCESS: 成功
 * @retval PL_PACKET_TYPE_ERR: 型関連エラー
 */
PL_ACK PL_initialize_with_ctcp(PL_Node* pl_node_stock,
                               CommonTlmCmdPacket* packet_stock,
                               uint16_t node_num,
                               PacketList* pl);

/**
 * @brief static に確保された PL_Node 配列と CTP 配列を受け取りその領域を使用して PL を初期化
 * @param[in] pl_node_stock: 使用する PL_Node 配列
 * @param[in] packet_stock: PL_Node として使用する CTP の配列（メモリ確保用）
 * @param[in] node_num: PL_Node の数
 * @param[out] pl: 初期化する PacketList
 * @retval PL_SUCCESS: 成功
 * @retval PL_PACKET_TYPE_ERR: 型関連エラー
 */
PL_ACK PL_initialize_with_ctp(PL_Node* pl_node_stock,
                              CommonTlmPacket* packet_stock,
                              uint16_t node_num,
                              PacketList* pl);

/**
 * @brief static に確保された PL_Node 配列と CCP 配列を受け取りその領域を使用して PL を初期化
 * @param[in] pl_node_stock: 使用する PL_Node 配列
 * @param[in] packet_stock: PL_Node として使用する CCP の配列（メモリ確保用）
 * @param[in] node_num: PL_Node の数
 * @param[out] pl: 初期化する PacketList
 * @retval PL_SUCCESS: 成功
 * @retval PL_PACKET_TYPE_ERR: 型関連エラー
 */
PL_ACK PL_initialize_with_ccp(PL_Node* pl_node_stock,
                              CommonCmdPacket* packet_stock,
                              uint16_t node_num,
                              PacketList* pl);

#endif
