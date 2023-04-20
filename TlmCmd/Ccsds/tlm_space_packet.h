/**
 * @file
 * @brief CCSDS で規定される Space Packet の テレメ版の実装
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
 *        |       7 |     0 |    32 | Board Time       |
 *        |      11 |     0 |     8 | Tlm ID           |
 *        |      12 |     0 |    64 | Global Time      |
 *        |      20 |     0 |    32 | On-Board         |
 *        |         |       |       |  Subnetwork Time |
 *        |      24 |     0 |     8 | Dest Flags       |
 *        |      25 |     0 |     8 | Dest Info        |
 *        |---------+-------+-------+------------------|
 *        | === User Data Field ====================== |
 *        |---------+-------+-------+------------------|
 *        |      26 |     0 |     * | Tlm Data #0      |
 *        |       * |     0 |     * | Tlm Data #1      |
 *        |       * |     0 |     * | Tlm Data #2      |
 *        |       * |     0 |     * | ...              |
 *        |---------+-------+-------+------------------|
 *
 *        以下の 3 つの時刻情報をパケットに仕込む
 *        - Board Time
 *          - テレメパケットを生成したボードのテレメ生成時間． TI など．型は任意．
 *        - Global Time
 *          - テレメパケットを生成したボードの絶対時刻でのテレメ生成時刻． GPS Time や unixtime など．型は任意．
 *          - 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF の場合， パケット中継中に MOBC で上書きされる
 *          - FIXME: 一旦 double で実装するが，本当は変えたほうが良いかもしれない．．．？
 *        - On-Board Subnetwork Time
 *          - オンボードサブネットワークでのテレメ生成時刻．オンボードサブネットワーク TI など．型は任意．
 *          - 0xFFFFFFFF の場合， パケット中継中に MOBC で上書きされる
 */
#ifndef TLM_SPACE_PACKET_H_
#define TLM_SPACE_PACKET_H_

// はじめにバージョン型だけ定義
/**
 * @enum   TSP_2ND_HDR_VER
 * @brief  Secondary Header Version No
 * @note   8bit
 */
typedef enum
{
  TSP_2ND_HDR_VER_UNKNOWN = 0,  //!< バージョン不定
  TSP_2ND_HDR_VER_1 = 1         //!< Version-1
} TSP_2ND_HDR_VER;


#define TSP_SND_HDR_LEN         (20)                   //!< Secondary Header 長
#define TSP_2ND_HDR_VER_TO_USE  (TSP_2ND_HDR_VER_1)   //!< 使う Sec. HDR Ver

// TSP_MAX_LEN, TSP_2ND_HDR_VER_TO_USE を再定義
#include <src_user/Settings/TlmCmd/Ccsds/tlm_space_packet_params.h>

// 相互参照問題があるので，この順番で include
#include <stdint.h>
#include "space_packet_typedef.h"
#include "space_packet.h"
#include "../common_tlm_packet.h"

#if TSP_MAX_LEN <= SP_PRM_HDR_LEN
#error TSP_MAX_LEN is too small
#endif


// ******************************
//  Primary Header getter/setter
// ******************************

/**
 * @brief  Version No を取得
 * @param  tsp: TlmSpacePacket
 * @return SP_VER
 */
SP_VER TSP_get_ver(const TlmSpacePacket* tsp);

/**
 * @brief  Version No を設定
 * @param[in,out] tsp: TlmSpacePacket
 * @param[in]     ver: SP_VER
 * @return void
 */
void TSP_set_ver(TlmSpacePacket* tsp, SP_VER ver);

/**
 * @brief  Packet Type を取得
 * @param  tsp: TlmSpacePacket
 * @return SP_TYPE
 */
SP_TYPE TSP_get_type(const TlmSpacePacket* tsp);

/**
 * @brief  Packet Type を設定
 * @param[in,out] tsp:  TlmSpacePacket
 * @param[in]     type: SP_TYPE
 * @return void
 */
void TSP_set_type(TlmSpacePacket* tsp, SP_TYPE type);

/**
 * @brief  Secondary Header Flag を取得
 * @param  tsp: TlmSpacePacket
 * @return SP_2ND_HDR_FLAG
 */
SP_2ND_HDR_FLAG TSP_get_2nd_hdr_flag(const TlmSpacePacket* tsp);

