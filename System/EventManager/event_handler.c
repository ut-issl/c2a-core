#pragma section REPRO
/**
 * @file  event_handler.c
 * @brief イベント発火型処理を行う
 * @note  event_logger の情報を元に BC を展開する ( Event 発火に応じて対応を行う )
 */
#include "event_handler.h"
#include <string.h>
#include "../../CmdTlm/common_tlm_cmd_packet_util.h"
#include "../../Applications/timeline_command_dispatcher.h"
#include "../TimeManager/time_manager.h"
#include "../../Library/endian_memcpy.h"

#ifdef EL_IS_ENABLE_TLOG

/**
 * @enum   EH_EL_LOCAL_ID
 * @brief  EL_CORE_GROUP_EVENT_HANDLER イベントの local id
 * @note   uint8_t を想定
 */
typedef enum
{
  EH_EL_LOCAL_ID_EL_TOTAL_COUNTER_ERR = 0,  //!< ELとEHのカウンタの不整合エラー (counter_total)
  EH_EL_LOCAL_ID_EL_COUNTER_ERR,            //!< ELとEHのカウンタの不整合エラー (counters)
  EH_EL_LOCAL_ID_TOO_MANY_EVENT,            //!< イベントが発生しすぎて，TLogが失われている
  EH_EL_LOCAL_ID_FAIL_FORM_CTCP,            //!< BC 展開 Cmd の生成に失敗
  EH_EL_LOCAL_ID_LOG_TABLE_FULL,            //!< EH_LogTable が満杯になり， wp が 0 に戻った
  EH_EL_LOCAL_ID_SEARCH_ERR,                //!< EH_search_rule_table_index_ の返り値不正
  EH_EL_LOCAL_ID_UNKNOWN_ERR                //!< 不明なエラー
} EH_EL_LOCAL_ID;

/**
 * @enum   EH_ACK
 * @brief  EH の内部関数返り値
 * @note   uint8_t を想定
 */
typedef enum
{
  EH_ACK_OK,
  EH_ACK_ERR
} EH_ACK;

/**
 * @enum   EH_RULE_SORTED_INDEX_ACK
 * @brief  EH_RuleSortedIndex を操作するときの返り値
 * @note   uint8_t を想定
 */
typedef enum
{
  EH_RULE_SORTED_INDEX_ACK_OK = 0,           //!< 正常終了
  EH_RULE_SORTED_INDEX_ACK_ILLEGAL_RULE_ID,  //!< 不正な EH_RULE_ID
  EH_RULE_SORTED_INDEX_ACK_NOT_FOUND,        //!< 見つからず
  EH_RULE_SORTED_INDEX_ACK_FULL,             //!< これ以上登録できない
  EH_RULE_SORTED_INDEX_ACK_RULE_OVERWRITE,   //!< ルールの上書きになってしまう（すでに同じ ID にルールが登録されているため棄却）
  EH_RULE_SORTED_INDEX_ACK_DUPLICATE_FULL,   //!< 重複上限まで重複してしまっている
  EH_RULE_SORTED_INDEX_ACK_UNKNOWN_ERR       //!< 不明なエラー
} EH_RULE_SORTED_INDEX_ACK;

/**
 * @enum   EH_CKECK_AND_RESPOND_ACK
 * @brief  EH_check_{hoge}_and_respond_ の返り値
 * @note   uint8_t を想定
 */
typedef enum
{
  EH_CKECK_AND_RESPOND_ACK_RESPONDED = 0,         //!< EH として対応した
  EH_CKECK_AND_RESPOND_ACK_NOT_RESPONDED,         //!< 対応する EH_Rule なし or 対応条件満たさず
  EH_CKECK_AND_RESPOND_ACK_NO_UNPROCESSED_EVENT,  //!< 未処理な EL_Event なし
  EH_CKECK_AND_RESPOND_ACK_UNKNOWN_ERR            //!< 不明なエラー
} EH_CKECK_AND_RESPOND_ACK;


/**
 * @brief  EH_RuleTable を全消去
 * @param  void
 * @return void
 */
static void EH_clear_rules_(void);

/**
 * @brief  EH 対応ログを全消去
 * @param  void
 * @return void
 */
static void EH_clear_log_(void);

/**
 * @brief  EL 側の EL_Event のカウンタをチェックし，整合性を確認する
 * @param  void
 * @retval EH_ACK_OK  : 正常終了
 * @retval EH_ACK_ERR : EL と EH のカウンタの不整合が発生し， EH カウンタリセット
 */
static EH_ACK EH_check_el_event_counter_(void);

/**
 * @brief  最も古い EL_Event を１つ取得し， EL_Event に対応する EH_Rule が存在するかチェックし，対応する
 * @param  responded_num[out] 対応した EH_Rule 数
 * @return EH_CKECK_AND_RESPOND_ACK
 */
static EH_CKECK_AND_RESPOND_ACK EH_check_event_and_respond_(uint8_t* responded_num);

/**
 * @brief  EH の対応条件をチェックし， EL_Event に対応する
 * @note   引数はアサーション済みを仮定する
 * @param  rule_id[in] 対応条件をチェックし，対応する EH_RULE_ID
 * @param  event[in]   発生した EL_Event
 * @return EH_CKECK_AND_RESPOND_ACK
 */
static EH_CKECK_AND_RESPOND_ACK EH_check_rule_and_respond_(EH_RULE_ID rule_id, const EL_Event* event);

/**
 * @brief  EH の対応条件をチェックし， EL_Event に対応する (EH_RESPONSE_CONDITION_SINGLE)
 * @note   引数はアサーション済みを仮定する
 * @param  rule_id[in] 対応条件をチェックし，対応する EH_RULE_ID
 * @param  event[in]   発生した EL_Event
 * @return EH_CKECK_AND_RESPOND_ACK
 */
static EH_CKECK_AND_RESPOND_ACK EH_check_single_rule_and_respond_(EH_RULE_ID rule_id, const EL_Event* event);

