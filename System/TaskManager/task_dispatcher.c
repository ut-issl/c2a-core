#pragma section REPRO
/**
 * @file
 * @brief task list に設定されたタスクを実行する主体
 * @note  これは， RT OS のタスク時分割処理に相当する
 *        したがって，これによって， 1 cycle を step 刻みで各 task に振り分けて実行していくことになる
 */
#include "task_dispatcher.h"

#include <stdlib.h>
#include <string.h> // for memcpy

#include "../../TlmCmd/packet_list_util.h"
#include "../../TlmCmd/block_command_executor.h"
#include "../../TlmCmd/common_cmd_packet_util.h"
#include "../ModeManager/mode_manager.h"
#include "../TimeManager/time_manager.h"
#include "../EventManager/event_logger.h"
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>

#define TDSP_TASK_MAX BCT_MAX_CMD_NUM

static TaskDsipatcher task_dispathcer_;
const TaskDsipatcher* const task_dispathcer = &task_dispathcer_;

// 現在のサイクルで実行すべきタスク一覧を保存したもの
static PacketList TDSP_task_list_;

/**
 * @brief `task_dispathcer_.task_list_id` に登録された BC を TaskListに展開.
 * @note  実際に BC の内容を読み込み、展開しているのは `PL_ACK PL_deploy_block_cmd(args)`
 */
static void TDSP_deploy_block_as_task_list_(void);

// debug_apps にあるべき & 今はつわかないので無効化
#if 0
#include "../../Library/print.h"
#include <src_user/Library/vt100.h>
/**
 * @brief デバッグ情報表示
 */
static void TDSP_print_tdsp_status_(void);
#endif


void TDSP_initialize(void)
{
  static PL_Node task_stock_[TDSP_TASK_MAX];
  static CommonCmdPacket packet_stock_[TDSP_TASK_MAX];
  PL_initialize_with_ccp(task_stock_, packet_stock_, TDSP_TASK_MAX, &TDSP_task_list_);

  // タスクリストを初期化し、INITIALモードのブロックコマンドを展開する
  task_dispathcer_.tskd = CDIS_init(&TDSP_task_list_);
  task_dispathcer_.task_list_id = MM_get_tasklist_id_of_mode(MD_MODEID_START_UP);
  TDSP_deploy_block_as_task_list_();
  task_dispathcer_.activated_at = 0;
}


TDSP_ACK TDSP_set_task_list_id(bct_id_t id)
{
  if (id >= BCT_MAX_BLOCKS) return TDSP_INVAILD_BCT_ID;
  if (!BCE_is_active(id)) return TDSP_INACTIVE_BCT_ID;
  if (BCT_get_bc_length(id) == 0) return TDSP_EMPTY_BC;

  task_dispathcer_.task_list_id = id;
  return TDSP_SUCCESS;
}


static void TDSP_deploy_block_as_task_list_(void)
{
  PL_ACK ack;

  // 本関数内の処理中にMaster Cycleが変化した場合を検出できるよう、
  // まず次Master Cycleの情報を更新する。
  task_dispathcer_.activated_at = TMGR_get_master_total_cycle() + 1;

  ack = PL_deploy_block_cmd(&TDSP_task_list_, task_dispathcer_.task_list_id, 0);

  if (ack != PL_SUCCESS)
  {
    EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                    TDSP_DEPLOY_FAILED,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)ack);
  }
}


