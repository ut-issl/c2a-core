/**
 * @file
 * @brief  CTP 型の定義
 */
#ifndef COMMON_TLM_PACKET_DEFINE_H_
#define COMMON_TLM_PACKET_DEFINE_H_

#include <src_core/TlmCmd/Ccsds/space_packet_typedef.h>

// CommonTlmPacket として TlmSpacePacket をつかう
typedef TlmSpacePacket CommonTlmPacket;

#define CTP_MAX_LEN (TSP_MAX_LEN)

#endif
