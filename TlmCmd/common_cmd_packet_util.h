/**
 * @file
 * @brief  CCP の汎用 Utility
 */
#ifndef COMMON_CMD_PACKET_UTIL_H_
#define COMMON_CMD_PACKET_UTIL_H_

#include "common_cmd_packet.h"
#include "packet_handler.h"
#include "../Applications/timeline_command_dispatcher_id_define.h"
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
 * @brief コマンド返り値である CCP_CmdRet を作成
 * @note  err_code を使わないときはそれを明示するために CCP_make_cmd_ret_without_err_code をつかうこと
 * @param[in] exec_sts: コマンド実行結果 (CCP_EXEC_STS)
 * @param[in] err_code: ユーザー定義エラーコード
 * @return CCP_CmdRet
 */
CCP_CmdRet CCP_make_cmd_ret(CCP_EXEC_STS exec_sts, uint32_t err_code);

/**
 * @brief コマンド返り値である CCP_CmdRet を作成（エラーコード不使用版）
 * @param[in] exec_sts: コマンド実行結果 (CCP_EXEC_STS)
 * @return CCP_CmdRet
 */
CCP_CmdRet CCP_make_cmd_ret_without_err_code(CCP_EXEC_STS exec_sts);

/**
 * @brief App 実行 TL コマンドを生成
 * @note  生成した時は CCP_EXEC_TYPE_TL_FROM_GS
 * @param[in,out] packet: CCP
 * @param[in]     ti:     TI
 * @param[in]     id:     AR_APP_ID
 * @return void
 */
void CCP_form_app_cmd(CommonCmdPacket* packet, cycle_t ti, AR_APP_ID id);

/**
 * @brief Realtime command を生成
 * @note  引数が不正なとき， packet は NOP RTC を返す
 * @note  RTC のキューに登録までする場合は `CCP_register_rtc` を使用
 * @param[in,out] packet: CCP
 * @param[in]     cmd_id: CMD_CODE
 * @param[in]     param:  パラメタ
 * @param[in]     len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_rtc(CommonCmdPacket* packet, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief Timeline command を生成
 * @note  引数が不正なとき， packet は NOP TLC を返す
 * @note  TL に登録までする場合は `CCP_register_tlc` を使用
 * @param[in,out] packet:  CCP
 * @param[in]     ti:      TI
 * @param[in]     cmd_id:  CMD_CODE
 * @param[in]     param:   パラメタ
 * @param[in]     len:     パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_tlc(CommonCmdPacket* packet, cycle_t ti, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief 他の OBC のコマンドを RT として生成
 * @note  本OBC で RT として処理されたあと 他の OBC に送られ RT として実行される
 * @note  param チェックは未実装
 * @param[in] apid:   どの OBC かを指定する APID
 * @param[in] cmd_id: CMD_CODE
 * @param[in] param:  パラメタ
 * @param[in] len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_rtc_to_other_obc(CommonCmdPacket* packet, APID apid, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief 他の OBC のコマンドを TL として生成
 * @note  本OBC で TL として処理されたあと 他の OBC に送られ RT として実行される
 * @note  param チェックは未実装
 * @param[in] ti:     TI
 * @param[in] apid:   どの OBC かを指定する APID
 * @param[in] cmd_id: CMD_CODE
 * @param[in] param:  パラメタ
 * @param[in] len:    パラメタ長
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_tlc_to_other_obc(CommonCmdPacket* packet, cycle_t ti, APID apid, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief BC展開 Realtime command を生成
 * @note  引数が不正なとき， packet は NOP RTC を返す
 * @param[in,out] packet:   CCP
 * @param[in]     tl_no:    Timeline no
 * @param[in]     block_no: BC ID
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_form_block_deploy_cmd(CommonCmdPacket* packet, TLCD_ID tl_no, bct_id_t block_no);

/**
 * @brief Realtime Command から Timeline Command へ変換
 * @param[in,out] packet: 変換する packet
 * @param[in]     ti:     TI
 * @return void
 */
void CCP_convert_rtc_to_tlc(CommonCmdPacket* packet, cycle_t ti);

/**
 * @brief Realtime command を登録
 * @note  不正な時は登録されない
 * @param[in] cmd_id: CMD_CODE
 * @param[in] param:  パラメタ
 * @param[in] len:    パラメタ長
 * @return PH_ACK
 */
