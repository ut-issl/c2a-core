/**
 * @file
 * @brief 各種コマンドの実行管理
 */
#ifndef COMMAND_DISPATCHER_H_
#define COMMAND_DISPATCHER_H_

#include "common_cmd_packet_util.h"
#include "common_cmd_packet.h"
#include "packet_list.h"
#include "../System/TimeManager/obc_time.h"
#include <src_user/TlmCmd/command_definitions.h>

/**
 * @struct CDIS_ExecInfo
 * @brief  コマンド実行情報
 */
typedef struct
{
  ObcTime time;         //!< 実行時刻
  CMD_CODE code;        //!< 実行コマンドID
  CCP_CmdRet cmd_ret;   //!< 実行結果
} CDIS_ExecInfo;

/**
 * @struct CommandDispatcher
 * @brief  CommandDispatcher の Info 構造体
 */
typedef struct
{
  uint8_t idx;              //!< CDIS のインデックス. EL で用いる
  CDIS_ExecInfo prev;       //!< 前回のコマンド実行情報
  CDIS_ExecInfo prev_err;   //!< 最後にエラーが出たコマンド実行情報
  uint32_t error_counter;   //!< エラーカウンタ
  int lockout;              //!< 実行中断フラグ
  int stop_on_error;        //!< 異常時実行中断フラグ
  PacketList* pl;           //!< コマンドキュー
} CommandDispatcher;


/**
 * @brief  CDIS の初期化と取得
 * @note   各種 CommandDispatcher App (Realtime Command Dispatcher とか) でこれを呼び，使う
 * @param[in] pl: この CDIS と紐付けるコマンドキューの PacketList
 * @return 初期化された CommandDispatcher
 */
CommandDispatcher CDIS_init(PacketList* pl);

/**
 * @brief  CDIS に登録されているコマンドキューからコマンドを実行
 * @param[in,out] cdis: 参照する CDIS
 * @return void
 */
void CDIS_dispatch_command(CommandDispatcher* cdis);

/**
 * @brief  CDIS に登録されているコマンドキューからコマンドを削除
 * @param[in,out] cdis: 参照する CDIS
 * @return void
 */
void CDIS_clear_command_list(CommandDispatcher* cdis);

/**
 * @brief  CDIS に登録されているコマンドキューのコマンド実行エラー情報を削除
 * @param[in,out] cdis: 参照する CDIS
 * @return void
 */
void CDIS_clear_error_status(CommandDispatcher* cdis);

#endif
