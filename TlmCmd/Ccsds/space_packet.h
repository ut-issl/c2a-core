/**
 * @file
 * @brief CCSDS で規定される Space Packet の実装
 * @note  資料: https://sma.jaxa.jp/TechDoc/Docs/JAXA-JERG-2-403.pdf
 * @note  C2A においては， CommonHogePacket の実体として，以下のように Space Packet を標準とする
 *        CommonTlmCmdPacket -> SpacePacket
 *        CommonTlmPacket    -> TlmSpacePacket
 *        CommonCmdPacket    -> CmdSpacePacket
 * @note  Packet Secondary Header は C2A 独自定義． いくつかのバージョンを用意している
 * @note  /Docs/Core/communication.md などを参照
 * @note  packet 構造
 *        |---------+-------+-------+------------------|
 *        | Pos     | Pos   | size  | name             |
 *        | [octet] | [bit] | [bit] |                  |
 *        |---------+-------+-------+------------------|
 *        | === Primary Header ======================= |
 *        |---------+-------+-------+------------------|
 *        |       0 |     0 |     3 | Version No       |
 *        |       0 |     3 |     1 | Packet Type      |
 *        |       0 |     4 |     1 | Sec. HDR Flag    |
 *        |       0 |     5 |    11 | APID             |
 *        |       2 |     0 |     2 | Sequence Flags   |
 *        |       2 |     2 |    14 | Sequence Count   |
 *        |       4 |     0 |    16 | Packet Data Len  |
 *        |---------+-------+-------+------------------|
 *        | === Secondary Header ===================== |
 *        |---------+-------+-------+------------------|
 *        |       6 |     0 |     * | User Defined     |
 *        |---------+-------+-------+------------------|
 *        | === User Data Field ====================== |
 *        |---------+-------+-------+------------------|
 *        |       * |     0 |     * | User Defined     |
 *        |---------+-------+-------+------------------|
 */
#ifndef SPACE_PACKET_H_
#define SPACE_PACKET_H_

#include "tlm_space_packet.h"
#include "cmd_space_packet.h"

#if TSP_MAX_LEN > CSP_MAX_LEN
#define SP_MAX_LEN    TSP_MAX_LEN
#else
#define SP_MAX_LEN    CSP_MAX_LEN
#endif

#define SP_PRM_HDR_LEN   (6)   //!< Packet Primary Header 長


/**
 * @enum   SP_VER
 * @brief  Space Packet Version Number
 * @note   3 bit
 */
typedef enum
{
  SP_VER_1 = 0,     //!< 000b: Version-1
  SP_VER_UNKNOWN
} SP_VER;

/**
 * @enum   SP_TYPE
 * @brief  Space Packet Type
 * @note   tlm or cmd を規定
 * @note   0/1の 1 bit
 */
typedef enum
{
  SP_TYPE_TLM = 0,    //!< 0b: TELEMETRY
  SP_TYPE_CMD = 1     //!< 1b: COMMAND
} SP_TYPE;

/**
 * @enum   SP_2ND_HDR_FLAG
 * @brief  Space Packet Secandary Header Flag
 * @note   Secondary Header の有無
 * @note   0/1 の 1 bit
 */
typedef enum
{
  SP_2ND_HDR_FLAG_ABSENT  = 0,     //!< 0b: Secondary Header Absent
  SP_2ND_HDR_FLAG_PRESENT = 1      //!< 1b: Secondary Header Present
} SP_2ND_HDR_FLAG;

/**
 * @enum   SP_SEQ_FLAG
 * @brief  Space Packet Sequence Flag
 * @note   2 bit
 */
typedef enum
{
  SP_SEQ_FLAG_CONT   = 0,   //!< 00b: Continuation component of higher data structure
  SP_SEQ_FLAG_FIRST  = 1,   //!< 01b: First component of higher data structure
  SP_SEQ_FLAG_LAST   = 2,   //!< 10b: Last component of higher data structure
  SP_SEQ_FLAG_SINGLE = 3    //!< 11b: Standalone packet
} SP_SEQ_FLAG;

/**
 * @struct SpacePacket
 * @brief  Space Packet
 */
typedef struct
{
  uint8_t packet[SP_MAX_LEN];
} SpacePacket;

/**
 * @struct SP_ParamExtractionInfo
 * @brief  packet からデータを抜き取るときのパラメタ
 */
typedef struct
{
  uint16_t pos;       //!< 位置
  uint8_t  mask;      //!< 1 byte 目にあてるマスク
  uint8_t  offset;    //!< bit offset （bit が左シフトされて格納されている場合）
  uint8_t  byte_len;  //!< パケット octet 列の何 byte にわたるか．最大 8 (64bit)
} SP_ParamExtractionInfo;


// ******************************
//  Primary Header getter/setter
// ******************************

/**
 * @brief  Version No を取得
 * @param  sp: SpacePacket
 * @return SP_VER
 */