PH_ACK CCP_register_rtc(CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief Timeline command を登録
 * @param[in] ti:      TI
 * @param[in] tlcd_id: 登録する tl id
 * @param[in] cmd_id:  CMD_CODE
 * @param[in] param:   パラメタ
 * @param[in] len:     パラメタ長
 * @return PH_ACK
 */
PH_ACK CCP_register_tlc(cycle_t ti, TLCD_ID tlcd_id, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief Timeline command を登録 (ASAP 版)
 * @note  引数が不正なときは登録されない
 * @note  既にその TI が埋まっていた場合その後で最速の TI が勝手に設定される
 * @param[in] ti:      TI
 * @param[in] tlcd_id: 登録する tl id
 * @param[in] cmd_id:  CMD_CODE
 * @param[in] param:   パラメタ
 * @param[in] len:     パラメタ長
 * @return PH_ACK
 */
PH_ACK CCP_register_tlc_asap(cycle_t ti, TLCD_ID tlcd_id, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief Realtime command を生成し，即時実行する
 * @note  RTC のキューに登録する場合は `CCP_register_rtc` を使用
 * @note  生成される command は RTC だが，キューイングされずに即時実行されるため RTC Dispatcher にはログは残らない
 * @param[in]     cmd_id: CMD_CODE
 * @param[in]     param:  パラメタ
 * @param[in]     len:    パラメタ長
 * @retval CCP_CmdRet{CCP_EXEC_PACKET_FMT_ERR, *}: 引数が不正なとき
 * @retval それ以外: PH_dispatch_command の返り値
 */
CCP_CmdRet CCP_form_and_exec_rtc(CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief 他の OBC のコマンドを RT として生成し, 即時実行する
 * @note  生成される command は RTC だが，キューイングされずに即時実行されるため RTC Dispatcher にはログは残らない
 * @param[in]     apid:   APID
 * @param[in]     cmd_id: CMD_CODE
 * @param[in]     param:  パラメタ
 * @param[in]     len:    パラメタ長
 * @retval CCP_CmdRet{CCP_EXEC_PACKET_FMT_ERR, *}: 引数が不正なとき
 * @retval それ以外: PH_dispatch_command の返り値
 */
CCP_CmdRet CCP_form_and_exec_rtc_to_other_obc(APID apid, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief BC展開 command を生成し，即時実行する
 * @param[in]     tl_no:    Timeline no
 * @param[in]     block_no: BC ID
 * @retval CCP_CmdRet{CCP_EXEC_PACKET_FMT_ERR, *}: 引数が不正なとき
 * @retval それ以外: PH_dispatch_command の返り値
 */
CCP_CmdRet CCP_form_and_exec_block_deploy_cmd(TLCD_ID tl_no, bct_id_t block_no);

/**
 * @brief TLCD ID から CCP_EXEC_TYPE を取得する
 * @param[in] tlcd_id: tl id
 * @note 引数が不正な場合は CCP_EXEC_TYPE_UNKNOWN を返す
 * @return CCP_EXEC_TYPE
 */
CCP_EXEC_TYPE CCP_get_exec_type_from_tlcd_id(TLCD_ID tlcd_id);

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
 * @brief  CCP packet から，RAW コマンド引数の先頭ポインタを取得する
 * @note   RAW パラメタが存在しない場合は， NULL を返す
 * @note   RAW パラメタが存在しつつ，そのサイズが 0 だった場合は， NULL ではなく最終 param の次のアドレスを返す
 * @param[in]  packet: 取得する packet
 * @return RAW コマンド引数の先頭ポインタ
 */
const uint8_t* CCP_get_raw_param_head(const CommonCmdPacket* packet);

/**
 * @brief  CCP packet から，RAW コマンド引数の長さを返す
 * @note   RAW パラメタが存在しない場合や，不正なパケットの場合は 0 を返す
 * @param[in]  packet: 取得する packet
 * @return RAW コマンド引数の長さ
 */
uint16_t CCP_get_raw_param_len(const CommonCmdPacket* packet);

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

/**
 * @brief  Param Generator を初期化
 * @note   Param Generator の使い方
 *           1. CCP_init_param_for_packet で初期化
 *           2. CCP_prepare_hoge_param_for_packet にて，パラメタを 1 つめからセットしていく
 *           3. CCP_get_prepared_param_for_packet で完成した param とその長さを取得する
 * @param[in] cmd_id: これから作成する param の Cmd ID
 * @return void
 */
void CCP_init_param_for_packet(CMD_CODE cmd_id);

/**
 * @brief  Param Generator から完成した param を取得する
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[out] param_head: 完成した param の先頭ポインタ
 * @param[out] len: paran の長さ
 * @retval CCP_UTIL_ACK_OK: 正常終了
 * @retval CCP_UTIL_ACK_PARAM_ERR: これまでの param 生成仮定で何かしらの不整合が発生
 */
CCP_UTIL_ACK CCP_get_prepared_param_for_packet(const uint8_t** param_head, uint16_t* len);

/**
 * @brief  Param Generator で uint8_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_uint8_param_for_packet(uint8_t param);

/**
 * @brief  Param Generator で int8_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_int8_param_for_packet(int8_t param);

/**
 * @brief  Param Generator で uint16_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_uint16_param_for_packet(uint16_t param);

/**
 * @brief  Param Generator で int16_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_int16_param_for_packet(int16_t param);

/**
 * @brief  Param Generator で uint32_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_uint32_param_for_packet(uint32_t param);

/**
 * @brief  Param Generator で int32_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_int32_param_for_packet(int32_t param);

/**
 * @brief  Param Generator で uint64_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_uint64_param_for_packet(uint64_t param);

/**
 * @brief  Param Generator で int64_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_int64_param_for_packet(int64_t param);

/**
 * @brief  Param Generator で float のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_float_param_for_packet(float param);

/**
 * @brief  Param Generator で double のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_double_param_for_packet(double param);

/**
 * @brief  Param Generator で bct_id_t のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_bct_id_param_for_packet(bct_id_t param);

/**
 * @brief  Param Generator で raw パラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ配列
 * @param[in] len: 登録するパラメタの長さ
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_prepare_raw_param_for_packet(const uint8_t* param, uint16_t len);

#endif
