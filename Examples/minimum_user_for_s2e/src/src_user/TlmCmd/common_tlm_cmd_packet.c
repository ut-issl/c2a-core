#pragma section REPRO
/**
 * @file
 * @brief  CTCP 関連基本関数の定義
 * @note   CTCP: CommonTlmCmdPacket
 * @note   CTP:  CommonTlmPacket
 * @note   CCP:  CommonCmdPacket
 */
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>


CTCP_PACKET_TYPE CTCP_get_packet_type(const CommonTlmCmdPacket* packet)
{
  switch (TCP_get_type(packet))
  {
  case TCP_TYPE_TLM:
    return CTCP_PACKET_TYPE_TLM;

  case TCP_TYPE_CMD:
    return CTCP_PACKET_TYPE_CMD;

  default:
    return CTCP_PACKET_TYPE_UNKNOWN;
  }
}


void CTCP_copy_packet(CommonTlmCmdPacket* dest, const CommonTlmCmdPacket* src)
{
  TCP_copy_packet(dest, src);
}

// FIXME: Space Packet が整備されたら実装する
// APID CTCP_get_apid(const CommonTlmCmdPacket* packet)
// {
// }

// void CTCP_set_apid(CommonTlmCmdPacket* packet, APID apid)
// {
// }


// uint16_t CTCP_get_packet_len(const CommonTlmCmdPacket* packet)
// {
// }

#pragma section