/**
 * @brief  EH の対応条件をチェックし， EL_Event に対応する (EH_RESPONSE_CONDITION_CONTINUOUS)
 * @note   引数はアサーション済みを仮定する
 * @param  rule_id[in] 対応条件をチェックし，対応する EH_RULE_ID
 * @param  event[in]   発生した EL_Event
 * @return EH_CKECK_AND_RESPOND_ACK
 */
static EH_CKECK_AND_RESPOND_ACK EH_check_continuous_rule_and_respond_(EH_RULE_ID rule_id, const EL_Event* event);

/**
 * @brief  EH の対応条件をチェックし， EL_Event に対応する (EH_RESPONSE_CONDITION_CUMULATIVE)
 * @note   引数はアサーション済みを仮定する
 * @param  rule_id[in] 対応条件をチェックし，対応する EH_RULE_ID
 * @param  event[in]   発生した EL_Event
 * @return EH_CKECK_AND_RESPOND_ACK
 */
static EH_CKECK_AND_RESPOND_ACK EH_check_cumulative_rule_and_respond_(EH_RULE_ID rule_id, const EL_Event* event);

/**
 * @brief  EH 対応を実施
 * @note   引数はアサーション済みを仮定する
 * @note   is_active はこの関数ではみない（上流でチェックしてることを想定）
 * @param  rule_id[in] 対応する EH_RULE_ID
 * @return void
 */
static void EH_respond_(EH_RULE_ID rule_id);

/**
 * @brief  EH 対応のログを残す
 * @note   引数はアサーション済みを仮定する
 * @param  rule_id[in]        対応した EH_RULE_ID
 * @param  deploy_cmd_ack[in] 対応 BC 展開コマンドの実行結果
 * @return void
 */
static void EH_record_responded_log_(EH_RULE_ID rule_id, CCP_EXEC_STS deploy_cmd_ack);

/**
 * @brief  まだ処理していない最も古い EL_Event を返す
 * @note   見つからなかった場合， NULL を返す
 * @param  void
 * @return 見つけた EL_Event
 */
const EL_Event* EH_get_oldest_event_(void);

/**
 * @brief  EH_RuleSortedIndex から，目的の EL_Event の idx を検索する
 * @note   引数はアサーション済みを仮定する
 * @param[in]  group: 検索する EL_Event.group
 * @param[in]  local: 検索する EL_Event.local
 * @param[out] found_ids: 見つかった EH_RULE_ID 一覧
 * @param[out] found_sorted_idxes: 見つかった EH_RuleSortedIndex の index
 * @param[out] found_id_num: 見つかった idx の数
 * @retval EH_RULE_SORTED_INDEX_ACK_NOT_FOUND: 見つからず
 * @retval EH_RULE_SORTED_INDEX_ACK_UNKNOWN_ERR: 不明なエラー
 * @retval EH_RULE_SORTED_INDEX_ACK_OK: 正常に探索完了
 */
static EH_RULE_SORTED_INDEX_ACK EH_search_rule_table_index_(EL_GROUP group,
                                                            uint32_t local,
                                                            EH_RULE_ID found_ids[EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES],
                                                            uint16_t found_sorted_idxes[EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES],
                                                            uint8_t* found_id_num);

/**
 * @brief  EH_Rule を EH_RuleTable と EH_RuleSortedIndex に挿入する
 * @note   引数は rule に関してはアサーション済みを仮定する
 * @note   すでに登録された id に対しての上書き登録はエラー (EH_RULE_SORTED_INDEX_ACK_RULE_OVERWRITE) を返す
 * @param[in]  id: 挿入する EH_Rule の EH_RULE_ID
 * @param[in]  rule: 挿入する EH_Rule
 * @retval EH_RULE_SORTED_INDEX_ACK_FULL: ルール登録上限に到達済み
 * @retval EH_RULE_SORTED_INDEX_ACK_ILLEGAL_RULE_ID: 不正な EH_RULE_ID
 * @retval EH_RULE_SORTED_INDEX_ACK_RULE_OVERWRITE: すでに同じ ID にルールが登録されているため棄却
 * @retval EH_RULE_SORTED_INDEX_ACK_DUPLICATE_FULL: 重複上限まで重複してしまっている
 * @retval EH_RULE_SORTED_INDEX_ACK_OK: 正常に挿入完了
 */
static EH_RULE_SORTED_INDEX_ACK EH_insert_rule_table_(EH_RULE_ID id, const EH_Rule* rule);

/**
 * @brief  EH_Rule を EH_RuleTable と EH_RuleSortedIndex から削除する
 * @param[in]  id: 削除する EH_Rule の EH_RULE_ID
 * @retval EH_RULE_SORTED_INDEX_ACK_ILLEGAL_RULE_ID: 不正な EH_RULE_ID
 * @retval EH_RULE_SORTED_INDEX_ACK_NOT_FOUND: 削除対象が見つからず or 登録されてない
 * @retval EH_RULE_SORTED_INDEX_ACK_OK: 正常に削除完了
 */
static EH_RULE_SORTED_INDEX_ACK EH_delete_rule_table_(EH_RULE_ID id);

/**
 * @brief  EH_RULE_ID のルール登録状況を調べる
 * @param[in] id: 調べる EH_RULE_ID
 * @return EH_CHECK_RULE_ACK
 */
static EH_CHECK_RULE_ACK EH_check_rule_id_(EH_RULE_ID id);


static EventHandler event_handler_;
const EventHandler* const event_handler = &event_handler_;


void EH_initialize(void)
{
  memset(&event_handler_, 0x00, sizeof(EventHandler));

  EH_clear_rules_();
  EH_clear_log_();

  EH_match_event_counter_to_el();
  event_handler_.max_response_num = EH_MAX_RESPONSE_NUM_DEFAULT;
  event_handler_.max_check_event_num = EH_MAX_CHECK_EVENT_NUM_DEFAULT;

  EH_load_default_rules();
}


