/**
 * @file
 * @brief  CCP の汎用 Utility
 */
#ifndef COMMON_CMD_PACKET_UTIL_H_
#define COMMON_CMD_PACKET_UTIL_H_

#include "common_cmd_packet.h"
#include "../Applications/timeline_command_dispatcher.h" // for TLCD_ID
#include "block_command_table.h" // for bct_id
#include <src_user/Applications/app_registry.h>

/**
 * @enum   CCP_UTIL_ACK
 * @brief  CCP Utility の汎用返り値
 * @note   uint8_t を想定
 */
typedef enum
{
  CCP_UTIL_ACK_OK = 0,       //!< 正常終了
  CCP_UTIL_ACK_PARAM_ERR     //!< パラメタエラー
} CCP_UTIL_ACK;

/**
 * @brief  App 実行コマンドを生成
 * @note   TL登録まで行う場合は `CCP_register_app_cmd` を使用
 * @param[in,out] packet: CCP
 * @param[in]     ti: TI
 * @param[in]     id: AR_APP_ID
 * @return void
 */
void CCP_form_app_cmd(CommonCmdPacket* packet, cycle_t ti, AR_APP_ID id);

/**
 * @brief  Realtime command を生成
 * @note   引数が不正なとき， packet は NOP RTC を返す
 * @note   RTC のキューに登録までする場合は `CCP_register_rtc` を使用
 * @param[in,out] packet: CCP
 * @param[in]     packet: CMD_CODE
 * @param[in]     param:  パラメタ
 * @param[in]     len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_rtc(CommonCmdPacket* packet, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  Timeline command を生成
 * @note   引数が不正なとき， packet は NOP TLC を返す
 * @note   TL に登録までする場合は `CCP_register_tlc` を使用
 * @param[in,out] packet: CCP
 * @param[in]     ti:     TI
 * @param[in]     packet: CMD_CODE
 * @param[in]     param:  パラメタ
 * @param[in]     len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_tlc(CommonCmdPacket* packet, cycle_t ti, CCP_EXEC_TYPE type, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  Timeline command を生成
 * @note   引数が不正なとき， packet は NOP TLC を返す
 * @note   既にその TI が埋まっていた場合その後で最速の TI が勝手に設定される
 * @param[in,out] packet: CCP
 * @param[in]     ti:     TI
 * @param[in]     packet: CMD_CODE
 * @param[in]     param:  パラメタ
 * @param[in]     len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_tlc_asap(CommonCmdPacket* packet, cycle_t ti, CCP_EXEC_TYPE type, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  BC展開 command を生成
 * @note   引数が不正なとき， packet は NOP RTC を返す
 * @note   RTC のキューに登録までする場合は `CCP_register_block_deploy_cmd` を使用
 * @param[in,out] packet: CCP
 * @param[in]     tl_no: Timeline no
 * @param[in]     block_no: BC ID
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_block_deploy_cmd(CommonCmdPacket* packet, TLCD_ID tl_no, bct_id_t block_no);

/**
 * @brief  Realtime Command から Timeline Command へ変換
 * @param[in,out] packet: 変換する packet
 * @param[in]     ti:     TI
 * @return void
 */
void CCP_convert_rtc_to_tlc(CommonCmdPacket* packet, cycle_t ti);

/**
 * @brief  App 実行コマンドを登録
 * @param[in] ti: TI
 * @param[in] id: AR_APP_ID
 * @return PH_ACK
 */
PH_ACK CCP_register_app_cmd(cycle_t ti, AR_APP_ID id);

/**
 * @brief  Realtime command を登録
 * @param[in] cmd_id: CMD_CODE
 * @param[in] param:  パラメタ
 * @param[in] len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
PH_ACK CCP_register_rtc(CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  Timeline command を登録
 * @param[in] ti:     TI
 * @param[in] cmd_id: CMD_CODE
 * @param[in] param:  パラメタ
 * @param[in] len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
PH_ACK CCP_register_tlc(cycle_t ti, CCP_EXEC_TYPE type, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  Timeline command を登録
 * @note   引数が不正なとき， packet は NOP TLC を返す
 * @note   既にその TI が埋まっていた場合その後で最速の TI が勝手に設定される
 * @param[in,out] packet: CCP
 * @param[in]     ti:     TI
 * @param[in]     packet: CMD_CODE
 * @param[in]     param:  パラメタ
 * @param[in]     len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_register_tlc_asap(CommonCmdPacket* packet, cycle_t ti, CCP_EXEC_TYPE type, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  BC展開 command を登録
 * @note   RTC に登録される
 * @param[in] tl_no: Timeline no
 * @param[in] block_no: BC ID
 * @return CCP_UTIL_ACK
 */
