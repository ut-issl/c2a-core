/**
 * @file
 * @brief  CTCP 関連基本関数の宣言
 * @note   CTCP: CommonTlmCmdPacket
 * @note   CTP:  CommonTlmPacket
 * @note   CCP:  CommonCmdPacket
 */
#ifndef COMMON_TLM_CMD_PACKET_H_
#define COMMON_TLM_CMD_PACKET_H_

#include <src_user/Settings/TlmCmd/common_tlm_cmd_packet_define.h>
#include <src_user/Settings/TlmCmd/common_tlm_cmd_packet_params.h>
#include "./common_tlm_packet.h"
#include "./common_cmd_packet.h"

// ここで APID を定義する
/* 例
// FIXME: CCSDS JAXA 標準になおす
typedef enum
{
  APID_MOBC_CMD = 0x210,         // 01000010000b:
  APID_AOBC_CMD = 0x211,         // 01000010001b:
  APID_TCAL_TLM = 0x410,         // 10000010000b: APID for TIME CARIBLATION TLM
  APID_MIS_TLM  = 0x510,         // 10100010000b: APID for MIS TLM
  APID_DUMP_TLM = 0x710,         // 11100010000b: APID for DUMP TLM
  APID_FILL_PKT = 0x7ff,         // 11111111111b: APID for FILL PACKET
  APID_UNKNOWN
} APID;
*/
#include <src_user/Settings/TlmCmd/Ccsds/apid_define.h>


/**
 * @enum   CTCP_PACKET_TYPE
 * @brief  tlm (CTP) or cmd (CCP) を規定
 * @note   uint8_tを想定
 */
typedef enum
{
  CTCP_PACKET_TYPE_CMD,
  CTCP_PACKET_TYPE_TLM,
  CTCP_PACKET_TYPE_UNKNOWN
} CTCP_PACKET_TYPE;


/**
 * @brief  tlm か cmd かを判断
 * @param  packet: CTCP
 * @return CTCP_PACKET_TYPE
 */
CTCP_PACKET_TYPE CTCP_get_packet_type(const CommonTlmCmdPacket* packet);

/**
 * @brief  パケット長を取得
 * @param  packet: CTCP
 * @return packet len
 */
uint16_t CTCP_get_packet_len(const CommonTlmCmdPacket* packet);

/**
 * @brief  APID を取得
 * @param  packet: CTCP
 * @return APID
 */
APID CTCP_get_apid(const CommonTlmCmdPacket* packet);

/**
 * @brief  APID を設定
 * @param[in,out] packet: CTCP
 * @param[in]     apid: APID
 * @return void
 */
void CTCP_set_apid(CommonTlmCmdPacket* packet, APID apid);

/**
 * @brief  CommonTlmCmdPacket をコピー
 * @note   user実装次第だが，高速化のためすすべてではなく必要最低限しかコピーしない場合もある
 * @param[in,out] dest: コピー先 CTCP
 * @param[in]     src:  コピー元 CTCP
 * @return void
 */
void CTCP_copy_packet(CommonTlmCmdPacket* dest, const CommonTlmCmdPacket* src);

#endif
