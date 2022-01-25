#pragma section REPRO
/**
 * @file
 * @brief  CTP 関連基本関数の定義
 * @note   CTP:  CommonTlmPacket
 */
#include <src_core/TlmCmd/common_tlm_packet.h>


int CTP_is_valid_packet(const CommonTlmPacket* packet)
{
  if (packet == NULL) return 0;
  if ( CTP_get_packet_len(packet) > (uint16_t)sizeof(CommonTlmPacket) ) return 0;

  return 1;
}

CTP_DEST_FLAG CTP_get_dest_flag(const CommonTlmPacket* packet)
{
  uint8_t category = TCP_TLM_get_category(packet);
  uint8_t mask = 0xe0; // 11100000b

  category &= mask;
  category >>= 5;

  if (category == 0)
  {
    return CTP_DEST_FLAG_RP;
  }
  else
  {
    return (CTP_DEST_FLAG)category;
  }
}

void CTP_set_dest_flag(CommonTlmPacket* packet, CTP_DEST_FLAG flag)
{
  uint8_t category = TCP_TLM_get_category(packet);
  uint8_t mask = 0xe0; // 11100000b

  category &= (uint8_t)(~mask);
  category |= (uint8_t)(flag << 5);

  TCP_TLM_set_category(packet, category);
}

uint16_t CTP_get_packet_len(const CommonTlmPacket* packet)
{
  return (uint16_t)(TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN);
}

uint8_t* CTP_get_user_data_head(CommonTlmPacket* packet)
{
  return TCP_TLM_get_user_data_head(packet);
}

#pragma section