PH_ACK CCP_register_block_deploy_cmd(TLCD_ID tl_no, bct_id_t block_no);

/**
 * @brief CCP_EXEC_TYPE から PacketList を取得する
 * @note  引数が不正 or 無い場合 NULL を返す
 * @param[in] type: CCP_EXEC_TYPE
 * @return PacketList の const ポインタ
 */
const PacketList* CCP_get_packet_list_from_exec_type(CCP_EXEC_TYPE type);

/**
 * @brief  CCP packet から，サイズが 1 byte のコマンド引数を取得する
 * @note   セグメンテーション違反の場合は， 0 が代入されたポインタを返す
 * @note   CCP_get_param_from_packet マクロ で使うことを想定し，ユーザーはこの関数を直接叩くことは想定していない
 * @param[in] packet: 取得する packet
 * @param[in] n: N番目の引数 （0起算）
 * @return パラメタを保持したポインタ（型は意味をもってない）
 */
uint8_t* CCP_get_1byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n);

/**
 * @brief  CCP packet から，サイズが 2 byte のコマンド引数を取得する
 * @note   セグメンテーション違反の場合は， 0 が代入されたポインタを返す
 * @note   CCP_get_param_from_packet マクロ で使うことを想定し，ユーザーはこの関数を直接叩くことは想定していない
 * @param[in] packet: 取得する packet
 * @param[in] n: N番目の引数 （0起算）
 * @return パラメタを保持したポインタ（型は意味をもってない）
 */
uint16_t* CCP_get_2byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n);

/**
 * @brief  CCP packet から，サイズが 4 byte のコマンド引数を取得する
 * @note   セグメンテーション違反の場合は， 0 が代入されたポインタを返す
 * @note   CCP_get_param_from_packet マクロ で使うことを想定し，ユーザーはこの関数を直接叩くことは想定していない
 * @param[in] packet: 取得する packet
 * @param[in] n: N番目の引数 （0起算）
 * @return パラメタを保持したポインタ（型は意味をもってない）
 */
uint32_t* CCP_get_4byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n);

/**
 * @brief  CCP packet から，サイズが 8 byte のコマンド引数を取得する
 * @note   セグメンテーション違反の場合は， 0 が代入されたポインタを返す
 * @note   CCP_get_param_from_packet マクロ で使うことを想定し，ユーザーはこの関数を直接叩くことは想定していない
 * @param[in] packet: 取得する packet
 * @param[in] n: N番目の引数 （0起算）
 * @return パラメタを保持したポインタ（型は意味をもってない）
 */
uint64_t* CCP_get_8byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n);

/**
 * @brief  CCP packet から，RAW コマンド引数を取得する
 * @note   RAW パラメタが存在しない場合は， 0 を返す
 * @param[in]  packet: 取得する packet
 * @param[out] dest: RAW パラメタコピー先
 * @param[in]  max_copy_len : コピーする最大長． 0 の場合，無制限
 * @return コピーした長さ
 */
uint16_t CCP_get_raw_param_from_packet(const CommonCmdPacket* packet, void* dest, uint16_t max_copy_len);

/**
 * @def    CCP_get_param_from_packet(packet, n, type)
 * @brief  CCP packet から，n番目のコマンド引数を取得する
 * @note   セグメンテーション違反の場合は， 0 を返す
 * @note   関数ではなくマクロであることに注意する
 * @param[in] packet: 取得する packet
 * @param[in] n: N番目の引数 （0起算）
 * @param[in] type: 取得するパラメタの型．ユーザー定義型でもOKだが， enum は不可
 * @return パラメタ（型は意味をもってない）
 * @note   使用例
 *         uint32_t param0 = CCP_get_param_from_packet(packet, 0, uint32_t);
 *         HOGE_ENUM param1 = (HOGE_ENUM)CCP_get_param_from_packet(packet, 1, uint8_t);
 */
#define CCP_get_param_from_packet(packet, n, type) \
(*((type*)( \
sizeof(type) == 1 ? \
(void*)CCP_get_1byte_param_from_packet(packet, n) : \
sizeof(type) == 2 ? \
(void*)CCP_get_2byte_param_from_packet(packet, n) : \
sizeof(type) == 4 ? \
(void*)CCP_get_4byte_param_from_packet(packet, n) : \
sizeof(type) == 8 ? \
(void*)CCP_get_8byte_param_from_packet(packet, n) : \
0)))

#endif
