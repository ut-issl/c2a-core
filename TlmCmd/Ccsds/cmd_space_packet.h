/**
 * @file
 * @brief CCSDS で規定される Space Packet の コマンド版の実装
 * @brief C ではテンプレートが使えないため，別で定義する
 * @note  詳細は space_packet.h を参照
 * @note  現在は， Sec. HDR Ver.1 のみ．追加するときに拡張していく
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
 *        |       6 |     0 |     8 | Sec. HDR Ver     |
 *        |       7 |     0 |     8 | Command Type     |
 *        |       8 |     0 |    16 | Command ID       |
 *        |      10 |     0 |     4 | Dest Type        |
 *        |      10 |     4 |     4 | Execution Type   |
 *        |      11 |     0 |    32 | Time Indicator   |
 *        |---------+-------+-------+------------------|
 *        | === User Data Field ====================== |
 *        |---------+-------+-------+------------------|
 *        |      15 |     0 |     * | Cmd Param #0     |
 *        |       * |     0 |     * | Cmd Param #1     |
 *        |       * |     0 |     * | Cmd Param #2     |
 *        |       * |     0 |     * | ...              |
 *        |---------+-------+-------+------------------|
 */
#ifndef CMD_SPACE_PACKET_H_
#define CMD_SPACE_PACKET_H_

// はじめにバージョン型だけ定義
/**
 * @enum   CSP_2ND_HDR_VER
 * @brief  Secondary Header Version No
 * @note   8bit
 */
typedef enum
{
  CSP_2ND_HDR_VER_UNKNOWN = 0,  //!< バージョン不定
  CSP_2ND_HDR_VER_1 = 1         //!< Version-1
} CSP_2ND_HDR_VER;


#define CSP_SND_HDR_LEN         (9)                   //!< Secondary Header 長
#define CSP_2ND_HDR_VER_TO_USE  (CSP_2ND_HDR_VER_1)   //!< 使う Sec. HDR Ver

// CSP_MAX_LEN, CSP_2ND_HDR_VER_TO_USE を再定義
#include <src_user/Settings/TlmCmd/Ccsds/cmd_space_packet_params.h>

// 相互参照問題があるので，この順番で include
#include <stdint.h>
#include "space_packet_typedef.h"
#include "space_packet.h"
#include "../common_cmd_packet.h"

#if CSP_MAX_LEN <= SP_PRM_HDR_LEN
#error CSP_MAX_LEN is too small
#endif


/**
 * @enum   CSP_CMD_TYPE
 * @brief  Command Type
 * @note   8bit
 * @note   過去では使われていたが，現在は（ひとまず）使ってない
 */
typedef enum
{
  CSP_CMD_TYPE_UNKNOWN
} CSP_CMD_TYPE;


// ******************************
//  Primary Header getter/setter
// ******************************

/**
 * @brief  Version No を取得
 * @param  csp: CmdSpacePacket
 * @return SP_VER
 */
SP_VER CSP_get_ver(const CmdSpacePacket* csp);

/**
 * @brief  Version No を設定
 * @param[in,out] csp: CmdSpacePacket
 * @param[in]     ver: SP_VER
 * @return void
 */
void CSP_set_ver(CmdSpacePacket* csp, SP_VER ver);

/**
 * @brief  Packet Type を取得
 * @param  csp: CmdSpacePacket
 * @return SP_TYPE
 */
SP_TYPE CSP_get_type(const CmdSpacePacket* csp);

/**
 * @brief  Packet Type を設定
 * @param[in,out] csp:  CmdSpacePacket
 * @param[in]     type: SP_TYPE
 * @return void
 */
void CSP_set_type(CmdSpacePacket* csp, SP_TYPE type);

/**
 * @brief  Secondary Header Flag を取得
 * @param  csp: CmdSpacePacket
 * @return SP_2ND_HDR_FLAG
 */
SP_2ND_HDR_FLAG CSP_get_2nd_hdr_flag(const CmdSpacePacket* csp);

/**
 * @brief  Secondary Header Flag を設定
 * @param[in,out] csp:  CmdSpacePacket
 * @param[in]     flag: SP_2ND_HDR_FLAG
 * @return void
 */
void CSP_set_2nd_hdr_flag(CmdSpacePacket* csp, SP_2ND_HDR_FLAG flag);

/**
 * @brief  APID を取得
 * @param  csp: CmdSpacePacket
 * @return APID
 */
APID CSP_get_apid(const CmdSpacePacket* csp);

/**
 * @brief  APID を設定
 * @param[in,out] csp:  CmdSpacePacket
 * @param[in]     apid: APID
 * @return void
 */
void CSP_set_apid(CmdSpacePacket* csp, APID apid);

/**
 * @brief  Sequence Flag を取得
 * @param  csp: CmdSpacePacket
 * @return SP_SEQ_FLAG
 */
SP_SEQ_FLAG CSP_get_seq_flag(const CmdSpacePacket* csp);

/**
 * @brief  Sequence Flag を設定
 * @param[in,out] csp:  CmdSpacePacket
 * @param[in]     flag: SP_SEQ_FLAG
 * @return void
 */
void CSP_set_seq_flag(CmdSpacePacket* csp, SP_SEQ_FLAG flag);

/**
 * @brief  Sequence Count を取得
 * @param  csp: CmdSpacePacket
 * @return Sequence Count
 */
uint16_t CSP_get_seq_count(const CmdSpacePacket* csp);

/**
 * @brief  Sequence Count を設定
 * @param[in,out] csp:   CmdSpacePacket
 * @param[in]     count: Sequence Count
 * @return void
 */
void CSP_set_seq_count(CmdSpacePacket* csp, uint16_t count);