static void EH_clear_rules_(void)
{
  int i;
  memset(&event_handler_.rule_table, 0x00, sizeof(EH_RuleTable));
  // 現時点で EL_CORE_GROUP_NULL == 0 であるため，以下は不要．
  // for (i = 0; i < EH_RULE_MAX; ++i)
  // {
  //   event_handler_.rule_table.rules[i].settings.event.group = (EL_GROUP)EL_CORE_GROUP_NULL;
  // }

  // EH_RuleSortedIndex もクリア
  memset(event_handler_.sorted_idxes, 0x00, sizeof(EH_RuleSortedIndex) * EH_RULE_MAX);
  for (i = 0; i < EH_RULE_MAX; ++i)
  {
    // 現時点で EL_CORE_GROUP_NULL == 0 であるため，以下は不要．
    // event_handler_.sorted_idxes[i].group = (EL_GROUP)EL_CORE_GROUP_NULL;
    event_handler_.sorted_idxes[i].rule_id = EH_RULE_MAX;
  }
}


static void EH_clear_log_(void)
{
  int i;
  memset(&event_handler_.log_table, 0x00, sizeof(EH_LogTable));

  for (i = 0; i < EH_LOG_MAX; ++i)
  {
    event_handler_.log_table.logs[i].rule_id = EH_RULE_MAX;
  }
}


void EH_execute(void)
{
  uint8_t responded_count = 0;
  EH_ACK ack;
  uint16_t i;

  ack = EH_check_el_event_counter_();
  if (ack != EH_ACK_OK) return;

  for (i = 0; i < event_handler_.max_check_event_num; ++i)
  {
    uint8_t responded_num;
    EH_CKECK_AND_RESPOND_ACK ack = EH_check_event_and_respond_(&responded_num);
    if (ack == EH_CKECK_AND_RESPOND_ACK_NO_UNPROCESSED_EVENT) break;
    responded_count += responded_num;
    if (responded_count >= event_handler_.max_response_num) break;
  }
}


static EH_ACK EH_check_el_event_counter_(void)
{
  int32_t delta_counter = event_logger->statistics.record_counter_total - event_handler_.el_event_counter.counter_total;
  int32_t delta_counters[EL_ERROR_LEVEL_MAX];
  uint8_t err_level;
  uint32_t subtotal = 0;

  // カウンタ不整合を調べる．
  // エラー等は定期的にリセットされているはずなので，オーバーフローは考慮していない
  if (delta_counter < 0)
  {
    // 不整合
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_EL_TOTAL_COUNTER_ERR,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)(-delta_counter));
    // 仕方がないので強制的に合わせる
    EH_match_event_counter_to_el();
    return EH_ACK_ERR;
  }

  for (err_level = 0; err_level < EL_ERROR_LEVEL_MAX; ++err_level)
  {
    delta_counters[err_level] = event_logger->statistics.record_counters[err_level] - event_handler_.el_event_counter.counters[err_level];
    if (delta_counters[err_level] < 0)
    {
      // 不整合
      EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                      EH_EL_LOCAL_ID_EL_COUNTER_ERR,
                      EL_ERROR_LEVEL_HIGH,
                      (uint32_t)(err_level));
      // 仕方がないので強制的に合わせる
      EH_match_event_counter_to_el();
      return EH_ACK_ERR;
    }

    subtotal += delta_counters[err_level];
  }

  if (delta_counter != subtotal)
  {
    // 不整合
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_EL_COUNTER_ERR,
                    EL_ERROR_LEVEL_HIGH,
                    EL_ERROR_LEVEL_MAX);    // ここの note は少し考える？
    // 仕方がないので強制的に合わせる
    EH_match_event_counter_to_el();
    return EH_ACK_ERR;
  }

  return EH_ACK_OK;
}


static EH_CKECK_AND_RESPOND_ACK EH_check_event_and_respond_(uint8_t* responded_num)
{
  const EL_Event* event = EH_get_oldest_event_();
  EH_RULE_SORTED_INDEX_ACK ack;

  EH_RULE_ID found_ids[EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES];
  uint16_t found_sorted_idxes[EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES];
  uint8_t found_id_num;
  uint8_t i;

  // エラーを返してもいいようにここで初期化
  *responded_num = 0;

  if (event == NULL) return EH_CKECK_AND_RESPOND_ACK_NO_UNPROCESSED_EVENT;

  // 処理する EL_Event が見つかったので，カウントアップ
  event_handler_.el_event_counter.counter_total++;
  event_handler_.el_event_counter.counters[event->err_level]++;

  ack = EH_search_rule_table_index_(event->group,
                                    event->local,
                                    found_ids,
                                    found_sorted_idxes,
                                    &found_id_num);

  if (ack == EH_RULE_SORTED_INDEX_ACK_NOT_FOUND)
  {
    // 対応する EH_Rule なし
    return EH_CKECK_AND_RESPOND_ACK_NOT_RESPONDED;
  }
  if (ack != EH_RULE_SORTED_INDEX_ACK_OK)
  {
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_SEARCH_ERR,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)ack);
    return EH_CKECK_AND_RESPOND_ACK_UNKNOWN_ERR;
  }

  // ルールチェック & 対応
  for (i = 0; i < found_id_num; ++i)
  {
    EH_CKECK_AND_RESPOND_ACK ack =  EH_check_rule_and_respond_(found_ids[i], event);
    if (ack == EH_CKECK_AND_RESPOND_ACK_RESPONDED)
    {
      (*responded_num)++;
    }
  }

  return (*responded_num == 0) ? EH_CKECK_AND_RESPOND_ACK_NOT_RESPONDED : EH_CKECK_AND_RESPOND_ACK_RESPONDED;
}


