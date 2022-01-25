/**
 * @file
 * @brief TCP のユーザー設定
 * @note  https://github.com/ut-issl/c2a-core/issues/206 を実装するまでの一時的なつなぎファイル
 */
#ifndef TCPACKET_PARAMS_H_
#define TCPACKET_PARAMS_H_

#undef TCP_MAX_LEN

#define TCP_MAX_LEN  (432u)

// // FIXME: #206 を実装する際は，これは params.h ではなく define.h で定義すべき
// /**
//  * @enum   TCP_APID
//  * @brief  Application Process ID
//  * @note   GSTOS の蓄積データの拡張子につく数字
//  * @note   11bit
//  * @note   CTCP_DEST_ID として typedef する
//  */
// typedef enum
// {
//   TCP_APID_MOBC_CMD = 0x210,         // 01000010000b:
//   TCP_APID_AOBC_CMD = 0x211,         // 01000010001b:
//   TCP_APID_TOBC_CMD = 0x212,         // 01000010010b:
//   TCP_APID_TCAL_TLM = 0x410,         // 10000010000b: APID for TIME CARIBLATION TLM
//   TCP_APID_MIS_TLM  = 0x510,         // 10100010000b: APID for MIS TLM
//   TCP_APID_DUMP_TLM = 0x710,         // 11100010000b: APID for DUMP TLM
//   TCP_APID_FILL_PKT = 0x7ff,         // 11111111111b: APID for FILL PACKET
//   TCP_APID_UNKNOWN
// } TCP_APID;

/**
 * @enum   TCP_CMD_DEST_TYPE
 * @brief  コマンドの解釈の宛先を規定
 * @note   TO_ME: 自分自身 → 自分自身のTLCやBCとして解釈
 * @note   TO_*:  転送先のTLやBCとして解釈（GSから来たコマンドを自身のキューにいれない）
 * @note   0x*0を想定
 * @note   下位4bitは TCP_CMD_EXEC_TYPE と CCP_EXEC_TYPE に
 */
typedef enum
{
  TCP_CMD_DEST_TYPE_TO_ME     = 0x00,
  TCP_CMD_DEST_TYPE_TO_MOBC   = 0x10,
  TCP_CMD_DEST_TYPE_TO_AOBC   = 0x20,
  TCP_CMD_DEST_TYPE_TO_TOBC   = 0x30,
  TCP_CMD_DEST_TYPE_TO_UNKOWN = 0x40
} TCP_CMD_DEST_TYPE;

#endif
