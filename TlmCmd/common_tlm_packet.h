/**
 * @file
 * @brief  CTP 関連基本関数の宣言
 * @note   CTP:  CommonTlmPacket
 */
#ifndef COMMON_TLM_PACKET_H_
#define COMMON_TLM_PACKET_H_

#include <src_user/Settings/TlmCmd/common_tlm_packet_define.h>

// ここで APID を定義する
// 詳細は common_tlm_cmd_packet.h を参照
#include <src_user/Settings/TlmCmd/Ccsds/apid_define.h>


/**
 * @enum   CTP_DEST_FLAG
 * @brief  配送先指定．テレメのキューを指定するフラグ．
 * @note   4bitのフラグ．重送できるように，bitごとのフラグとする
 */
typedef enum
{
  CTP_DEST_FLAG_HK = 0x01, // 00000001b: Housekeeping Telemetry
  CTP_DEST_FLAG_MS = 0x02, // 00000010b: Mission Telemetry
  CTP_DEST_FLAG_ST = 0x04, // 00000100b: Stored Telemetry
  CTP_DEST_FLAG_RP = 0x08, // 00001000b: Replay Telemetry
  CTP_DEST_FLAG_UNKNOWN
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
 * @brief  CTP_DEST_FLAG を取得
 * @param  packet: CTP
 * @return CTP_DEST_FLAG
 */
CTP_DEST_FLAG CTP_get_dest_flag(const CommonTlmPacket* packet);

/**
 * @brief  CTP_DEST_FLAG を設定
 * @param[in,out] packet: CTP
 * @param[in]     flag: CTP_DEST_FLAG
 * @return void
 */
void CTP_set_dest_flag(CommonTlmPacket* packet, CTP_DEST_FLAG flag);

/**
 * @brief  パケット長を取得
 * @param  packet: CTP
 * @return packet len
 */
uint16_t CTP_get_packet_len(const CommonTlmPacket* packet);

/**
 * @brief  ADU 分割なしの場合の User Data 先頭のポインタを取得
 * @param  packet: CTP
 * @note   TCP の場合， ADU 分割なし時のポインタを取得
 * @note   FIXME: この関数で取得したポインタ経由で tlm のデータをセットする事があるため， const をつけていない
 */
uint8_t* CTP_get_user_data_head(CommonTlmPacket* packet);


#endif