static EH_CKECK_AND_RESPOND_ACK EH_check_rule_and_respond_(EH_RULE_ID rule_id, const EL_Event* event)
{
  EH_RuleSettings* rule_settings = &event_handler_.rule_table.rules[rule_id].settings;

  if (rule_settings->event.group != event->group || rule_settings->event.local != event->local)
  {
    // 何かがおかしい（ありえないが，安全のため入れている．問題なさそうなら消してよし）
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_UNKNOWN_ERR,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)rule_id);
    return EH_CKECK_AND_RESPOND_ACK_UNKNOWN_ERR;
  }

  if (rule_settings->should_match_err_level)
  {
    if (rule_settings->event.err_level != event->err_level)
    {
      return EH_CKECK_AND_RESPOND_ACK_NOT_RESPONDED;
    }
  }

  if (!(rule_settings->is_active))
  {
    return EH_CKECK_AND_RESPOND_ACK_NOT_RESPONDED;
  }

  switch (rule_settings->condition.type)
  {
  case EH_RESPONSE_CONDITION_SINGLE:
    return EH_check_single_rule_and_respond_(rule_id, event);
  case EH_RESPONSE_CONDITION_CONTINUOUS:
    return EH_check_continuous_rule_and_respond_(rule_id, event);
  case EH_RESPONSE_CONDITION_CUMULATIVE:
    return EH_check_cumulative_rule_and_respond_(rule_id, event);
  default:
    // 何かがおかしい（ありえないが，安全のため入れている．問題なさそうなら消してよし）
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_UNKNOWN_ERR,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)rule_id);
    return EH_CKECK_AND_RESPOND_ACK_NOT_RESPONDED;
  }
}


static EH_CKECK_AND_RESPOND_ACK EH_check_single_rule_and_respond_(EH_RULE_ID rule_id, const EL_Event* event)
{
  EH_Rule* rule = &event_handler_.rule_table.rules[rule_id];
  rule->last_event_time = event->time;

  EH_respond_(rule_id);
  return EH_CKECK_AND_RESPOND_ACK_RESPONDED;
}


static EH_CKECK_AND_RESPOND_ACK EH_check_continuous_rule_and_respond_(EH_RULE_ID rule_id, const EL_Event* event)
{
  EH_Rule* rule = &event_handler_.rule_table.rules[rule_id];
  uint32_t delta_time_ms = OBCT_diff_in_msec(&rule->last_event_time, &event->time);
  rule->last_event_time = event->time;

  if (delta_time_ms > rule->settings.condition.time_threshold_ms)
  {
    // 連続ではなくなった
    rule->counter = 0;
  }

  rule->counter++;

  if (rule->counter >= rule->settings.condition.count_threshold)
  {
    EH_respond_(rule_id);
    return EH_CKECK_AND_RESPOND_ACK_RESPONDED;
  }

  return EH_CKECK_AND_RESPOND_ACK_NOT_RESPONDED;
}


static EH_CKECK_AND_RESPOND_ACK EH_check_cumulative_rule_and_respond_(EH_RULE_ID rule_id, const EL_Event* event)
{
  EH_Rule* rule = &event_handler_.rule_table.rules[rule_id];
  rule->last_event_time = event->time;
  rule->counter++;

  if (rule->counter >= rule->settings.condition.count_threshold)
  {
    EH_respond_(rule_id);
    return EH_CKECK_AND_RESPOND_ACK_RESPONDED;
  }

  return EH_CKECK_AND_RESPOND_ACK_NOT_RESPONDED;
}


static void EH_respond_(EH_RULE_ID rule_id)
{
  EH_Rule* rule = &event_handler_.rule_table.rules[rule_id];
  CTCP packet;
  CTCP_UTIL_ACK deploy_ack;
  CCP_EXEC_STS  deploy_cmd_ack;

  deploy_ack = CCP_form_block_deploy_cmd(&packet, TL_ID_DEPLOY_BC, rule->settings.deploy_bct_id);
  if (deploy_ack != CTCP_UTIL_ACK_OK)
  {
    // BC 展開 Cmd の生成に失敗
    // 正しく組んでいる場合，ここに来るはずはない
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_FAIL_FORM_CTCP,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)rule_id);
    return;
  }

  deploy_cmd_ack = PH_dispatch_command(&packet);
  if (deploy_cmd_ack != CCP_EXEC_SUCCESS)
  {
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_FAIL_FORM_CTCP,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)(deploy_cmd_ack + 100));   // FIXME: CCP_EXEC_STS が負数も含むので．．．なんとかしたい
  }

  rule->counter = 0;
  EH_inactivate_rule(rule_id);

  EH_record_responded_log_(rule_id, deploy_cmd_ack);
}


static void EH_record_responded_log_(EH_RULE_ID rule_id, CCP_EXEC_STS deploy_cmd_ack)
{
  EH_LogTable* log_table = &event_handler_.log_table;

  log_table->respond_counter++;

  log_table->logs[log_table->log_wp].rule_id = rule_id;
  log_table->logs[log_table->log_wp].respond_time_in_master_cycle = TMGR_get_master_total_cycle();
  log_table->logs[log_table->log_wp].deploy_cmd_ack = deploy_cmd_ack;

  log_table->log_wp++;
  if (log_table->log_wp >= EH_LOG_MAX)
  {
    log_table->log_wp = 0;
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_LOG_TABLE_FULL,
                    EL_ERROR_LEVEL_HIGH,
                    0);
  }
}


const EL_Event* EH_get_oldest_event_(void)
{
  uint8_t err_level;      // for で回すので u8 で
  ObcTime oldest_time = OBCT_get_max();
  const EL_Event* oldest_event = NULL;

  for (err_level = 0; err_level < EL_ERROR_LEVEL_MAX; ++err_level)
  {
    // EH 実行中にも EL のイベント登録は発生するため，ここで最新の情報を元に処理する
    int32_t delta_counter = event_logger->statistics.record_counters[err_level] - event_handler_.el_event_counter.counters[err_level];
    const uint16_t tlog_capacity = event_logger->tlogs[err_level].log_capacity;
    const EL_Event* event;

    if (delta_counter <= 0)
    {
      continue;
    }

    // キャパを溢れていたら，諦めて情報を修正する
    if (delta_counter > tlog_capacity)
    {
      const uint16_t keep_tlogs_num = (uint16_t)(tlog_capacity / 2);
      uint32_t drop_tlog_num;

      // イベントが発生しすぎて，TLogが失われている
      EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                      EH_EL_LOCAL_ID_TOO_MANY_EVENT,
                      EL_ERROR_LEVEL_HIGH,
                      (uint32_t)(err_level));

      // 溢れかえっているということなので，またすぐ溢れることを考慮し
      // tlog_capacity の半分ぐらいまでのこし，後は諦める
      drop_tlog_num = (uint32_t)(delta_counter - keep_tlogs_num);

      event_handler_.el_event_counter.counter_total += drop_tlog_num;
      event_handler_.el_event_counter.counters[err_level] += drop_tlog_num;

      // カウンタを変えたので，更新
      delta_counter = event_logger->statistics.record_counters[err_level] - event_handler_.el_event_counter.counters[err_level];
    }

    event = EL_get_the_nth_tlog_from_the_latest((EL_ERROR_LEVEL)err_level, (uint16_t)(delta_counter - 1));
    if (OBCT_compare(&event->time, &oldest_time) == 1)
    {
      oldest_event = event;
      oldest_time = event->time;
    }
  }

  return oldest_event;
}


