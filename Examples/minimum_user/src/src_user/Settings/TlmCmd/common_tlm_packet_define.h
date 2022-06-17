/**
 * @file
 * @brief  CTP 型の定義
 */
#ifndef COMMON_TLM_PACKET_DEFINE_H_
#define COMMON_TLM_PACKET_DEFINE_H_

#include <src_core/TlmCmd/Ccsds/space_packet_typedef.h>
#include "./Ccsds/apid_define.h"

// CommonTlmPacket として TlmSpacePacket をつかう
typedef TlmSpacePacket CommonTlmPacket;

#define CTP_MAX_LEN (TSP_MAX_LEN)

// 自分で生成される TLM を示す AIPD を定義
// FIXME: Space Packet が整備されたら直す
#define CTP_APID_FROM_ME (APID_MOBC_TLM)

#endif