/**
 * @brief  Secondary Header Flag を設定
 * @param[in,out] tsp:  TlmSpacePacket
 * @param[in]     flag: SP_2ND_HDR_FLAG
 * @return void
 */
void TSP_set_2nd_hdr_flag(TlmSpacePacket* tsp, SP_2ND_HDR_FLAG flag);

/**
 * @brief  APID を取得
 * @param  tsp: TlmSpacePacket
 * @return APID
 */
APID TSP_get_apid(const TlmSpacePacket* tsp);

/**
 * @brief  APID を設定
 * @param[in,out] tsp:  TlmSpacePacket
 * @param[in]     apid: APID
 * @return void
 */
void TSP_set_apid(TlmSpacePacket* tsp, APID apid);

/**
 * @brief  Sequence Flag を取得
 * @param  tsp: TlmSpacePacket
 * @return SP_SEQ_FLAG
 */
SP_SEQ_FLAG TSP_get_seq_flag(const TlmSpacePacket* tsp);

/**
 * @brief  Sequence Flag を設定
 * @param[in,out] tsp:  TlmSpacePacket
 * @param[in]     flag: SP_SEQ_FLAG
 * @return void
 */
void TSP_set_seq_flag(TlmSpacePacket* tsp, SP_SEQ_FLAG flag);

/**
 * @brief  Sequence Count を取得
 * @param  tsp: TlmSpacePacket
 * @return Sequence Count
 */
uint16_t TSP_get_seq_count(const TlmSpacePacket* tsp);

/**
 * @brief  Sequence Count を設定
 * @param[in,out] tsp:   TlmSpacePacket
 * @param[in]     count: Sequence Count
 * @return void
 */
void TSP_set_seq_count(TlmSpacePacket* tsp, uint16_t count);

/**
 * @brief  Packet Data Length を取得
 * @param  tsp: TlmSpacePacket
 * @return Packet Data Length
 */
uint16_t TSP_get_packet_data_len(const TlmSpacePacket* tsp);

/**
 * @brief  Packet Data Length を設定
 * @note   Packet Data Length とは PACKET DATA FIELD 長であり，PRIMARY HEADER を含まない， 0 起算の長さ
 *         ( https://public.ccsds.org/Pubs/133x0b2e1.pdf )
 * @param[in,out] tsp: TlmSpacePacket
 * @param[in]     len: Packet Data Length
 * @return void
 */
void TSP_set_packet_data_len(TlmSpacePacket* tsp, uint16_t len);


// **************************
//  2nd Header getter/setter
// **************************

/**
 * @brief  Secondary Header Version No を取得
 * @param  tsp: TlmSpacePacket
 * @return TSP_2ND_HDR_VER
 */
TSP_2ND_HDR_VER TSP_get_2nd_hdr_ver(const TlmSpacePacket* tsp);

/**
 * @brief  Secondary Header Version No を設定
 * @param[in,out] tsp: TlmSpacePacket
 * @param[in]     id:  TSP_2ND_HDR_VER
 * @return void
 */
void TSP_set_2nd_hdr_ver(TlmSpacePacket* tsp, TSP_2ND_HDR_VER ver);

/**
 * @brief  Board Time を取得
 * @param  tsp: TlmSpacePacket
 * @return Board Time
 */
uint32_t TSP_get_board_time(const TlmSpacePacket* tsp);

/**
 * @brief  Board Time を設定
 * @param[in,out] tsp:  TlmSpacePacket
 * @param[in]     time: Board Time
 * @return void
 */
void TSP_set_board_time(TlmSpacePacket* tsp, uint32_t time);

/**
 * @brief  Global Time を取得
 * @param  tsp: TlmSpacePacket
 * @return Global Time
 */
double TSP_get_global_time(const TlmSpacePacket* tsp);

/**
 * @brief  Global Time を設定
 * @param[in,out] tsp:  TlmSpacePacket
 * @param[in]     time: Global Time
 * @return void
 */
void TSP_set_global_time(TlmSpacePacket* tsp, double time);

/**
 * @brief  On-Board Subnetwork Time を取得
 * @param  tsp: TlmSpacePacket
 * @return On-Board Subnetwork Time
 */