static EH_RULE_SORTED_INDEX_ACK EH_search_rule_table_index_(EL_GROUP group,
                                                            uint32_t local,
                                                            EH_RULE_ID found_ids[EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES],
                                                            uint16_t found_sorted_idxes[EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES],
                                                            uint8_t* found_id_num)
{
  // idx: 0 ~ rule_table.registered_rule_num の間で二分探索する
  // 重複もあり得ることを考慮する

  int32_t lower, upper;     // 計算の都合上，upper が -1 になることがあるので，符号付き整数
  uint16_t found_idx = EH_RULE_MAX;
  int i = 0;

  if (event_handler_.rule_table.registered_rule_num == 0)
  {
    return EH_RULE_SORTED_INDEX_ACK_NOT_FOUND;
  }

  lower = 0;
  upper = event_handler_.rule_table.registered_rule_num - 1;   // 非排他（包含）範囲

  // 無限ループ回避のための while ではなく for
  for (i = 0; i < EH_RULE_MAX; ++i)
  {
    int32_t mid = (lower + upper) / 2;
    EH_RuleSortedIndex* p_mid = &event_handler_.sorted_idxes[mid];

    if (lower > upper)
    {
      return EH_RULE_SORTED_INDEX_ACK_NOT_FOUND;
    }

    if (p_mid->group == group)
    {
      if (p_mid->local == local)
      {
        if (p_mid->duplicate_id == 0)
        {
          // 見つかった！！（重複の場合は最も duplicate_id が小さいもの）
          found_idx = (uint16_t)mid;
          break;
        }
        else
        {
          // より若い duplicate_id があるはず
          upper = mid - 1;
        }
      }
      else if (p_mid->local < local)
      {
        lower = mid + 1;
      }
      else
      {
        upper = mid - 1;
      }
    }
    else if (p_mid->group < group)
    {
      lower = mid + 1;
    }
    else
    {
      upper = mid - 1;
    }
  }

  if (found_idx == EH_RULE_MAX) return EH_RULE_SORTED_INDEX_ACK_UNKNOWN_ERR;

  // 見つかった．後は，いくつあるか？
  *found_id_num = 0;
  for (i = 0; i < EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES; ++i)
  {
    EH_RuleSortedIndex* p_idx = &event_handler_.sorted_idxes[found_idx + i];
    if ( !(p_idx->group == group && p_idx->local == local) )
    {
      break;
    }
    found_sorted_idxes[i] = (uint16_t)(found_idx + i);
    found_ids[i] = p_idx->rule_id;
    (*found_id_num)++;
  }

  return EH_RULE_SORTED_INDEX_ACK_OK;
}


static EH_RULE_SORTED_INDEX_ACK EH_insert_rule_table_(EH_RULE_ID id, const EH_Rule* rule)
{
  // insert は，めんどくさい & 頻繁には起きないので，二分探索せずにやっていく
  EH_RuleTable* p_rule_table = &event_handler_.rule_table;
  EH_RuleSortedIndex* p_sorted_idxes = event_handler_.sorted_idxes;
  const EL_GROUP insert_group = rule->settings.event.group;
  const uint32_t insert_local = rule->settings.event.local;
  uint16_t i;
  uint8_t j;
  uint16_t insert_idx;
  uint8_t duplicate_id;

  EH_CHECK_RULE_ACK check_ack = EH_check_rule_id_(id);
  if (check_ack == EH_CHECK_RULE_ACK_INVALID_RULE_ID) return EH_RULE_SORTED_INDEX_ACK_ILLEGAL_RULE_ID;
  if (check_ack == EH_CHECK_RULE_ACK_OK) return EH_RULE_SORTED_INDEX_ACK_RULE_OVERWRITE;

  if (p_rule_table->registered_rule_num >= EH_RULE_MAX)
  {
    return EH_RULE_SORTED_INDEX_ACK_FULL;
  }

  insert_idx = p_rule_table->registered_rule_num;
  duplicate_id = 0;
  for (i = 0; i < p_rule_table->registered_rule_num; ++i)
  {
    EH_RuleSortedIndex* p_sorted_idx = &p_sorted_idxes[i];
    uint8_t break_flag = 0;

    if (p_sorted_idx->group < insert_group) continue;

    if (p_sorted_idx->group > insert_group)
    {
      // 挿入する場所はここ
      insert_idx = i;
      break;
    }

    // ここまできたら p_sorted_idx->group == insert_group
    if (p_sorted_idx->local < insert_local) continue;

    if (p_sorted_idx->local > insert_local)
    {
      // 挿入する場所はここ
      insert_idx = i;
      break;
    }

    // ここまできたら
    // - p_sorted_idx->group == insert_group
    // - p_sorted_idx->local == insert_local

    // 重複してる or する
    for (j = 1; j < EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES; ++j)
    {
      uint16_t idx = (uint16_t)(i + j);
      EH_RuleSortedIndex* p_sorted_idx = &p_sorted_idxes[idx];

      if (idx >= p_rule_table->registered_rule_num)
      {
        // 末端まできたので，ここが挿入する場所
        insert_idx = idx;
        duplicate_id = j;
        break_flag = 1;
        break;
      }

      if (p_sorted_idx->group == insert_group && p_sorted_idx->local == insert_local)
      {
        // まだ重複している
        continue;
      }

      // 重複しなくなったので，ここが挿入する場所
      insert_idx = idx;
      duplicate_id = j;
      break_flag = 1;
      break;
    }
    if (break_flag) break;

    // 重複上限まで重複してしまっている
    return EH_RULE_SORTED_INDEX_ACK_DUPLICATE_FULL;
  }

  // ここまで来たら，挿入できる
  // insert_idx, duplicate_id が出揃っているはず
  memmove(&p_sorted_idxes[insert_idx + 1],
          &p_sorted_idxes[insert_idx],
          sizeof(EH_RuleSortedIndex) * (EH_RULE_MAX - insert_idx - 1));
  p_sorted_idxes[insert_idx].group        = insert_group;
  p_sorted_idxes[insert_idx].local        = insert_local;
  p_sorted_idxes[insert_idx].duplicate_id = duplicate_id;
  p_sorted_idxes[insert_idx].rule_id      = id;

  p_rule_table->rules[id] = *rule;
  p_rule_table->registered_rule_num++;

  return EH_RULE_SORTED_INDEX_ACK_OK;
}


