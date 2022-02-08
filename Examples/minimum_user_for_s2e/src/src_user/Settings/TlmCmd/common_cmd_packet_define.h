/**
 * @file
 * @brief  CCP 型の定義
 */
#ifndef COMMON_CMD_PACKET_DEFINE_H_
#define COMMON_CMD_PACKET_DEFINE_H_

#include <src_core/TlmCmd/Ccsds/space_packet_typedef.h>

// CommonCmdPacket として CmdSpacePacket をつかう
typedef CmdSpacePacket CommonCmdPacket;

#define CCP_MAX_LEN (CSP_MAX_LEN)

// 自分宛て CMD を示す AIPD を定義
// FIXME: Space Packet が整備されたら直す
#define CCP_APID_TO_ME (APID_MOBC_CMD)

/**
 * @enum   CCP_DEST_TYPE
 * @brief  コマンドの解釈の宛先を規定
 * @note   TO_ME: 自分自身 → 自分自身の TLC や BC として解釈．コマンド実行時に必要に応じて別 OBC へ配送 （この定義は C2A Core で使うため，どんな C2A でも必須）
 * @note   TO_*:  転送先の TL や BC として解釈 （直接指定 OBC へ配送． GS から来たコマンドを自身のキューにいれない）
 * @note   4bit を想定
 */
typedef enum
{
  CCP_DEST_TYPE_TO_ME     = 0,
  CCP_DEST_TYPE_TO_MOBC   = 1,
  CCP_DEST_TYPE_TO_AOBC   = 2,
  CCP_DEST_TYPE_TO_TOBC   = 3,
  CCP_DEST_TYPE_TO_UNKOWN = 4
} CCP_DEST_TYPE;

#endif
