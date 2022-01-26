#pragma section REPRO
#include "task_dispatcher.h"

#include <stdlib.h>
#include <string.h> // for memcpy

#include "../../TlmCmd/packet_list_util.h"
#include "../../TlmCmd/block_command_executor.h"
#include "../ModeManager/mode_manager.h"
#include "../TimeManager/time_manager.h"
#include "../AnomalyLogger/anomaly_logger.h"
#include "../EventManager/event_logger.h"
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include "../../Library/print.h"
#include <src_user/Library/VT100.h>

static TDSP_Info TDSP_info_;
const TDSP_Info* const TDSP_info = &TDSP_info_;

// 現在のサイクルで実行すべきタスク一覧を保存したもの
static PacketList task_list_;

/**
 * @brief `TDSP_info_.task_list_id` に登録された BC を TaskListに展開.
 * @note  実際に BC の内容を読み込み、展開しているのは `PL_ACK PL_deploy_block_cmd(args)`
 */
static void TDSP_deploy_block_as_task_list_(void);

/**
 * @brief デバッグ情報表示
 */
static void print_tdsp_status_(void);

void TDSP_initialize(void)
{
  static PL_Node task_stock_[TDSP_TASK_MAX];
  static CommonCmdPacket packet_stock_[TDSP_TASK_MAX];
  PL_initialize_with_ccp(task_stock_, packet_stock_, TDSP_TASK_MAX, &task_list_);

  // タスクリストを初期化し、INITIALモードのブロックコマンドを展開する
  TDSP_info_.tskd = CDIS_init(&task_list_);
  TDSP_info_.task_list_id = MM_get_tasklist_id_of_mode(MD_MODEID_START_UP);
  TDSP_deploy_block_as_task_list_();
  TDSP_info_.activated_at = 0;
}

TDSP_ACK TDSP_set_task_list_id(bct_id_t id)
{
  if (id >= BCT_MAX_BLOCKS) return TDSP_INVAILD_BCT_ID;
  if (!BCE_is_active(id)) return TDSP_INACTIVE_BCT_ID;
  if (BCT_get_bc_length(id) == 0) return TDSP_EMPTY_BC;

  TDSP_info_.task_list_id = id;
  return TDSP_SUCCESS;
}