static EH_RULE_SORTED_INDEX_ACK EH_delete_rule_table_(EH_RULE_ID id)
{
  // delete は，めんどくさい & 頻繁には起きないので，二分探索せずにやっていく
  EH_RuleTable* p_rule_table = &event_handler_.rule_table;
  EH_RuleSortedIndex* p_sorted_idxes = event_handler_.sorted_idxes;
  EL_GROUP delete_group;
  uint32_t delete_local;
  uint16_t i;
  uint8_t j;
  uint16_t delete_idx;

  EH_CHECK_RULE_ACK check_ack = EH_check_rule_id_(id);
  if (check_ack == EH_CHECK_RULE_ACK_INVALID_RULE_ID) return EH_RULE_SORTED_INDEX_ACK_ILLEGAL_RULE_ID;
  if (check_ack == EH_CHECK_RULE_ACK_UNREGISTERED) return EH_RULE_SORTED_INDEX_ACK_NOT_FOUND;

  // この２個，const にしたい．．．
  delete_group = p_rule_table->rules[id].settings.event.group;
  delete_local = p_rule_table->rules[id].settings.event.local;

  delete_idx = EH_RULE_MAX;
  for (i = 0; i < p_rule_table->registered_rule_num; ++i)
  {
    if (id == p_sorted_idxes[i].rule_id)
    {
      // 見つかった
      delete_idx = i;
      break;
    }
  }

  if (delete_idx == EH_RULE_MAX)
  {
    // 本当はこれはありえないはず
    // （ありえないが，安全のため入れている．問題なさそうなら消してよし）
    EL_record_event((EL_GROUP)EL_CORE_GROUP_EVENT_HANDLER,
                    EH_EL_LOCAL_ID_UNKNOWN_ERR,
                    EL_ERROR_LEVEL_HIGH,
                    0xffffffff);
    return EH_RULE_SORTED_INDEX_ACK_NOT_FOUND;
  }

  // ここまで来たら削除できる
  memmove(&p_sorted_idxes[delete_idx],
          &p_sorted_idxes[delete_idx + 1],
          sizeof(EH_RuleSortedIndex) * (EH_RULE_MAX - delete_idx - 1));
  memset(&p_sorted_idxes[EH_RULE_MAX - 1], 0x00, sizeof(EH_RuleSortedIndex));
  p_sorted_idxes[EH_RULE_MAX - 1].group = (EL_GROUP)EL_CORE_GROUP_NULL;
  p_sorted_idxes[EH_RULE_MAX - 1].rule_id = EH_RULE_MAX;

  // 重複IDの処理
  for (j = 0; j < (EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES - 1); ++j)
  {
    uint16_t idx = (uint16_t)(delete_idx + j);
    EH_RuleSortedIndex* p_sorted_idx = &p_sorted_idxes[idx];

    if (p_sorted_idx->group == delete_group && p_sorted_idx->local == delete_local)
    {
      p_sorted_idx->duplicate_id--;
    }
    else
    {
      break;
    }
  }

  memset(&p_rule_table->rules[id], 0x00, sizeof(EH_Rule));
  p_rule_table->rules[id].settings.event.group = (EL_GROUP)EL_CORE_GROUP_NULL;
  p_rule_table->registered_rule_num--;

  return EH_RULE_SORTED_INDEX_ACK_OK;
}


EH_REGISTER_ACK EH_register_rule(EH_RULE_ID id, const EH_RuleSettings* settings)
{
  EH_RULE_SORTED_INDEX_ACK ack;
  EH_Rule rule;

  if (settings->event.group >= EL_GROUP_MAX)                 return EH_REGISTER_ACK_ILLEGAL_GROUP;
  if (settings->event.group <= (EL_GROUP)EL_CORE_GROUP_NULL) return EH_REGISTER_ACK_ILLEGAL_GROUP;
  if (settings->event.err_level < 0)                         return EH_REGISTER_ACK_ILLEGAL_ERROR_LEVEL;
  if (settings->event.err_level >= EL_ERROR_LEVEL_MAX)       return EH_REGISTER_ACK_ILLEGAL_ERROR_LEVEL;
  if (settings->should_match_err_level != 0 && settings->should_match_err_level != 1)
  {
    return EH_REGISTER_ACK_ILLEGAL_MATCH_FLAG;
  }
  if (settings->condition.type >= EH_RESPONSE_CONDITION_TYPE_MAX) return EH_REGISTER_ACK_ILLEGAL_CONDITION_TYPE;
  if (settings->condition.type != EH_RESPONSE_CONDITION_SINGLE)
  {
    if (settings->condition.count_threshold == 0) return EH_REGISTER_ACK_ILLEGAL_COUNT_THRESHOLD;
  }
  if (settings->deploy_bct_id >= BCT_MAX_BLOCKS) return EH_REGISTER_ACK_ILLEGAL_BCT_ID;
  if (settings->is_active != 0 && settings->is_active != 1)
  {
    return EH_REGISTER_ACK_ILLEGAL_ACTIVE_FLAG;
  }

  rule.settings = *settings;
  rule.counter = 0;
  OBCT_clear(&rule.last_event_time);

  ack = EH_insert_rule_table_(id, &rule);

  switch (ack)
  {
  case EH_RULE_SORTED_INDEX_ACK_OK:
    return EH_REGISTER_ACK_OK;
  case EH_RULE_SORTED_INDEX_ACK_ILLEGAL_RULE_ID:
    return EH_REGISTER_ACK_ILLEGAL_RULE_ID;
  case EH_RULE_SORTED_INDEX_ACK_FULL:
    return EH_REGISTER_ACK_ERR_FULL;
  case EH_RULE_SORTED_INDEX_ACK_RULE_OVERWRITE:
    return EH_REGISTER_ACK_ERR_RULE_OVERWRITE;
  case EH_RULE_SORTED_INDEX_ACK_DUPLICATE_FULL:
    return EH_REGISTER_ACK_ERR_DUPLICATE_FULL;
  default:
    return EH_REGISTER_ACK_UNKNOWN_ERR;
  }
}


