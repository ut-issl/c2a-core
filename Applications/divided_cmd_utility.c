#pragma section REPRO
/**
 * @file
 * @brief コマンド分割をサポートするUtil
 * @note  HOW TO USE
 *        1. 各Cmdにて初めに DCU_check_in を実行し，現在の実行状況を取得する
 *        2. 実行が終了し 再度再帰させる必要がある場合は DCU_register_next で登録する
 *        3. すべての処理が終了した場合は DCU_report_finish を実行する
 *        4. 実行中のエラーは DCU_report_err で報告する．すると中断される
 *        5. 中断したいときは DCU_abort_cmd を実行する
 *        6. 中断された状態をリセットする場合は DCU_donw_abort_flag を実行する
 * @note  telemetry_manager などで使っているので，それを参照のこと
 */
#include "divided_cmd_utility.h"
#include "../CmdTlm/packet_handler.h"
#include "../CmdTlm/common_tlm_cmd_packet_util.h"
#include "../System/TimeManager/time_manager.h"
#include "../System/EventManager/event_logger.h"

/**
 * @brief  App初期化関数
 * @param  void
 * @return void
 */
static void DCU_init_(void);

/**
 * @brief  ログクリア
 * @param  void
 * @return void
 */
static void DCU_clear_log_(void);

/**
 * @brief  ログクリア（１要素）
 * @param[in]  log_idx:  消すログ
 * @param[in]  cmd_code: 設定するコマンドID
 * @return void
 */
static void DCU_clear_log_element_(uint8_t log_idx, CMD_CODE cmd_code);

/**
 * @brief  該当コマンドのログを検索し取得する
 * @param[in]  cmd_code: 探索するコマンド
 * @param[out] log_idx:  exec_logs におけるログの場所
 * @param[out] sort_key: exec_log_order におけるログの場所
 * @retval DCU_LOG_ACK_OK:        見つかった
 * @retval DCU_LOG_ACK_NOT_FOUND: 見つからず
 */
static DCU_LOG_ACK DCU_search_log_(CMD_CODE cmd_code, uint8_t* log_idx, uint8_t* sort_key);

/**
 * @brief  該当コマンドのログを，ログの先頭に出す
 * @note   ログがなければ insert する
 * @param[in]  cmd_code: 先頭に出すコマンド
 * @return void
 */
static void DCU_move_to_front_in_log_(CMD_CODE cmd_code);

/**
 * @brief  先頭に該当コマンドのログをつくる
 * @note   最も古いログが破棄されることに注意
 * @note   すでにログがあった場合もチェックせずに insert する．すでにチェックされていることを前提とする！！！
 * @param[in]  cmd_code: 先頭に作るコマンド
 * @return void
 */
static void DCU_create_log_on_front_(CMD_CODE cmd_code);


static DividedCmdUtility divided_cmd_utility_;
const DividedCmdUtility* const divided_cmd_utility = &divided_cmd_utility_;

static CTCP DCU_packet_;


AppInfo DCU_create_app(void)
{
  return create_app_info("divided_cmd_utility", DCU_init_, NULL);
}


static void DCU_init_(void)
{
  DCU_clear_log_();
}


static void DCU_clear_log_(void)
{
  uint8_t i;
  for (i = 0; i < DCU_LOG_MAX; ++i)
  {
    DCU_clear_log_element_(i, Cmd_CODE_MAX);
    divided_cmd_utility_.exec_log_order[i] = i;
  }
}


static void DCU_clear_log_element_(uint8_t log_idx, CMD_CODE cmd_code)
{
  divided_cmd_utility_.exec_logs[log_idx].cmd_code = cmd_code;
  divided_cmd_utility_.exec_logs[log_idx].status = DCU_STATUS_FINISHED;
  divided_cmd_utility_.exec_logs[log_idx].exec_counter = 0;
  divided_cmd_utility_.exec_logs[log_idx].last_exec_sts = CCP_EXEC_SUCCESS;
  divided_cmd_utility_.exec_logs[log_idx].last_exec_time = OBCT_create(0, 0, 0);
}


static DCU_LOG_ACK DCU_search_log_(CMD_CODE cmd_code, uint8_t* log_idx, uint8_t* sort_key)
{
  uint8_t i;
  *log_idx = 0;
  *sort_key = 0;

  for (i = 0; i < DCU_LOG_MAX; ++i)
  {
    uint8_t idx = divided_cmd_utility_.exec_log_order[i];
    if (divided_cmd_utility_.exec_logs[idx].cmd_code == cmd_code)
    {
      *log_idx  = idx;
      *sort_key = i;
      return DCU_LOG_ACK_OK;
    }
  }

  return DCU_LOG_ACK_NOT_FOUND;
}


static void DCU_move_to_front_in_log_(CMD_CODE cmd_code)
{
  uint8_t i;
  uint8_t log_idx;
  uint8_t sort_key;
  DCU_LOG_ACK log_ret = DCU_search_log_(cmd_code, &log_idx, &sort_key);

  if (log_ret == DCU_LOG_ACK_NOT_FOUND)
  {
    // ログがないので作る
    DCU_create_log_on_front_(cmd_code);
    return;
  }

  for (i = sort_key; i > 0; --i)
  {
    divided_cmd_utility_.exec_log_order[i] = divided_cmd_utility_.exec_log_order[i - 1];
  }
  divided_cmd_utility_.exec_log_order[0] = log_idx;
}


