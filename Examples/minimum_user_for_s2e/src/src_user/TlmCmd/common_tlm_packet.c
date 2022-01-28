#pragma section REPRO
/**
 * @file
 * @brief  CTP 関連基本関数の定義
 * @note   CTP:  CommonTlmPacket
 */
#include <src_core/TlmCmd/common_tlm_packet.h>
#include <src_core/TlmCmd/Ccsds/tlm_space_packet.h>
#include <string.h>


int CTP_is_valid_packet(const CommonTlmPacket* packet)
{
  if (packet == NULL) return 0;
  if ( CTP_get_packet_len(packet) > CTP_MAX_LEN ) return 0;

  return 1;
}

ctp_dest_flags_t CTP_get_dest_flags(const CommonTlmPacket* packet)
{
  return TSP_get_dest_flgas(packet);
}

void CTP_set_dest_flags(CommonTlmPacket* packet, ctp_dest_flags_t flags)
{
  TSP_set_dest_flgas(packet, flags);
}

uint16_t CTP_get_packet_len(const CommonTlmPacket* packet)
{
  return TSP_get_packet_len(packet);
}

uint8_t* CTP_get_user_data_head(CommonTlmPacket* packet)
{
  return TSP_get_user_data_head(packet);
}

void CTP_copy_packet(CommonTlmPacket* dest, const CommonTlmPacket* src)
{
  TSP_copy_packet(dest, src);
}

#pragma section