static EH_CHECK_RULE_ACK EH_check_rule_id_(EH_RULE_ID id)
{
  if (id < 0) return EH_CHECK_RULE_ACK_INVALID_RULE_ID;
  if (id >= EH_RULE_MAX) return EH_CHECK_RULE_ACK_INVALID_RULE_ID;
  if (event_handler_.rule_table.rules[id].settings.event.group == (EL_GROUP)EL_CORE_GROUP_NULL)
  {
    return EH_CHECK_RULE_ACK_UNREGISTERED;
  }
  return EH_CHECK_RULE_ACK_OK;
}


EH_CHECK_RULE_ACK EH_activate_rule(EH_RULE_ID id)
{
  EH_CHECK_RULE_ACK ack = EH_check_rule_id_(id);
  if (ack != EH_CHECK_RULE_ACK_OK) return ack;

  event_handler_.rule_table.rules[id].settings.is_active = 1;
  return EH_CHECK_RULE_ACK_OK;
}


EH_CHECK_RULE_ACK EH_inactivate_rule(EH_RULE_ID id)
{
  EH_CHECK_RULE_ACK ack = EH_check_rule_id_(id);
  if (ack != EH_CHECK_RULE_ACK_OK) return ack;

  event_handler_.rule_table.rules[id].settings.is_active = 0;
  return EH_CHECK_RULE_ACK_OK;
}


void EH_match_event_counter_to_el(void)
{
  uint8_t err_level;
  event_handler_.el_event_counter.counter_total = event_logger->statistics.record_counter_total;
  for (err_level = 0; err_level < EL_ERROR_LEVEL_MAX; ++err_level)
  {
    event_handler_.el_event_counter.counters[err_level] = event_logger->statistics.record_counters[err_level];
  }
}


const EH_Log* EH_get_the_nth_log_from_the_latest(uint16_t n)
{
  uint16_t idx;

  if (n >= EH_LOG_MAX)
  {
    // 仕方がないので，最新のものを
    idx = 0;
  }
  else
  {
    idx = (uint16_t)((event_handler->log_table.log_wp - 1 - n + EH_LOG_MAX) % EH_LOG_MAX);
  }

  return &event_handler->log_table.logs[idx];
}


