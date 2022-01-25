#pragma section REPRO
/**
 * @file
* @brief PacketList の Util．User は基本こちらを使い， PL を直接使わない．
 */
#include "packet_list_util.h"

PL_ACK PL_initialize_with_ctcp(PL_Node* pl_node_stock,
                               CommonTlmCmdPacket* packet_stock,
                               uint16_t node_num,
                               PacketList* pl)
{
  return PL_initialize(pl_node_stock,
                       packet_stock,
                       node_num,
                       PL_PACKET_TYPE_CTCP,
                       (uint16_t)sizeof(CommonTlmCmdPacket),
                       pl);
}

PL_ACK PL_initialize_with_ctp(PL_Node* pl_node_stock,
                              CommonTlmPacket* packet_stock,
                              uint16_t node_num,
                              PacketList* pl)
{
  return PL_initialize(pl_node_stock,
                       packet_stock,
                       node_num,
                       PL_PACKET_TYPE_CTP,
                       (uint16_t)sizeof(CommonTlmPacket),
                       pl);
}

PL_ACK PL_initialize_with_ccp(PL_Node* pl_node_stock,
                              CommonCmdPacket* packet_stock,
                              uint16_t node_num,
                              PacketList* pl)
{
  return PL_initialize(pl_node_stock,
                       packet_stock,
                       node_num,
                       PL_PACKET_TYPE_CCP,
                       (uint16_t)sizeof(CommonCmdPacket),
                       pl);
}

#pragma section
