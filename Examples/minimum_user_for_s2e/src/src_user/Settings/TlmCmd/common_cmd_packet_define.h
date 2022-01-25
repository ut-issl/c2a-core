/**
 * @file
 * @brief  CCP 型の定義
 */
#ifndef COMMON_CMD_PACKET_DEFINE_H_
#define COMMON_CMD_PACKET_DEFINE_H_

#include <src_core/TlmCmd/Ccsds/TCPacket.h>

// CommonCmdPacket として TCP をつかう
// FIXME: CmdSpacePacket に差し替える
typedef TCP CommonCmdPacket;

// 2021-01-22 追加
// DEST_TYPE を定義
typedef TCP_CMD_DEST_TYPE CCP_DEST_TYPE;

#endif
