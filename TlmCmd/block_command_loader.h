/**
 * @file
 * @brief  ブロックコマンド登録関連のユーティリティ
 *
 *         本機能はブロックコマンドの登録部をwrapし，userが簡潔かつ安全な形でブロック
 *         コマンド登録を行えるようにするためのユーティリティ機能です．
 */
#ifndef BLOCK_COMMAND_LOADER_H_
#define BLOCK_COMMAND_LOADER_H_

#include "./block_command_table.h"
#include "../Applications/timeline_command_dispatcher.h"
#include "../System/TimeManager/obc_time.h"
#include <src_user/Applications/app_registry.h>


/**
 * @brief  指定されたブロックコマンドIDへの登録を安全に行うためのwrapper関数
 * @note   BlockCommandDefinitions.cで呼ばれることを想定している
 * @param  pos        登録先のブロックコマンドID
 * @param  *func      実際に登録を行う関数へのポインタ
 *                    この関数内部はBCL_tool_xxxで構成されていることを想定している
 */
void BCL_load_bc(bct_id_t pos, void (*func)(void));

// 実装予定
#if 0
void BCL_load_tl(bct_id_t pos, void (*func)(void));
void BCL_load_sl(bct_id_t pos, void (*func)(void));
#endif


// register系列

/**
 * @brief  ブロックコマンドの最後にコマンドを追加する
 * @note   ブロックコマンドの定義時に使用する
 * @param  ti         コマンドを実行する相対TI
 * @param  cmd_id     実行するコマンドID
 * @return void
 */
void BCL_tool_register_cmd(cycle_t ti, CMD_CODE cmd_id);

/**
 * @brief  ブロックコマンドの最後に sub OBC コマンドを追加する
 * @note   ブロックコマンドの定義時に使用する
 * @param  ti         コマンドを実行する相対TI
 * @param  apid       sub OBC を識別する APID
 * @param  cmd_id     実行する sub OBC のコマンドID
 * @return void
 */
void BCL_tool_register_other_obc_cmd(cycle_t ti, APID apid, CMD_CODE cmd_id);

/**
 * @brief  ブロックコマンドの最後にローテーターの実行コマンドを追加する
 * @note   ブロックコマンドの定義時に使用する
 * @param  ti         コマンドを実行する相対TI
 * @param  bct_id     ローテートするブロックコマンドID
 * @return void
 */
void BCL_tool_register_rotate(cycle_t ti, bct_id_t bct_id);

/**
 * @brief  ブロックコマンドの最後にコンバイナーの実行コマンドを追加する
 * @note   ブロックコマンドの定義時に使用する
 * @param  ti         コマンドを実行する相対TI
 * @param  bct_id     コンバインするブロックコマンドID
 * @return void
 */
void BCL_tool_register_combine(cycle_t ti, bct_id_t bct_id);

/**
 * @brief  ブロックコマンドの最後にタイムリミットコンバイナーの実行コマンドを追加する
 * @note   ブロックコマンドの定義時に使用する
 * @param  ti         コマンドを実行する相対TI
 * @param  bct_id     タイムリミットコンバインするブロックコマンドID
 * @param  limit_step 超過すると実行を打ち切るステップ数
 * @return void
 */
void BCL_tool_register_limit_combine(cycle_t ti, bct_id_t bct_id, step_t limit_step);

/**
 * @brief  ブロックコマンドの最後にブロックコマンドの展開コマンドを追加する
 * @note   BlockCommandDefinitions.cで呼ばれることを想定している
 * @param  ti         コマンドを実行する相対TI
 * @param  bct_id     展開するブロックコマンドID
 * @param  tlcd_id      展開先のタイムラインID
 * @return void
 */
void BCL_tool_register_deploy(cycle_t ti, bct_id_t bct_id, TLCD_ID tlcd_id);

/**
 * @brief  ブロックコマンドの最後にアプリの実行コマンドを追加する
 * @note   BlockCommandDefinitions.cで呼ばれることを想定している
 * @param  ti         コマンドを実行する相対TI
 * @param  app_id     実行するアプリID
 * @return void
 */
void BCL_tool_register_app(cycle_t ti, AR_APP_ID app_id);


// add cmd param系列

/**
 * @brief  コマンド登録前にコマンドの引数としてuint8のパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_uint8(uint8_t val);

/**
 * @brief  コマンド登録前にコマンドの引数としてint8のパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_int8(int8_t val);

/**
 * @brief  コマンド登録前にコマンドの引数としてuint16のパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_uint16(uint16_t val);

/**
 * @brief  コマンド登録前にコマンドの引数としてint16のパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_int16(int16_t val);

/**
 * @brief  コマンド登録前にコマンドの引数としてuint32のパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_uint32(uint32_t val);

/**
 * @brief  コマンド登録前にコマンドの引数としてint32のパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_int32(int32_t val);

/**
 * @brief  コマンド登録前にコマンドの引数としてfloatのパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_float(float val);

/**
 * @brief  コマンド登録前にコマンドの引数としてuint64のパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_uint64(uint64_t val);

/**
 * @brief  コマンド登録前にコマンドの引数としてint64のパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_int64(int64_t val);

/**
 * @brief  コマンド登録前にコマンドの引数としてdoubleのパラメータを追加する
 * @note   BCL_tool_register_cmd前でのみ使用することを想定している
 * @param  val        追加するパラメータ
 * @return void
 */
void BCL_tool_prepare_param_double(double val);

#endif