CCP_EXEC_STS Cmd_EH_INIT(const CTCP* packet)
{
  (void)packet;
  EH_initialize();
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_CLEAR_ALL_RULE(const CTCP* packet)
{
  (void)packet;
  EH_clear_rules_();
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_LOAD_DEFAULT_RULE(const CTCP* packet)
{
  (void)packet;
  EH_load_default_rules();
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_SET_REGISTER_RULE_EVENT_PARAM(const CTCP* packet)
{
  // 登録する瞬間にしかわからないので，ここでは値のアサーションはせず，
  // Cmd_EH_REGISTER_RULE でアサーションする
  const uint8_t* param = CCP_get_param_head(packet);

  uint16_t temp_u16;
  uint32_t temp_u32;
  bct_id_t bct_id;

  if (CCP_get_param_len(packet) != (12 + SIZE_OF_BCT_ID_T))
  {
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  endian_memcpy(&temp_u16, &param[0], 2);
  event_handler_.reg_from_cmd.rule_id = (EH_RULE_ID)temp_u16;
  endian_memcpy(&temp_u32, &param[2], 4);
  event_handler_.reg_from_cmd.settings.event.group = (EL_GROUP)temp_u32;
  endian_memcpy(&temp_u32, &param[6], 4);
  event_handler_.reg_from_cmd.settings.event.local = temp_u32;

  event_handler_.reg_from_cmd.settings.event.err_level = (EL_ERROR_LEVEL)param[10];
  event_handler_.reg_from_cmd.settings.should_match_err_level = param[11];

  endian_memcpy(&bct_id, &param[12], SIZE_OF_BCT_ID_T);
  event_handler_.reg_from_cmd.settings.deploy_bct_id = bct_id;

  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_SET_REGISTER_RULE_CONDITION_PARAM(const CTCP* packet)
{
  // 登録する瞬間にしかわからないので，ここでは値のアサーションはせず，
  // Cmd_EH_REGISTER_RULE でアサーションする
  const uint8_t* param = CCP_get_param_head(packet);

  uint16_t temp_u16;
  uint32_t temp_u32;

  event_handler_.reg_from_cmd.settings.condition.type = (EH_RESPONSE_CONDITION_TYPE)param[0];

  endian_memcpy(&temp_u16, &param[1], 2);
  event_handler_.reg_from_cmd.settings.condition.count_threshold = temp_u16;
  endian_memcpy(&temp_u32, &param[3], 4);
  event_handler_.reg_from_cmd.settings.condition.time_threshold_ms = temp_u32;

  event_handler_.reg_from_cmd.settings.is_active = param[7];

  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_REGISTER_RULE(const CTCP* packet)
{
  (void)packet;
  event_handler_.reg_from_cmd.register_ack =
      EH_register_rule(event_handler_.reg_from_cmd.rule_id, &event_handler_.reg_from_cmd.settings);

  switch (event_handler_.reg_from_cmd.register_ack)
  {
  case EH_REGISTER_ACK_OK:
    return CCP_EXEC_SUCCESS;
  case EH_REGISTER_ACK_ILLEGAL_RULE_ID:
  case EH_REGISTER_ACK_ILLEGAL_GROUP:
  case EH_REGISTER_ACK_ILLEGAL_ERROR_LEVEL:
  case EH_REGISTER_ACK_ILLEGAL_MATCH_FLAG:
  case EH_REGISTER_ACK_ILLEGAL_CONDITION_TYPE:
  case EH_REGISTER_ACK_ILLEGAL_COUNT_THRESHOLD:
  case EH_REGISTER_ACK_ILLEGAL_BCT_ID:
  case EH_REGISTER_ACK_ILLEGAL_ACTIVE_FLAG:
    return CCP_EXEC_ILLEGAL_PARAMETER;    // 正確にはこのコマンドのパラメタではないが．．．
  case EH_REGISTER_ACK_ERR_FULL:
  case EH_REGISTER_ACK_ERR_RULE_OVERWRITE:
  case EH_REGISTER_ACK_ERR_DUPLICATE_FULL:
  case EH_REGISTER_ACK_UNKNOWN_ERR:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}


CCP_EXEC_STS Cmd_EH_DELETE_RULE(const CTCP* packet)
{
  uint16_t rule_id;
  EH_RULE_SORTED_INDEX_ACK ack;

  endian_memcpy(&rule_id, CCP_get_param_head(packet), 2);
  ack = EH_delete_rule_table_((EH_RULE_ID)rule_id);

  switch (ack)
  {
  case EH_RULE_SORTED_INDEX_ACK_OK:
    return CCP_EXEC_SUCCESS;
  case EH_RULE_SORTED_INDEX_ACK_ILLEGAL_RULE_ID:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  case EH_RULE_SORTED_INDEX_ACK_NOT_FOUND:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}


CCP_EXEC_STS Cmd_EH_ACTIVATE_RULE(const CTCP* packet)
{
  uint16_t rule_id;
  EH_CHECK_RULE_ACK ack;

  endian_memcpy(&rule_id, CCP_get_param_head(packet), 2);
  ack = EH_activate_rule((EH_RULE_ID)rule_id);

  switch (ack)
  {
  case EH_CHECK_RULE_ACK_OK:
    return CCP_EXEC_SUCCESS;
  case EH_CHECK_RULE_ACK_INVALID_RULE_ID:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  case EH_CHECK_RULE_ACK_UNREGISTERED:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}


CCP_EXEC_STS Cmd_EH_INACTIVATE_RULE(const CTCP* packet)
{
  uint16_t rule_id;
  EH_CHECK_RULE_ACK ack;

  endian_memcpy(&rule_id, CCP_get_param_head(packet), 2);
  ack = EH_inactivate_rule((EH_RULE_ID)rule_id);

  switch (ack)
  {
  case EH_CHECK_RULE_ACK_OK:
    return CCP_EXEC_SUCCESS;
  case EH_CHECK_RULE_ACK_INVALID_RULE_ID:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  case EH_CHECK_RULE_ACK_UNREGISTERED:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}


CCP_EXEC_STS Cmd_EH_CLEAR_LOG(const CTCP* packet)
{
  (void)packet;
  EH_clear_log_();
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_SET_MAX_RESPONSE_NUM(const CTCP* packet)
{
  event_handler_.max_response_num = CCP_get_param_head(packet)[0];
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_SET_MAX_CHECK_EVENT_NUM(const CTCP* packet)
{
  endian_memcpy(&event_handler_.max_check_event_num, CCP_get_param_head(packet), 2);
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_SET_PAGE_OF_RULE_TABLE_FOR_TLM(const CTCP* packet)
{
  uint8_t page = CCP_get_param_head(packet)[0];
  if (page >= EH_RULE_TLM_PAGE_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;
  event_handler_.tlm_info.rule.page_no = page;
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM(const CTCP* packet)
{
  uint8_t page = CCP_get_param_head(packet)[0];
  if (page >= EH_RULE_TLM_PAGE_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;
  event_handler_.tlm_info.rule_sorted_index.page_no = page;
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_EH_SET_PAGE_OF_LOG_TABLE_FOR_TLM(const CTCP* packet)
{
  uint8_t page = CCP_get_param_head(packet)[0];
  if (page >= EH_LOG_TLM_PAGE_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;
  event_handler_.tlm_info.log.page_no = page;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM(const CTCP* packet)
{
  uint16_t temp;
  EH_RULE_ID rule_id;

  endian_memcpy(&temp, CCP_get_param_head(packet), 2);
  rule_id = (EH_RULE_ID)temp;

  if (EH_check_rule_id_(rule_id) == EH_CHECK_RULE_ACK_INVALID_RULE_ID)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  event_handler_.tlm_info.rule.target_rule_id = rule_id;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_EH_MATCH_EVENT_COUNTER_TO_EL(const CTCP* packet)
{
  (void)packet;
  EH_match_event_counter_to_el();
  return CCP_EXEC_SUCCESS;
}

#endif  // EL_IS_ENABLE_TLOG

CCP_EXEC_STS Cmd_EH_TEMP_FOR_ISTS(const CTCP* packet)
{
  EH_RULE_ID rule_id = (EH_RULE_ID)CCP_get_param_from_packet(packet, 0, uint16_t);
  EH_Rule oldest_rule = event_handler->rule_table.rules[EH_RULE_MAX];

  if (EH_check_rule_id_(rule_id) == EH_CHECK_RULE_ACK_INVALID_RULE_ID)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  if (EH_insert_rule_table_(rule_id, &oldest_rule) != EH_RULE_SORTED_INDEX_ACK_OK)
  {
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
  return CCP_EXEC_SUCCESS;
}

#pragma section
