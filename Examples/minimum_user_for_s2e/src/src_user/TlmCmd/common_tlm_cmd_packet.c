#pragma section REPRO
/**
 * @file
 * @brief  CTCP 関連基本関数の定義
 * @note   CTCP: CommonTlmCmdPacket
 * @note   CTP:  CommonTlmPacket
 * @note   CCP:  CommonCmdPacket
 */
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>


int CTCP_is_valid_packet(const CommonTlmCmdPacket* packet)
{
  if (packet == NULL) return 0;
  if ( CTCP_get_packet_len(packet) > (uint16_t)sizeof(CommonTlmCmdPacket) ) return 0;

  return 1;
}

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

uint16_t CTCP_get_packet_len(const CommonTlmCmdPacket* packet)
{
  return (uint16_t)(TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN);
}

// FIXME: 以下4関数について， Space Packet が整備されたら， `if len(ctcp) > CCP_MAX_LEN return NULL;` のようなアサーションをいれる！
const CommonTlmPacket* CTCP_convert_to_ctp(const CommonTlmCmdPacket* ctcp)
{
  if (CTCP_get_packet_type(ctcp) != CTCP_PACKET_TYPE_TLM) return NULL;
  return (const CommonTlmPacket*)ctcp;
}

const CommonTlmCmdPacket* CTCP_convert_from_ctp(const CommonTlmPacket* ctp)
{
  return (const CommonTlmCmdPacket*)ctp;
}

const CommonCmdPacket* CTCP_convert_to_ccp(const CommonTlmCmdPacket* ctcp)
{
  if (CTCP_get_packet_type(ctcp) != CTCP_PACKET_TYPE_CMD) return NULL;
  return (const CommonCmdPacket*)ctcp;
}

const CommonTlmCmdPacket* CTCP_convert_from_ccp(const CommonCmdPacket* ccp)
{
  return (const CommonTlmCmdPacket*)ccp;
}

#pragma section
