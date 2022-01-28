#pragma section REPRO
/**
 * @file
 * @brief  CTCP 関連基本関数の定義
 * @note   CTCP: CommonTlmCmdPacket
 * @note   CTP:  CommonTlmPacket
 * @note   CCP:  CommonCmdPacket
 */
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/TlmCmd/Ccsds/space_packet.h>
#include <string.h>


int CTCP_is_valid_packet(const CommonTlmCmdPacket* packet)
{
  if (packet == NULL) return 0;
  if ( CTCP_get_packet_len(packet) > CTCP_MAX_LEN ) return 0;

  return 1;
}

CTCP_PACKET_TYPE CTCP_get_packet_type(const CommonTlmCmdPacket* packet)
{
  switch (SP_get_type(packet))
  {
  case SP_TYPE_TLM:
    return CTCP_PACKET_TYPE_TLM;

  case SP_TYPE_CMD:
    return CTCP_PACKET_TYPE_CMD;

  default:
    return CTCP_PACKET_TYPE_UNKNOWN;
  }
}

uint16_t CTCP_get_packet_len(const CommonTlmCmdPacket* packet)
{
  return SP_get_packet_len(packet);
}

APID CTCP_get_apid(const CommonTlmCmdPacket* packet)
{
  return SP_get_apid(packet);
}

void CTCP_set_apid(CommonTlmCmdPacket* packet, APID apid)
{
  SP_set_apid(packet, apid);
}

void CTCP_copy_packet(CommonTlmCmdPacket* dest, const CommonTlmCmdPacket* src)
{
  SP_copy_packet(dest, src);
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
