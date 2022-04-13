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
 * @param[in,out] packet: CCP
 * @param[in]     ti: TI
 * @param[in]     id: AR_APP_ID
 * @return void
 */
void CCP_form_app_cmd(CommonCmdPacket* packet, cycle_t ti, AR_APP_ID id);

/**
 * @brief  Realtime command を生成
 * @note   引数が不正なとき， packet は NOP RTC を返す
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
 * @param[in,out] packet: CCP
 * @param[in]     ti:     TI
 * @param[in]     packet: CMD_CODE
 * @param[in]     param:  パラメタ
 * @param[in]     len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_tlc(CommonCmdPacket* packet, cycle_t ti, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  BC展開 command を生成
 * @note   引数が不正なとき， packet は NOP RTC を返す
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
