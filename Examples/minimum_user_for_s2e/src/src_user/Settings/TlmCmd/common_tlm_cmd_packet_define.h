/**
 * @file
 * @brief  CTCP 型の定義
 */
#ifndef COMMON_TLM_CMD_PACKET_DEFINE_H_
#define COMMON_TLM_CMD_PACKET_DEFINE_H_

#include <src_core/TlmCmd/Ccsds/TCPacket.h>
#include "./Ccsds/apid_define.h"

// CommonTlmCmdPacket として TCP をつかう
// FIXME: SpacePacket に差し替える
typedef TCP CommonTlmCmdPacket;

// テレコマパケットの最大長を定義
#define CTCP_MAX_LEN (TCP_MAX_LEN)

// 自分宛てCMDを示すDestination IDを定義
// FIXME: Space Packet が整備されたら直す
#define CTCP_MY_DST_ID (APID_MOBC_CMD)

#endif