static void TDSP_deploy_block_as_task_list_(void)
{
  PL_ACK ack;

  // 本関数内の処理中にMaster Cycleが変化した場合を検出できるよう、
  // まず次Master Cycleの情報を更新する。
  TDSP_info_.activated_at = TMGR_get_master_total_cycle() + 1;

  ack = PL_deploy_block_cmd(&task_list_, TDSP_info_.task_list_id, 0);

  if (ack != PL_SUCCESS)
  {
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_TASK_DISPATCHER, TDSP_DEPLOY_FAILED);
#endif
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

  if (TDSP_info_.activated_at == TMGR_get_master_total_cycle())
  {
    // タスクリストの先頭コマンド実行予定時刻と現在時刻を比較
    PL_ACK ack = PL_check_tl_cmd(&task_list_,
                                 (size_t)(TMGR_get_master_step()));

    switch (ack)
    {
    case PL_TLC_PAST_TIME:
      // 実行時刻が過ぎていた場合は実行前にアノマリを登録。
#ifndef AL_DISALBE_AT_C2A_CORE
      AL_add_anomaly(AL_CORE_GROUP_TASK_DISPATCHER, TDSP_STEP_OVERRUN);
#endif
      EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                      TDSP_STEP_OVERRUN,
                      EL_ERROR_LEVEL_LOW,
                      (uint32_t)CCP_get_ti( (const CommonCmdPacket*)(PL_get_head(&task_list_)->packet) ));

      // FALL THROUGH

    case PL_TLC_ON_TIME:
      // 実行時刻が過ぎている、もしくは実行時刻ピッタリの場合はコマンドを実行
      CDIS_dispatch_command(&(TDSP_info_.tskd));

      if (TDSP_info_.tskd.prev.sts != CCP_EXEC_SUCCESS)
      {
        // コマンド実行時に異常が発生した場合はアノマリを登録。
#ifndef AL_DISALBE_AT_C2A_CORE
        AL_add_anomaly(AL_CORE_GROUP_TASK_DISPATCHER, TDSP_TASK_EXEC_FAILED);
#endif
        EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                      TDSP_TASK_EXEC_FAILED,
                      EL_ERROR_LEVEL_HIGH,
                      TDSP_info_.tskd.prev.sts);
      }

      break;

    case PL_TLC_NOT_YET:
      // case名が不適切だが、ここに来るのは以下の二つの場合
      // ・タスクリストが空
      // ・次のコマンドの実行時刻がまだ

      if (PL_count_active_nodes(&task_list_) == 0)
      {
        // task_listが空なら再度タスクリストを展開
        // これを行うとTDSP_info_.activated_atがインクリメントされるので、"次のサイクル実行待ち状態"になる
        TDSP_deploy_block_as_task_list_();
      }

      break;

    default:
      // 基本ここには来ない
#ifndef AL_DISALBE_AT_C2A_CORE
      AL_add_anomaly(AL_CORE_GROUP_TASK_DISPATCHER, TDSP_UNKNOWN);
#endif
      EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                      TDSP_UNKNOWN,
                      EL_ERROR_LEVEL_HIGH,
                      (uint32_t)ack);
    }
  }
  else if (TDSP_info_.activated_at > TMGR_get_master_total_cycle())
  {
    // 次のサイクルの実行待ち状態
    return;
  }
  else if (TDSP_info_.activated_at < TMGR_get_master_total_cycle())
  {
    if ((TDSP_info_.activated_at == 0) && (TMGR_get_master_total_cycle() == OBCT_MAX_CYCLE - 1))
    {
      // 次サイクルの実行待ち状態(サイクルオーバーフロー直前)
      // 本来は TDSP_info_.activated_at > TMGR_get_master_total_cycle() となるはずが、
      // TDSP_info_.activated_atがあふれて0に戻っている場合ここに来る
      return;
    }
    else
    {
      // 1サイクル以内に全てのコマンドを実行し終えなかった場合ここに来る
#ifndef AL_DISALBE_AT_C2A_CORE
      AL_add_anomaly(AL_CORE_GROUP_TASK_DISPATCHER, TDSP_CYCLE_OVERRUN);
#endif
      EL_record_event((EL_GROUP)EL_CORE_GROUP_TASK_DISPATCHER,
                      TDSP_CYCLE_OVERRUN,
                      EL_ERROR_LEVEL_HIGH,
                      0);

      // リストをクリア->再展開し次サイクルから再実行
      PL_clear_list(&task_list_);
      TDSP_deploy_block_as_task_list_();
    }
  }
}

void TDSP_resync_internal_counter(void)
{
  TDSP_info_.activated_at = TMGR_get_master_total_cycle();
}

CCP_EXEC_STS Cmd_TDSP_SET_TASK_LIST(const CommonCmdPacket* packet)
{
  // FIXME: u8 でいいのか？ まあ，いい気もする．
  TDSP_ACK ack = TDSP_set_task_list_id((bct_id_t)(CCP_get_param_head(packet)[0]));

  switch (ack)
  {
  case TDSP_SUCCESS:
    return CCP_EXEC_SUCCESS;
  case TDSP_INVAILD_BCT_ID:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  case TDSP_INACTIVE_BCT_ID:
  case TDSP_EMPTY_BC:
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}

AppInfo print_tdsp_status(void)
{
  return AI_create_app_info("tstm", NULL, print_tdsp_status_);
}

void print_tdsp_status_(void)
{
  VT100_erase_line();
  Printf("TASK: BC %d, ERR (TOTAL, STEP, STS) = (%10u, %3u, %d)\n",
         TDSP_info->task_list_id,
         TDSP_info->tskd.prev_err.time.total_cycle,
         TDSP_info->tskd.prev_err.time.step,
         TDSP_info->tskd.prev_err.sts);
}

#pragma section
