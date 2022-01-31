/**
 * @file
 * @brief  CTCP 型の定義
 */
#ifndef COMMON_TLM_CMD_PACKET_DEFINE_H_
#define COMMON_TLM_CMD_PACKET_DEFINE_H_

#include <src_core/TlmCmd/Ccsds/space_packet_typedef.h>

// CommonTlmCmdPacket として SpacePacket をつかう
typedef SpacePacket CommonTlmCmdPacket;

#define CTCP_MAX_LEN (SP_MAX_LEN)

#endif