/**
 * @brief  Packet Data Length を取得
 * @param  csp: CmdSpacePacket
 * @return Packet Data Length
 */
uint16_t CSP_get_packet_data_len(const CmdSpacePacket* csp);

/**
 * @brief  Packet Data Length を設定
 * @note   Packet Data Length とは PACKET DATA FIELD 長であり，PRIMARY HEADER を含まない， 0 起算の長さ
 *         ( https://public.ccsds.org/Pubs/133x0b2e1.pdf )
 * @param[in,out] csp: CmdSpacePacket
 * @param[in]     len: Packet Data Length
 * @return void
 */
void CSP_set_packet_data_len(CmdSpacePacket* csp, uint16_t len);


// **************************
//  2nd Header getter/setter
// **************************

/**
 * @brief  Secondary Header Version No を取得
 * @param  csp: CmdSpacePacket
 * @return CSP_2ND_HDR_VER
 */
CSP_2ND_HDR_VER CSP_get_2nd_hdr_ver(const CmdSpacePacket* csp);

/**
 * @brief  Secondary Header Version No を設定
 * @param[in,out] csp: CmdSpacePacket
 * @param[in]     id:  CSP_2ND_HDR_VER
 * @return void
 */
void CSP_set_2nd_hdr_ver(CmdSpacePacket* csp, CSP_2ND_HDR_VER ver);

/**
 * @brief  Command Type を取得
 * @param  csp: CmdSpacePacket
 * @return CSP_CMD_TYPE
 */
CSP_CMD_TYPE CSP_get_cmd_type(const CmdSpacePacket* csp);

/**
 * @brief  Command Type を設定
 * @param[in,out] csp:      CmdSpacePacket
 * @param[in]     cmd_type: CSP_CMD_TYPE
 * @return void
 */
void CSP_set_cmd_type(CmdSpacePacket* csp, CSP_CMD_TYPE cmd_type);

/**
 * @brief  Command ID を取得
 * @param  csp: CmdSpacePacket
 * @return Command ID
 */
CMD_CODE CSP_get_cmd_id(const CmdSpacePacket* csp);

/**
 * @brief  Command ID を設定
 * @param[in,out] csp: CmdSpacePacket
 * @param[in]     id:  Command ID
 * @return void
 */
void CSP_set_cmd_id(CmdSpacePacket* csp, CMD_CODE id);

/**
 * @brief  Destination Type を取得
 * @param  csp: CmdSpacePacket
 * @return CCP_DEST_TYPE
 */
CCP_DEST_TYPE CSP_get_dest_type(const CmdSpacePacket* csp);

/**
 * @brief  Destination Type を設定
 * @param[in,out] csp:       CmdSpacePacket
 * @param[in]     dest_type: CCP_DEST_TYPE
 * @return void
 */
void CSP_set_dest_type(CmdSpacePacket* csp, CCP_DEST_TYPE dest_type);

/**
 * @brief  Execution Type を取得
 * @param  csp: CmdSpacePacket
 * @return CCP_EXEC_TYPE
 */
CCP_EXEC_TYPE CSP_get_exec_type(const CmdSpacePacket* csp);

/**
 * @brief  Execution Type を設定
 * @param[in,out] csp:       CmdSpacePacket
 * @param[in]     exec_type: CCP_EXEC_TYPE
 * @return void
 */
void CSP_set_exec_type(CmdSpacePacket* csp, CCP_EXEC_TYPE exec_type);

/**
 * @brief  Time Indicator (TI) を取得
 * @param  csp: CmdSpacePacket
 * @return TI
 */
uint32_t CSP_get_ti(const CmdSpacePacket* csp);

/**
 * @brief  Time Indicator (TI) を設定
 * @param[in,out] csp: CmdSpacePacket
 * @param[in]     ti:  TI
 * @return void
 */
void CSP_set_ti(CmdSpacePacket* csp, uint32_t ti);


// ******************
//  Utility Function
// ******************

/**
 * @brief  Packet Length を取得
 * @param  csp: CmdSpacePacket
 * @return Packet Length
 */
uint16_t CSP_get_packet_len(const CmdSpacePacket* csp);

/**
 * @brief  Packet Length を設定
 * @param[in,out] csp: CmdSpacePacket
 * @param[in]     len: Packet Length
 * @return void
 */
void CSP_set_packet_len(CmdSpacePacket* csp, uint16_t len);

/**
 * @brief  CmdSpacePacket をコピー
 * @note   高速化のためすすべてではなく必要最低限しかコピーしない
 * @param[in,out] dest: コピー先 CmdSpacePacket
 * @param[in]     src:  コピー元 CmdSpacePacket
 * @return void
 */
void CSP_copy_packet(CmdSpacePacket* dest, const CmdSpacePacket* src);

/**
 * @brief  コマンドパラメタの先頭ポインタを取得
 * @param  csp: CmdSpacePacket
 * @return *param_head
 */
const uint8_t* CSP_get_param_head(const CmdSpacePacket* csp);

/**
 * @brief  Param Length を取得
 * @param  csp: CmdSpacePacket
 * @return Param Length
 */
uint16_t CSP_get_param_len(const CmdSpacePacket* csp);

/**
 * @brief  コマンドパラメタを設定
 * @param[in,out] csp:   CmdSpacePacket
 * @param[in]     param: パラメタ配列のポインタ
 * @param[in]     len:   パラメタ長
 * @return void
 */
void CSP_set_param(CmdSpacePacket* csp, const uint8_t* param, uint16_t len);

/**
 * @brief  共通部分の Header を構築
 * @param[in,out] csp: CmdSpacePacket
 * @return void
 */
void CSP_set_common_hdr(CmdSpacePacket* csp);

#endif