SP_VER SP_get_ver(const SpacePacket* sp);

/**
 * @brief  Version No を設定
 * @param[in,out] sp:  SpacePacket
 * @param[in]     ver: SP_VER
 * @return void
 */
void SP_set_ver(SpacePacket* sp, SP_VER ver);

/**
 * @brief  Packet Type を取得
 * @param  sp: SpacePacket
 * @return SP_TYPE
 */
SP_TYPE SP_get_type(const SpacePacket* sp);

/**
 * @brief  Packet Type を設定
 * @param[in,out] sp:   SpacePacket
 * @param[in]     type: SP_TYPE
 * @return void
 */
void SP_set_type(SpacePacket* sp, SP_TYPE type);

/**
 * @brief  Secondary Header Flag を取得
 * @param  sp: SpacePacket
 * @return SP_2ND_HDR_FLAG
 */
SP_2ND_HDR_FLAG SP_get_2nd_hdr_flag(const SpacePacket* sp);

/**
 * @brief  Secondary Header Flag を設定
 * @param[in,out] sp:   SpacePacket
 * @param[in]     flag: SP_2ND_HDR_FLAG
 * @return void
 */
void SP_set_2nd_hdr_flag(SpacePacket* sp, SP_2ND_HDR_FLAG flag);

/**
 * @brief  APID を取得
 * @param  sp: SpacePacket
 * @return APID
 */
APID SP_get_apid(const SpacePacket* sp);

/**
 * @brief  APID を設定
 * @param[in,out] sp:   SpacePacket
 * @param[in]     apid: APID
 * @return void
 */
void SP_set_apid(SpacePacket* sp, APID apid);

/**
 * @brief  Sequence Flag を取得
 * @param  sp: SpacePacket
 * @return SP_SEQ_FLAG
 */
SP_SEQ_FLAG SP_get_seq_flag(const SpacePacket* sp);

/**
 * @brief  Sequence Flag を設定
 * @param[in,out] sp:   SpacePacket
 * @param[in]     flag: SP_SEQ_FLAG
 * @return void
 */
void SP_set_seq_flag(SpacePacket* sp, SP_SEQ_FLAG flag);

/**
 * @brief  Sequence Count を取得
 * @param  sp: SpacePacket
 * @return Sequence Count
 */
uint16_t SP_get_seq_count(const SpacePacket* sp);

/**
 * @brief  Sequence Count を設定
 * @param[in,out] sp:   SpacePacket
 * @param[in]     count: Sequence Count
 * @return void
 */
void SP_set_seq_count(SpacePacket* sp, uint16_t count);

/**
 * @brief  Packet Data Length を取得
 * @param  sp: SpacePacket
 * @return Packet Data Length
 */
uint16_t SP_get_packet_data_len(const SpacePacket* sp);

/**
 * @brief  Packet Data Length を設定
 * @note   Packet Data Length とは PACKET DATA FIELD 長であり，PRIMARY HEADER を含まない， 0 起算の長さ
 *         ( https://public.ccsds.org/Pubs/133x0b2e1.pdf )
 * @param[in,out] sp:  SpacePacket
 * @param[in]     len: Packet Data Length
 * @return void
 */
void SP_set_packet_data_len(SpacePacket* sp, uint16_t len);


// ******************
//  Utility Function
// ******************

/**
 * @brief  Packet Length を取得
 * @param  sp: SpacePacket
 * @return Packet Length
 */
uint16_t SP_get_packet_len(const SpacePacket* sp);

/**
 * @brief  Packet Length を設定
 * @param[in,out] sp:  SpacePacket
 * @param[in]     len: Packet Length
 * @return void
 */
void SP_set_packet_len(SpacePacket* sp, uint16_t len);

/**
 * @brief  SpacePacket をコピー
 * @note   高速化のためすすべてではなく必要最低限しかコピーしない
 * @param[in,out] dest: コピー先 SpacePacket
 * @param[in]     src:  コピー元 SpacePacket
 * @return void
 */
void SP_copy_packet(SpacePacket* dest, const SpacePacket* src);

/**
 * @brief  packet に格納された param を取得
 * @param sp[in]:   SpacePacket
 * @param pei[in]:  SP_ParamExtractionInfo
 * @param dst[out]: 格納先のポインタ
 * @return void
 */
void SP_extract_param_from_packet(const SpacePacket* sp,
                                  const SP_ParamExtractionInfo* pei,
                                  void* dst);

/**
 * @brief  packet に格納された param を設定
 * @param sp[in,out]: SpacePacket
 * @param pei[in]:    SP_ParamExtractionInfo
 * @param src[in]:    格納元のポインタ
 * @return void
 */
void SP_insert_param_from_packet(SpacePacket* sp,
                                 const SP_ParamExtractionInfo* pei,
                                 const void* src);

#endif