void TDSP_execute_pl_as_task_list(void)
{
  // まずは (1)ブロックコマンドがタスクリストに展開された時のサイクル数 と (2)現在のサイクル数 を比較
  // (1) = (2) の時は展開から 1サイクル (100ms) 以内なのでタスクリストに登録されたコマンドを一つ実行する
  // (1) > (2) の時は今回のサイクルで実行すべきコマンドがすべて実行済みである
  // (1) < (2) の時は展開から 1サイクル (100ms) 以上経過してしまっているので強制終了する

  if (task_dispathcer_.activated_at == TMGR_get_master_total_cycle())
  {
    // タスクリストの先頭コマンド実行予定時刻と現在時刻を比較
    PL_ACK ack = PL_check_tl_cmd(&TDSP_task_list_,
                                 (size_t)(TMGR_get_master_step()));

    switch (ack)
    {
    case PL_TLC_PAST_TIME:
      // 実行時刻が過ぎていた場合は実行前にアノマリを登録。
      EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                      TDSP_STEP_OVERRUN,
                      EL_ERROR_LEVEL_LOW,
                      (uint32_t)CCP_get_ti( (const CommonCmdPacket*)(PL_get_head(&TDSP_task_list_)->packet) ));

      // FALL THROUGH

    case PL_TLC_ON_TIME:
      // 実行時刻が過ぎている、もしくは実行時刻ピッタリの場合はコマンドを実行
      CDIS_dispatch_command(&(task_dispathcer_.tskd));

      if (task_dispathcer_.tskd.prev.cmd_ret.exec_sts != CCP_EXEC_SUCCESS)
      {
        // コマンド実行時に異常が発生した場合はアノマリを登録。
        EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                      TDSP_TASK_EXEC_FAILED,
                      EL_ERROR_LEVEL_HIGH,
                      task_dispathcer_.tskd.prev.cmd_ret.exec_sts);
      }

      break;

    case PL_TLC_NOT_YET:
      // case名が不適切だが、ここに来るのは以下の二つの場合
      // ・タスクリストが空
      // ・次のコマンドの実行時刻がまだ

      if (PL_count_active_nodes(&TDSP_task_list_) == 0)
      {
        // task_listが空なら再度タスクリストを展開
        // これを行うとtask_dispathcer_.activated_atがインクリメントされるので、"次のサイクル実行待ち状態"になる
        TDSP_deploy_block_as_task_list_();
      }

      break;

    default:
      // 基本ここには来ない
      EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                      TDSP_UNKNOWN,
                      EL_ERROR_LEVEL_HIGH,
                      (uint32_t)ack);
    }
  }
  else if (task_dispathcer_.activated_at > TMGR_get_master_total_cycle())
  {
    // 次のサイクルの実行待ち状態
    return;
  }
  else if (task_dispathcer_.activated_at < TMGR_get_master_total_cycle())
  {
    if ((task_dispathcer_.activated_at == 0) && (TMGR_get_master_total_cycle() == OBCT_MAX_CYCLE - 1))
    {
      // 次サイクルの実行待ち状態(サイクルオーバーフロー直前)
      // 本来は task_dispathcer_.activated_at > TMGR_get_master_total_cycle() となるはずが、
      // task_dispathcer_.activated_atがあふれて0に戻っている場合ここに来る
      return;
    }
    else
    {
      // 1サイクル以内に全てのコマンドを実行し終えなかった場合ここに来る
      EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                      TDSP_CYCLE_OVERRUN,
                      EL_ERROR_LEVEL_HIGH,
                      0);

      // リストをクリア->再展開し次サイクルから再実行
      PL_clear_list(&TDSP_task_list_);
      TDSP_deploy_block_as_task_list_();
    }
  }
}


void TDSP_resync_internal_counter(void)
{
  task_dispathcer_.activated_at = TMGR_get_master_total_cycle();
}


CCP_CmdRet Cmd_TDSP_SET_TASK_LIST(const CommonCmdPacket* packet)
{
  // FIXME: u8 でいいのか？ まあ，いい気もする．
  TDSP_ACK ack = TDSP_set_task_list_id((bct_id_t)(CCP_get_param_head(packet)[0]));

  switch (ack)
  {
  case TDSP_SUCCESS:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case TDSP_INVAILD_BCT_ID:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  case TDSP_INACTIVE_BCT_ID:
  case TDSP_EMPTY_BC:
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }
}


// debug_apps にあるべき & 今はつわかないので無効化
#if 0
AppInfo TDSP_print_tdsp_status(void)
{
  return AI_create_app_info("tstm", NULL, TDSP_print_tdsp_status_);
}


void TDSP_print_tdsp_status_(void)
{
  VT100_erase_line();
  Printf("TASK: BC %d, ERR (TOTAL, STEP, STS, CODE) = (%10u, %3u, %d, %d)\n",
         task_dispathcer->task_list_id,
         task_dispathcer->tskd.prev_err.time.total_cycle,
         task_dispathcer->tskd.prev_err.time.step,
         task_dispathcer->tskd.prev_err.cmd_ret.exec_sts,
         task_dispathcer->tskd.prev_err.cmd_ret.err_code);
}
#endif

#pragma section