static void DCU_create_log_on_front_(CMD_CODE cmd_code)
{
  uint8_t i;
  uint8_t idx;

  // 最も古いログを取得
  idx = divided_cmd_utility_.exec_log_order[DCU_LOG_MAX - 1];
  if (divided_cmd_utility_.exec_logs[idx].status == DCU_STATUS_PROGRESS)
  {
    EL_record_event((EL_GROUP)EL_CORE_GROUP_DCU,
                    (uint32_t)divided_cmd_utility_.exec_logs[idx].cmd_code,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)divided_cmd_utility_.exec_logs[idx].exec_counter);
  }

  for (i = (DCU_LOG_MAX - 1); i > 0; --i)
  {
    divided_cmd_utility_.exec_log_order[i] = divided_cmd_utility_.exec_log_order[i - 1];
  }
  divided_cmd_utility_.exec_log_order[0] = idx;

  DCU_clear_log_element_(idx, cmd_code);
}


DCU_STATUS DCU_check_in(CMD_CODE cmd_code, uint16_t* exec_counter)
{
  uint8_t idx;
  DCU_STATUS status;

  DCU_move_to_front_in_log_(cmd_code);
  idx = divided_cmd_utility_.exec_log_order[0];  // 自身のログidx，つまり最も新しいログidxを拾う
  status = divided_cmd_utility_.exec_logs[idx].status;

  if (status == DCU_STATUS_FINISHED)
  {
    DCU_clear_log_element_(idx, cmd_code);
  }
  else if (status == DCU_STATUS_PROGRESS)
  {
    divided_cmd_utility_.exec_logs[idx].exec_counter++;
  }
  else
  {
    // なにもしない
  }

  divided_cmd_utility_.exec_logs[idx].last_exec_time = TMGR_get_master_clock();
  *exec_counter = divided_cmd_utility_.exec_logs[idx].exec_counter;
  return status;
}


DCU_ACK DCU_register_next(CMD_CODE cmd_code, const uint8_t* param, uint16_t len)
{
  uint8_t idx;
  CTCP_UTIL_ACK ret;

  DCU_move_to_front_in_log_(cmd_code);
  idx = divided_cmd_utility_.exec_log_order[0];
  divided_cmd_utility_.exec_logs[idx].status = DCU_STATUS_PROGRESS;

  ret = CCP_form_rtc(&DCU_packet_, cmd_code, param, len);
  if (ret != CTCP_UTIL_ACK_OK) return DCU_ACK_ERR;
  if (PH_analyze_packet(&DCU_packet_) != PH_REGISTERED)
  {
    return DCU_ACK_ERR;
  }

  return DCU_ACK_OK;
}


void DCU_report_finish(CMD_CODE cmd_code, CCP_EXEC_STS last_exec_sts)
{
  uint8_t idx;

  DCU_move_to_front_in_log_(cmd_code);
  idx = divided_cmd_utility_.exec_log_order[0];

  divided_cmd_utility_.exec_logs[idx].status = DCU_STATUS_FINISHED;
  divided_cmd_utility_.exec_logs[idx].last_exec_sts = last_exec_sts;
}


void DCU_report_err(CMD_CODE cmd_code, CCP_EXEC_STS last_exec_sts)
{
  uint8_t idx;

  DCU_move_to_front_in_log_(cmd_code);
  idx = divided_cmd_utility_.exec_log_order[0];

  divided_cmd_utility_.exec_logs[idx].status = DCU_STATUS_ABORTED_BY_ERR;
  divided_cmd_utility_.exec_logs[idx].last_exec_sts = last_exec_sts;
}


void DCU_donw_abort_flag(CMD_CODE cmd_code)
{
  uint8_t idx;

  DCU_move_to_front_in_log_(cmd_code);
  idx = divided_cmd_utility_.exec_log_order[0];

  DCU_clear_log_element_(idx, cmd_code);
}


void DCU_abort_cmd(CMD_CODE cmd_code)
{
  uint8_t idx;

  DCU_move_to_front_in_log_(cmd_code);
  idx = divided_cmd_utility_.exec_log_order[0];

  divided_cmd_utility_.exec_logs[idx].status = DCU_STATUS_ABORTED_BY_CMD;
}


DCU_LOG_ACK DCU_search_and_get_log(CMD_CODE cmd_code, const DCU_ExecStatus* exec_log)
{
  uint8_t log_idx;
  uint8_t sort_key;
  DCU_LOG_ACK log_ret = DCU_search_log_(cmd_code, &log_idx, &sort_key);

  if (log_ret == DCU_LOG_ACK_NOT_FOUND)
  {
    return DCU_LOG_ACK_NOT_FOUND;
  }

  exec_log = &divided_cmd_utility_.exec_logs[log_idx];
  return DCU_LOG_ACK_OK;
}


CCP_EXEC_STS Cmd_DCU_ABORT_CMD(const CTCP* packet)
{
  // CMD_CODE は u16 と想定する
  CMD_CODE target_cmd = (CMD_CODE)CCP_get_param_from_packet(packet, 0, uint16_t);

  DCU_abort_cmd(target_cmd);

  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_DCU_DOWN_ABORT_FLAG(const CTCP* packet)
{
  // CMD_CODE は u16 と想定する
  CMD_CODE target_cmd = (CMD_CODE)CCP_get_param_from_packet(packet, 0, uint16_t);

  DCU_donw_abort_flag(target_cmd);

  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_DCU_CLEAR_LOG(const CTCP* packet)
{
  (void)packet;

  DCU_clear_log_();

  return CCP_EXEC_SUCCESS;
}

#pragma section
