/**
 * @file
 * @brief  CTCP型の定義
 */
#ifndef COMMON_TLM_CMD_PACKET_DEFINE_H_
#define COMMON_TLM_CMD_PACKET_DEFINE_H_

#include "../../TlmCmd/Ccsds/TCPacket.h"

// 使用するパケット構造体をCTCP型として定義する
typedef TCP CTCP;

// 宛先IDを定義
typedef TCP_APID CTCP_DEST_ID;

// 2021-01-22 追加
// DEST_TYPE を定義
typedef TCP_CMD_DEST_TYPE CCP_DEST_TYPE;

// テレコマパケットの最大長を定義
#define CTCP_MAX_LEN (TCP_MAX_LEN)

// 自分宛てCMDを示すDestination IDを定義
#define CTCP_MY_DST_ID (TCP_APID_MOBC_CMD)

#endif
