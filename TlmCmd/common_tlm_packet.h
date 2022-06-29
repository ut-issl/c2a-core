/**
 * @file
 * @brief  CTP 関連基本関数の宣言
 * @note   CTP:  CommonTlmPacket
 */
#ifndef COMMON_TLM_PACKET_H_
#define COMMON_TLM_PACKET_H_

#include "../System/TimeManager/obc_time.h"
#include <src_user/TlmCmd/telemetry_definitions.h>

// ここで， CTP_APID_FROM_ME, CTP_MAX_LEN, CommonTlmPacket として使うパケット型を指定する
#include <src_user/Settings/TlmCmd/common_tlm_packet_define.h>

// ここで APID を定義する
// 詳細は common_tlm_cmd_packet.h を参照
#include <src_user/Settings/TlmCmd/Ccsds/apid_define.h>

typedef uint8_t ctp_dest_flags_t;     //!< CTP_DEST_FLAG の & をとったフラグのための型

/**
 * @enum   CTP_DEST_FLAG
 * @brief  配送先指定．テレメのキューを指定するフラグ．
 * @note   8bit のフラグ．重送できるように， bit ごとのフラグとする
 * @note   これらのフラグの & を取ったものの方は ctp_dest_flags_t
 */
typedef enum
{
  CTP_DEST_FLAG_HK       = 0x01,  //!< 00000001b: Housekeeping Telemetry
  CTP_DEST_FLAG_MS       = 0x02,  //!< 00000010b: Mission Telemetry
  CTP_DEST_FLAG_ST       = 0x04,  //!< 00000100b: Stored Telemetry
  CTP_DEST_FLAG_RP       = 0x08,  //!< 00001000b: Replay Telemetry
  CTP_DEST_FLAG_RESERVE1 = 0x10,  //!< 00010000b: 将来拡張用の確保領域
  CTP_DEST_FLAG_RESERVE2 = 0x20,  //!< 00100000b: 将来拡張用の確保領域
  CTP_DEST_FLAG_RESERVE3 = 0x40,  //!< 01000000b: 将来拡張用の確保領域
  CTP_DEST_FLAG_RESERVE4 = 0x80   //!< 10000000b: 将来拡張用の確保領域
} CTP_DEST_FLAG;


/**
 * @brief  有効なパケットかチェックする
 * @note   NULL チェックも行う
 * @note   OBC の外から来たパケットな可能性もあるので，このC2Aで扱えるサイズかもチェックする
 * @param  packet: CTP
 * @retval 1: True
 * @retval 0: False
 */
int CTP_is_valid_packet(const CommonTlmPacket* packet);

/**
 * @brief  APID を取得
 * @param  packet: CTP
 * @return APID
 */
APID CTP_get_apid(const CommonTlmPacket* packet);

/**
 * @brief  APID を設定
 * @param[in,out] packet: CTP
 * @param[in]     apid: APID
 * @return void
 */
void CTP_set_apid(CommonTlmPacket* packet, APID apid);

/**
 * @brief  現在の global_time を設定
 * @note   何を設定するかは user 定義
 * @param[in,out] packet: CTP
 * @return void
 */
void CTP_set_global_time(CommonTlmPacket* packet);

/**
 * @brief  CTP_DEST_FLAG の & を取った flags を取得
 * @param  packet: CTP
 * @return Dest Flags
 */
ctp_dest_flags_t CTP_get_dest_flags(const CommonTlmPacket* packet);

/**
 * @brief  CTP_DEST_FLAG の & を取った flags を設定
 * @param[in,out] packet: CTP
 * @param[in]     flags: Dest Flags
 * @return void
 */
void CTP_set_dest_flags(CommonTlmPacket* packet, ctp_dest_flags_t flags);

/**
 * @brief  TLM ID を取得
 * @param  packet: CTP
 * @return TLM_CODE
 */
TLM_CODE CTP_get_id(const CommonTlmPacket* packet);

/**
 * @brief  TLM ID を設定
 * @param[in,out] packet: CTP
 * @param[in]     id: TLM_CODE
 * @return void
 */
void CTP_set_id(CommonTlmPacket* packet, TLM_CODE id);

/**
 * @brief  パケット長を取得
 * @param  packet: CTP
 * @return packet len
 */
uint16_t CTP_get_packet_len(const CommonTlmPacket* packet);

/**
 * @brief  ADU 分割なしの場合の User Data 先頭のポインタを取得
 * @param  packet: CTP
 * @note   FIXME: この関数で取得したポインタ経由で tlm のデータをセットする事があるため， const をつけていない
 *                引数の const を外しても良いかも？
 */
uint8_t* CTP_get_user_data_head(CommonTlmPacket* packet);

/**
 * @brief  CommonTlmPacket をコピー
 * @note   user 実装次第だが，高速化のためすすべてではなく必要最低限しかコピーしない場合もある
 * @param[in,out] dest: コピー先 CTP
 * @param[in]     src:  コピー元 CTP
 * @return void
 */
void CTP_copy_packet(CommonTlmPacket* dest, const CommonTlmPacket* src);

#endif
