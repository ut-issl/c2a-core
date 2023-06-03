/**
 * @file
 * @brief  CCP 型の定義
 */
#ifndef COMMON_CMD_PACKET_DEFINE_H_
#define COMMON_CMD_PACKET_DEFINE_H_

#include <src_core/TlmCmd/Ccsds/space_packet_typedef.h>
#include "./Ccsds/apid_define.h"

// CommonCmdPacket として CmdSpacePacket をつかう
typedef CmdSpacePacket CommonCmdPacket;

#define CCP_MAX_LEN (CSP_MAX_LEN)

// 自分宛て CMD を示す AIPD を定義
// FIXME: Space Packet が整備されたら直す
#define CCP_APID_TO_ME (APID_AOBC_CMD)

/**
 * @enum   CCP_DEST_TYPE
 * @brief  コマンドの解釈の宛先を規定
 * @note   詳細は https://github.com/ut-issl/c2a-core/blob/develop/Docs/Core/communication.md を参照
 * @note   4bit を想定
 */
typedef enum
{
  CCP_DEST_TYPE_TO_ME     = 0x0,
  CCP_DEST_TYPE_TO_MOBC   = 0x1,    // CCP_DEST_TYPE_TO_APID の追加に伴い deprecated
  CCP_DEST_TYPE_TO_AOBC   = 0x2,    // CCP_DEST_TYPE_TO_APID の追加に伴い deprecated
  CCP_DEST_TYPE_TO_TOBC   = 0x3,    // CCP_DEST_TYPE_TO_APID の追加に伴い deprecated
  CCP_DEST_TYPE_TO_UNKOWN = 0xe,
  CCP_DEST_TYPE_TO_APID   = 0xf
} CCP_DEST_TYPE;

/**
 * @brief  バイト列から CCP_DEST_TYPE を取得
 * @param  dest_type: CCP_DEST_TYPE 候補の uint8_t
 * @note   不正な入力のときは CCP_DEST_TYPE_TO_UNKOWN を返す
 * @return CCP_DEST_TYPE
 */
CCP_DEST_TYPE CCP_get_dest_type_from_uint8(uint8_t dest_type);

#endif