uint32_t TSP_get_on_board_subnet_time(const TlmSpacePacket* tsp);

/**
 * @brief  On-Board Subnetwork Time を設定
 * @param[in,out] tsp:  TlmSpacePacket
 * @param[in]     time: On-Board Subnetwork Time
 * @return void
 */
void TSP_set_on_board_subnet_time(TlmSpacePacket* tsp, uint32_t time);

/**
 * @brief  Tlm ID を取得
 * @param  tsp: TlmSpacePacket
 * @return Tlm ID
 */
TLM_CODE TSP_get_tlm_id(const TlmSpacePacket* tsp);

/**
 * @brief  Tlm ID を設定
 * @param[in,out] tsp: TlmSpacePacket
 * @param[in]     id:  Tlm ID
 * @return void
 */
void TSP_set_tlm_id(TlmSpacePacket* tsp, TLM_CODE id);

/**
 * @brief  Destination Flags を取得
 * @param  tsp: TlmSpacePacket
 * @return Destination Flags
 */
ctp_dest_flags_t TSP_get_dest_flags(const TlmSpacePacket* tsp);

/**
 * @brief  Destination Flags を設定
 * @param[in,out] tsp:   TlmSpacePacket
 * @param[in]     flags: Destination Flags
 * @return void
 */
void TSP_set_dest_flags(TlmSpacePacket* tsp, ctp_dest_flags_t flags);

/**
 * @brief  Dest Info を取得
 * @param  tsp: TlmSpacePacket
 * @return Dest Info
 */
uint8_t TSP_get_dest_info(const TlmSpacePacket* tsp);

/**
 * @brief  Dest Info を設定
 * @param[in,out] tsp:  TlmSpacePacket
 * @param[in]     info: Dest Info
 * @return void
 */
void TSP_set_dest_info(TlmSpacePacket* tsp, uint8_t info);


// ******************
//  Utility Function
// ******************

/**
 * @brief  Packet Length を取得
 * @param  tsp: TlmSpacePacket
 * @return Packet Length
 */
uint16_t TSP_get_packet_len(const TlmSpacePacket* tsp);

/**
 * @brief  Packet Length を設定
 * @param[in,out] tsp: TlmSpacePacket
 * @param[in]     len: Packet Length
 * @return void
 */
void TSP_set_packet_len(TlmSpacePacket* tsp, uint16_t len);

/**
 * @brief  TlmSpacePacket をコピー
 * @note   高速化のためすすべてではなく必要最低限しかコピーしない
 * @param[in,out] dest: コピー先 TlmSpacePacket
 * @param[in]     src:  コピー元 TlmSpacePacket
 * @return void
 */
void TSP_copy_packet(TlmSpacePacket* dest, const TlmSpacePacket* src);

/**
 * @brief  User Data Field の先頭ポインタを取得
 * @param  tsp: TlmSpacePacket
 * @return *user_data_head
 * @note   FIXME: この関数で取得したポインタ経由で tlm のデータをセットする事があるため， const をつけていない
 *                引数の const を外しても良いかも？
 */
uint8_t* TSP_get_user_data_head(const TlmSpacePacket* tsp);

/**
 * @brief  Primary Header を構築
 * @param[in,out] tsp:        TlmSpacePacket
 * @param[in]     apid:       APID
 * @param[in]     seq_count:  Sequence Count
 * @param[in]     packet_len: Packet Length
 * @return void
 */
void TSP_setup_primary_hdr(TlmSpacePacket* tsp, APID apid, uint16_t seq_count, uint16_t packet_len);

/**
 * @brief  共通部分の Header を構築
 * @param[in,out] tsp: TlmSpacePacket
 * @return void
 */
void TSP_set_common_hdr(TlmSpacePacket* tsp);

/**
 * @brief  パケット全体を 0x00 クリアし， fill_size に等しいサイズのパケットを生成する
 * @note   Primary Header Size より小さなパケットは作れないので，その場合は， SP_PRM_HDR_LEN + 1 の長さのパケットを生成する
 * @param[in,out] tsp: TlmSpacePacket
 * @param[in]     fill_size: packet size
 * @return void
 */
void TSP_setup_fill_packet(TlmSpacePacket* tsp, uint16_t fill_size);

#endif
