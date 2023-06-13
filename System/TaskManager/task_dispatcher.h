/**
 * @file
 * @brief task list に設定されたタスクを実行する主体
 * @note  これは， RT OS のタスク時分割処理に相当する
 *        したがって，これによって， 1 cycle を step 刻みで各 task に振り分けて実行していくことになる
 */
#ifndef TASK_DISPATCHER_H_
#define TASK_DISPATCHER_H_

#include "../../TlmCmd/block_command_table.h"
#include "../../TlmCmd/command_dispatcher.h"
#include "../ApplicationManager/app_info.h"

/**
 * @struct TaskDsipatcher
 * @brief TaskDispatcherの状態
 */
typedef struct
{
  CommandDispatcher tskd;     //!< タスクリストへのポインタと、その他実行情報を保存する構造体
  bct_id_t task_list_id;      //!< タスクリストに展開するブロックコマンドのID
  cycle_t  activated_at;      //!< ブロックコマンドがタスクリストに展開された時のサイクル数
} TaskDsipatcher;

/**
 * @enum  TDSP_ACK
 * @brief TDSPのエラーステータス
 * @note  uint8_t
 */
typedef enum
{
  TDSP_SUCCESS,
  TDSP_DEPLOY_FAILED,    //!< ブロックコマンドのタスクリストへの展開時にエラーが発生
  TDSP_CYCLE_OVERRUN,    //!< 1サイクル以内にすべてのコマンドを実行し終えなかった
  TDSP_STEP_OVERRUN,     //!< コマンドが実行予定のステップ数を過ぎている
  TDSP_TASK_EXEC_FAILED, //!< コマンド実行時にエラーが発生
  TDSP_INVAILD_BCT_ID,   //!< 実行する BCT_ID が無効だった
  TDSP_INACTIVE_BCT_ID,  //!< 実行する BCT_ID が無効化されている
  TDSP_EMPTY_BC,         //!< 実行する BC が空だった
  TDSP_UNKNOWN
} TDSP_ACK;

extern const TaskDsipatcher* const task_dispatcher;

/**
 * @brief タスク管理を行うTaskDsipatcher構造体(TaskDsipatcher_)の初期化
 * @note  データ構造は Packet List
 */
void TDSP_initialize(void);

/**
 * @brief 指定した BC を TaskList に展開するものとして登録
 * @param[in] id: BC の id
 * @note 展開自体は TDSP_deploy_block_as_task_list_ で行う
 *       "BC を TaskList に展開" とは `src_user\Settings\Modes\TaskLists\tl_initial.c` などを参照.
 */
TDSP_ACK TDSP_set_task_list_id(bct_id_t id);

/**
 * @brief 現在時刻に応じて、TaskList から Cmd を一つ実行する or TaskList を再展開する
 *
 *        展開されているタスクリストの実行時刻 (cycle レベル) を比較し, task_list_ に登録されているタスクを順番に実行する．
 *        1つタスクを消化すると return する． (while(1) で回っているのですぐ戻ってくる．)
 *        実行 cycle が現在だった場合, 各タスクの step によって実行する, しないを switch に合わせて処理する．
 *        実際にタスクを処理する場合, CDIS_dispatch_command -> PH_dispatch_command -> CA_execute_cmd の順に実行される(真の Executer は CA_execute_cmd)．
 */
void TDSP_execute_pl_as_task_list(void);

/**
 * @brief タスクリストの展開時サイクル数を現在のサイクル数に強制的に合わせる
 */
void TDSP_resync_internal_counter(void);

/**
 * @brief 指定したブロックコマンドを、次にタスクリストに展開するものとして登録するコマンド
 */
CCP_CmdRet Cmd_TDSP_SET_TASK_LIST(const CommonCmdPacket* packet);

// debug_apps にあるべき & 今はつわかないので無効化
#if 0
AppInfo TDSP_print_tdsp_status(void);
#endif

#endif
