#pragma section REPRO
/**
 * @file
 * @brief アノマリやエラーなどの各種イベントを記録する
 * @note  このイベントをもとに event_handler を発火させることができる
 * @note  ログは TLog (TimeSeriesLog) と CLog (EL_CumulativeLog) の二種類を準備し，必要なもののみを使う
 * @note  今後， NvLog (不揮発Log) が実装予定
 * @note  詳細は event_logger.h を参照
 */
#include "event_logger.h"
#include "event_handler.h"
#include <string.h>
#include "../TimeManager/time_manager.h"
#include "../WatchdogTimer/watchdog_timer.h"
#include <src_user/Settings/System/event_logger_settings.h>
#include "../../TlmCmd/common_cmd_packet_util.h"


#ifdef EL_IS_ENABLE_CLOG
/**
 * @enum   EL_CLOG_LOG_ACK
 * @brief  ログ操作の返り値
 * @note   uint8_t を想定
 */
typedef enum
{
  EL_CLOG_LOG_ACK_OK = 0,           //!< 正常終了
  EL_CLOG_LOG_ACK_NOT_FOUND         //!< 指定ログが見つからず
} EL_CLOG_LOG_ACK;
#endif


/**
 * @brief  イベント (EL_Event) を初期化
 * @param  group: EL_Event.group
 * @param  local: EL_Event.local
 * @param  err_level: EL_Event.err_level
 * @param  note: EL_Event.note
 * @return EL_Event
 */
static EL_Event EL_init_event_(EL_GROUP group, uint32_t local, EL_ERROR_LEVEL err_level, uint32_t note);

/**
 * @brief  イベント (EL_Event) を記録
 * @note   EL内部では EL_ERROR_LEVEL_EL のイベントが発行できるため，公開関数と分けている
 * @param  group: EL_Event.group
 * @param  local: EL_Event.local
 * @param  err_level: EL_Event.err_level
 * @param  note: EL_Event.note
 * @return EL_ACK
 */
static EL_ACK EL_record_event_(EL_GROUP group, uint32_t local, EL_ERROR_LEVEL err_level, uint32_t note);

#ifdef EL_IS_ENABLE_TLOG
/**
 * @brief  イベント (EL_Event) を TLog に記録
 * @note   アサーションのない，内部用関数
 * @param  event: 記録するイベント
 * @retval EL_ACK_OK        : 正常終了
 * @retval EL_ACK_TLOG_FULL : TLogの追記が不能のため，TLog記録ができず
 */
static EL_ACK EL_record_event_to_tlog_(const EL_Event* event);

/**
 * @brief  TLog の wp をチェックし， event を生成する
 *
 *         wp が 半分，終端のときに event を生成する
 *         これにより，ここで生じた event で TLog テーブルのデータの退避なを行うとよい．
 * @note   err_level のアサーションはしない（内部関数）
 * @param  err_level: チェックするテーブルのエラーレベル
 * @return void
 */
static void EL_check_tlog_wp_(EL_ERROR_LEVEL err_level);

/**
 * @brief  TLogテーブルをすべてクリア
 * @return void
 */
static void EL_clear_all_tlog_(void);

/**
 * @brief  TLogテーブルをクリア
 * @note   err_level のアサーションはしない（内部関数）
 * @param  err_level: クリアするテーブルのエラーレベル
 * @return void
 */
static void EL_clear_tlog_(EL_ERROR_LEVEL err_level);
#endif

#ifdef EL_IS_ENABLE_CLOG
/**
 * @brief  イベント (EL_Event) を CLog に記録
 * @note   アサーションのない，内部用関数
 * @param  event: 記録するイベント
 * @return void
 */
static void EL_record_event_to_clog_(const EL_Event* event);

/**
 * @brief  CLogテーブルをすべてクリア
 * @return void
 */
static void EL_clear_all_clog_(void);

/**
 * @brief  CLogテーブルをクリア
 * @note   err_level のアサーションはしない（内部関数）
 * @param  err_level: クリアするテーブルのエラーレベル
 * @return void
 */
static void EL_clear_clog_(EL_ERROR_LEVEL err_level);

/**
 * @brief  該当イベントの CLog を検索し取得する
 * @param[in]  event:     探索する EL_Event
 * @param[out] log_idx:   EL_CumulativeLog.logs における Clog の場所
 * @param[out] order_idx: EL_CumulativeLog.log_orders における Clog の場所
 * @retval EL_CLOG_LOG_ACK_OK:        見つかった
 * @retval EL_CLOG_LOG_ACK_NOT_FOUND: 見つからず
 */
static EL_CLOG_LOG_ACK EL_search_clog_(const EL_Event* event, uint16_t* log_idx, uint16_t* order_idx);

/**
 * @brief  該当イベントの CLog を，イベントテーブルの先頭に出す
 * @note   CLog がなければ insert する
 * @param[in]  event: 先頭に出す EL_Event
 * @return void
 */
static void EL_move_to_front_in_clog_(const EL_Event* event);

/**
 * @brief  イベントテーブル先頭に該当イベントの CLog をつくる
 * @note   最も古い CLog が破棄されることに注意
 * @note   すでに CLog が記録さえている場合もチェックせずに insert する．すでにチェック (EL_search_clog_) されていることを前提とする！！！
 * @param[in]  event: 先頭に作る EL_Event
 * @return void
 */
static void EL_create_clog_on_front_(const EL_Event* event);
#endif

/**
 * @brief  最新ログをクリア
 * @return void
 */
static void EL_clear_latest_event_(void);

/**
 * @brief  統計情報をクリア
 * @return void
 */
static void EL_clear_statistics_(void);


static EventLogger event_logger_;
const EventLogger* const event_logger = &event_logger_;

#ifdef EL_IS_ENABLE_TLOG
static EL_Event EL_tlog_event_table_high_[EL_TLOG_LOG_SIZE_MAX_HIGH];
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
static EL_Event EL_tlog_event_table_middle_[EL_TLOG_LOG_SIZE_MAX_MIDDLE];
#endif
static EL_Event EL_tlog_event_table_low_[EL_TLOG_LOG_SIZE_MAX_LOW];
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
static EL_Event EL_tlog_event_table_el_[EL_TLOG_LOG_SIZE_MAX_EL];
#endif
static EL_Event EL_tlog_event_table_eh_[EL_TLOG_LOG_SIZE_MAX_EH];
#endif

#ifdef EL_IS_ENABLE_CLOG
static EL_CLogElement EL_clog_log_table_high_[EL_CLOG_LOG_SIZE_MAX_HIGH];
static uint16_t EL_clog_log_order_table_high_[EL_CLOG_LOG_SIZE_MAX_HIGH];
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
static EL_CLogElement EL_clog_log_table_middle_[EL_CLOG_LOG_SIZE_MAX_MIDDLE];
static uint16_t EL_clog_log_order_table_middle_[EL_CLOG_LOG_SIZE_MAX_MIDDLE];
#endif
static EL_CLogElement EL_clog_log_table_low_[EL_CLOG_LOG_SIZE_MAX_LOW];
static uint16_t EL_clog_log_order_table_low_[EL_CLOG_LOG_SIZE_MAX_LOW];
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
static EL_CLogElement EL_clog_log_table_el_[EL_CLOG_LOG_SIZE_MAX_EL];
static uint16_t EL_clog_log_order_table_el_[EL_CLOG_LOG_SIZE_MAX_EL];
#endif
static EL_CLogElement EL_clog_log_table_eh_[EL_CLOG_LOG_SIZE_MAX_EH];
static uint16_t EL_clog_log_order_table_eh_[EL_CLOG_LOG_SIZE_MAX_EH];
#endif


void EL_initialize(void)
{
  memset(&event_logger_, 0x00, sizeof(EventLogger));

  // 最初はすべてのロギングが有効
  EL_enable_all_logging();

#ifdef EL_IS_ENABLE_TLOG
  event_logger_.tlogs[EL_ERROR_LEVEL_HIGH].events         = EL_tlog_event_table_high_;
  event_logger_.tlogs[EL_ERROR_LEVEL_HIGH].log_capacity   = EL_TLOG_LOG_SIZE_MAX_HIGH;
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
  event_logger_.tlogs[EL_ERROR_LEVEL_MIDDLE].events       = EL_tlog_event_table_middle_;
  event_logger_.tlogs[EL_ERROR_LEVEL_MIDDLE].log_capacity = EL_TLOG_LOG_SIZE_MAX_MIDDLE;
#endif
  event_logger_.tlogs[EL_ERROR_LEVEL_LOW].events          = EL_tlog_event_table_low_;
  event_logger_.tlogs[EL_ERROR_LEVEL_LOW].log_capacity    = EL_TLOG_LOG_SIZE_MAX_LOW;
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  event_logger_.tlogs[EL_ERROR_LEVEL_EL].events           = EL_tlog_event_table_el_;
  event_logger_.tlogs[EL_ERROR_LEVEL_EL].log_capacity     = EL_TLOG_LOG_SIZE_MAX_EL;
#endif
  event_logger_.tlogs[EL_ERROR_LEVEL_EH].events           = EL_tlog_event_table_eh_;
  event_logger_.tlogs[EL_ERROR_LEVEL_EH].log_capacity     = EL_TLOG_LOG_SIZE_MAX_EH;

  EL_clear_all_tlog_();

  // デフォルトでは TLog の上書きは有効
  EL_enable_tlog_overwrite_all();
  EL_enable_tlog_overwrite(EL_ERROR_LEVEL_EH);
#endif  // EL_IS_ENABLE_TLOG

#ifdef EL_IS_ENABLE_CLOG
  event_logger_.clogs[EL_ERROR_LEVEL_HIGH].logs           = EL_clog_log_table_high_;
  event_logger_.clogs[EL_ERROR_LEVEL_HIGH].log_orders     = EL_clog_log_order_table_high_;
  event_logger_.clogs[EL_ERROR_LEVEL_HIGH].log_capacity   = EL_CLOG_LOG_SIZE_MAX_HIGH;
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
  event_logger_.clogs[EL_ERROR_LEVEL_MIDDLE].logs         = EL_clog_log_table_middle_;
  event_logger_.clogs[EL_ERROR_LEVEL_MIDDLE].log_orders   = EL_clog_log_order_table_middle_;
  event_logger_.clogs[EL_ERROR_LEVEL_MIDDLE].log_capacity = EL_CLOG_LOG_SIZE_MAX_MIDDLE;
#endif
  event_logger_.clogs[EL_ERROR_LEVEL_LOW].logs            = EL_clog_log_table_low_;
  event_logger_.clogs[EL_ERROR_LEVEL_LOW].log_orders      = EL_clog_log_order_table_low_;
  event_logger_.clogs[EL_ERROR_LEVEL_LOW].log_capacity    = EL_CLOG_LOG_SIZE_MAX_LOW;
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  event_logger_.clogs[EL_ERROR_LEVEL_EL].logs             = EL_clog_log_table_el_;
  event_logger_.clogs[EL_ERROR_LEVEL_EL].log_orders       = EL_clog_log_order_table_el_;
  event_logger_.clogs[EL_ERROR_LEVEL_EL].log_capacity     = EL_CLOG_LOG_SIZE_MAX_EL;
#endif
  event_logger_.clogs[EL_ERROR_LEVEL_EH].logs             = EL_clog_log_table_eh_;
  event_logger_.clogs[EL_ERROR_LEVEL_EH].log_orders       = EL_clog_log_order_table_eh_;
  event_logger_.clogs[EL_ERROR_LEVEL_EH].log_capacity     = EL_CLOG_LOG_SIZE_MAX_EH;

  EL_clear_all_clog_();
#endif  // EL_IS_ENABLE_CLOG

  // EL_clear_statistics_();    // 冒頭の memset があるので不要
  // EL_clear_latest_event_();    // 冒頭の memset があるので不要

  // ユーザーデフォルト設定
  EL_load_default_settings();

  EH_match_event_counter_to_el();
}


static EL_Event EL_init_event_(EL_GROUP group, uint32_t local, EL_ERROR_LEVEL err_level, uint32_t note)
{
  EL_Event event;

  event.group = group;
  event.local = local;
  event.err_level = err_level;
  event.time = TMGR_get_master_clock();
#ifdef EL_IS_ENABLE_EVENT_NOTE
  event.note = note;
#else
  (void)note;
#endif

  return event;
}


EL_ACK EL_record_event(EL_GROUP group, uint32_t local, EL_ERROR_LEVEL err_level, uint32_t note)
{
  // ユーザーからのイベント記録で EL_IS_ENABLE_EL_ERROR_LEVEL は禁止
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  if (err_level == EL_ERROR_LEVEL_EL) return EL_ACK_ILLEGAL_ERROR_LEVEL;
#endif

  return EL_record_event_(group, local, err_level, note);
}


static EL_ACK EL_record_event_(EL_GROUP group, uint32_t local, EL_ERROR_LEVEL err_level, uint32_t note)
{
  const EL_Event event = EL_init_event_(group, local, err_level, note);
  EL_ACK ack = EL_ACK_OK;

  if (event.group >= EL_GROUP_MAX)                 return EL_ACK_ILLEGAL_GROUP;
  if (event.group <= (EL_GROUP)EL_CORE_GROUP_NULL) return EL_ACK_ILLEGAL_GROUP;     // これは本当に入れてよいか要議論
  if (event.err_level < 0)                         return EL_ACK_ILLEGAL_ERROR_LEVEL;
  if (event.err_level >= EL_ERROR_LEVEL_MAX)       return EL_ACK_ILLEGAL_ERROR_LEVEL;
  if (!EL_is_logging_enable(event.group))          return EL_ACK_DISABLE_LOGGING;

  event_logger_.latest_event = event;   // 再帰呼び出しの可能性があるので，別途コピーして持つ
  event_logger_.statistics.record_counter_total++;
  event_logger_.statistics.record_counters[event.err_level]++;

#ifdef EL_IS_ENABLE_TLOG
  ack = EL_record_event_to_tlog_(&event);
#endif

#ifdef EL_IS_ENABLE_CLOG
  EL_record_event_to_clog_(&event);
#endif

  // EL_record_event の再帰呼び出しになるので最後に呼び出し．注意！！
#ifdef EL_IS_ENABLE_TLOG
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  // 追記できない場合はスキップ
  if (ack != EL_ACK_TLOG_FULL)
  {
    EL_check_tlog_wp_(event.err_level);
  }
#endif
#endif

  return ack;
}


#ifdef EL_IS_ENABLE_TLOG
static EL_ACK EL_record_event_to_tlog_(const EL_Event* event)
{
  const EL_ERROR_LEVEL err_level = event->err_level;
  EL_TimeSeriesLog* p_tlog = &event_logger_.tlogs[err_level];
  const uint16_t wp = p_tlog->log_wp;

  if ( EL_is_tlog_overwrite_enable(err_level) == 0 && p_tlog->is_table_overflow == 1 )
  {
    // 追記できない
    return EL_ACK_TLOG_FULL;
  }

  p_tlog->events[wp] = *event;
  p_tlog->log_wp++;
  p_tlog->log_wp %= p_tlog->log_capacity;

  if ( EL_is_tlog_overwrite_enable(err_level) == 0 && p_tlog->log_wp == 0 )
  {
    // 次回から追記できなくなる
    p_tlog->is_table_overflow = 1;
  }
  else
  {
    p_tlog->is_table_overflow = 0;
  }
  return EL_ACK_OK;
}


#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
static void EL_check_tlog_wp_(EL_ERROR_LEVEL err_level)
{
  const uint16_t wp = event_logger_.tlogs[err_level].log_wp;
  const uint16_t capacity = event_logger_.tlogs[err_level].log_capacity;
  if (wp == 0)    // capacity を超え，一周したとき
  {
    switch (err_level)
    {
    case EL_ERROR_LEVEL_HIGH:
      EL_record_event_((EL_GROUP)EL_CORE_GROUP_EVENT_LOGGER,
                       EL_EVENT_LOCAL_TLOG_HIGH_FULL,
                       EL_ERROR_LEVEL_EL,
                       0);
      break;

#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
    case EL_ERROR_LEVEL_MIDDLE:
      EL_record_event_((EL_GROUP)EL_CORE_GROUP_EVENT_LOGGER,
                       EL_EVENT_LOCAL_TLOG_MIDDLE_FULL,
                       EL_ERROR_LEVEL_EL,
                       0);
      break;
#endif

    case EL_ERROR_LEVEL_LOW:
      EL_record_event_((EL_GROUP)EL_CORE_GROUP_EVENT_LOGGER,
                       EL_EVENT_LOCAL_TLOG_LOW_FULL,
                       EL_ERROR_LEVEL_EL,
                       0);
      break;

    default:
      // EL_ERROR_LEVEL_EL のものは，無限ループの可能性があるので，イベント発行しない
      break;
    }
  }

  if (wp == (capacity / 2) && (capacity % 2 ) == 0)   // 約半分に達したとき
  {
    switch (err_level)
    {
    case EL_ERROR_LEVEL_HIGH:
      EL_record_event_((EL_GROUP)EL_CORE_GROUP_EVENT_LOGGER,
                       EL_EVENT_LOCAL_TLOG_HIGH_HALF_FULL,
                       EL_ERROR_LEVEL_EL,
                       0);
      break;

#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
    case EL_ERROR_LEVEL_MIDDLE:
      EL_record_event_((EL_GROUP)EL_CORE_GROUP_EVENT_LOGGER,
                       EL_EVENT_LOCAL_TLOG_MIDDLE_HALF_FULL,
                       EL_ERROR_LEVEL_EL,
                       0);
      break;
#endif

    case EL_ERROR_LEVEL_LOW:
      EL_record_event_((EL_GROUP)EL_CORE_GROUP_EVENT_LOGGER,
                       EL_EVENT_LOCAL_TLOG_LOW_HALF_FULL,
                       EL_ERROR_LEVEL_EL,
                       0);
      break;

    default:
      // EL_ERROR_LEVEL_EL のものは，無限ループの可能性があるので，イベント発行しない
      break;
    }
  }
}
#endif


static void EL_clear_all_tlog_(void)
{
  uint8_t err_level;
  for (err_level = 0; err_level < EL_ERROR_LEVEL_MAX; ++err_level)
  {
    EL_clear_tlog_((EL_ERROR_LEVEL)err_level);
  }
}


static void EL_clear_tlog_(EL_ERROR_LEVEL err_level)
{
  EL_TimeSeriesLog* p_tlog = &event_logger_.tlogs[err_level];

  p_tlog->is_table_overflow = 0;
  p_tlog->log_wp = 0;

  memset(p_tlog->events, 0x00, p_tlog->log_capacity * sizeof(EL_Event));
}
#endif


#ifdef EL_IS_ENABLE_CLOG
static void EL_record_event_to_clog_(const EL_Event* event)
{
  uint16_t log_idx;
  const EL_ERROR_LEVEL err_level = event->err_level;
  EL_CLogElement* p_newest_clog_log;

  EL_move_to_front_in_clog_(event);

  // 自身のログidx，つまり最も新しいログidxを拾う
  log_idx = event_logger_.clogs[err_level].log_orders[0];
  p_newest_clog_log = &event_logger_.clogs[err_level].logs[log_idx];

  p_newest_clog_log->count++;
  p_newest_clog_log->delta_record_time = OBCT_diff(&p_newest_clog_log->event.time, &event->time);

  p_newest_clog_log->event = *event;
}


static void EL_clear_all_clog_(void)
{
  uint8_t err_level;
  for (err_level = 0; err_level < EL_ERROR_LEVEL_MAX; ++err_level)
  {
    EL_clear_clog_((EL_ERROR_LEVEL)err_level);
  }
}


static void EL_clear_clog_(EL_ERROR_LEVEL err_level)
{
  EL_CumulativeLog* p_clog = &event_logger_.clogs[err_level];
  uint16_t i;
  const uint16_t capacity = p_clog->log_capacity;

  memset(p_clog->logs, 0x00, capacity * sizeof(EL_CLogElement));

  for (i = 0; i < capacity; ++i)
  {
    p_clog->log_orders[i] = (uint16_t)i;
  }
}


static EL_CLOG_LOG_ACK EL_search_clog_(const EL_Event* event, uint16_t* log_idx, uint16_t* order_idx)
{
  uint16_t i;
  EL_CumulativeLog* p_clog = &event_logger_.clogs[event->err_level];
  const uint16_t capacity = p_clog->log_capacity;
  *log_idx = 0;
  *order_idx = 0;

  for (i = 0; i < capacity; ++i)
  {
    uint16_t idx = p_clog->log_orders[i];
    if (p_clog->logs[idx].event.group == (EL_GROUP)EL_CORE_GROUP_NULL)
    {
      return EL_CLOG_LOG_ACK_NOT_FOUND;
    }
    if (p_clog->logs[idx].event.group == event->group &&
        p_clog->logs[idx].event.local == event->local)
    {
      *log_idx  = idx;
      *order_idx = i;
      return EL_CLOG_LOG_ACK_OK;
    }
  }

  return EL_CLOG_LOG_ACK_NOT_FOUND;
}


static void EL_move_to_front_in_clog_(const EL_Event* event)
{
  uint16_t log_idx;
  uint16_t order_idx;
  EL_CumulativeLog* p_clog = &event_logger_.clogs[event->err_level];
  EL_CLOG_LOG_ACK log_ret = EL_search_clog_(event, &log_idx, &order_idx);

  if (log_ret == EL_CLOG_LOG_ACK_NOT_FOUND)
  {
    // ログがないので作る
    EL_create_clog_on_front_(event);
    return;
  }

  memmove(&p_clog->log_orders[1], &p_clog->log_orders[0], sizeof(uint16_t) * order_idx);
  p_clog->log_orders[0] = log_idx;
}


static void EL_create_clog_on_front_(const EL_Event* event)
{
  uint16_t log_idx;
  const EL_ERROR_LEVEL err_level = event->err_level;
  EL_CumulativeLog* p_clog = &event_logger_.clogs[err_level];
  const uint16_t capacity = p_clog->log_capacity;

  // 最も古いログを取得
  log_idx = p_clog->log_orders[capacity - 1];

#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  {
    const EL_CLogElement* drop_clog_log = &p_clog->logs[log_idx];
    if (drop_clog_log->event.group != (EL_GROUP)EL_CORE_GROUP_NULL)
    {
      // 一応記録のためにイベントを発行する
      // ここで， EL も出してしまうと再帰 & 最悪無限ループになる
      if (err_level != EL_ERROR_LEVEL_EL)
      {
        EL_record_event_((EL_GROUP)EL_CORE_GROUP_EL_DROP_CLOG1,
                         (uint32_t)drop_clog_log->event.group,
                         EL_ERROR_LEVEL_EL,
                         (uint32_t)drop_clog_log->event.err_level);
        EL_record_event_((EL_GROUP)EL_CORE_GROUP_EL_DROP_CLOG2,
                         drop_clog_log->event.local,
                         EL_ERROR_LEVEL_EL,
                         (uint32_t)drop_clog_log->count);
      }
    }
  }
#endif

  memmove(&p_clog->log_orders[1], &p_clog->log_orders[0], sizeof(uint16_t) * (capacity - 1));
  p_clog->log_orders[0] = log_idx;

  // ここで event をいれてしまう
  // そうすることで，初回の delta_record_time が 0 になる
  p_clog->logs[log_idx].event = *event;
  p_clog->logs[log_idx].count = 0;

  // delta_record_time はとりあえずは消してないが，直後に上書きされるはず
}
#endif


static void EL_clear_latest_event_(void)
{
  memset(&event_logger_.latest_event, 0x00, sizeof(EL_Event));
}


static void EL_clear_statistics_(void)
{
  memset(&event_logger_.statistics, 0x00, sizeof(EL_EventStatistics));
  EH_match_event_counter_to_el();
}


EL_ACK EL_enable_logging(EL_GROUP group)
{
  uint32_t group_idx    = (uint32_t)group / 8;
  uint32_t group_subidx = 7 - (uint32_t)group % 8;    // 反転
  uint8_t  info;
  uint8_t  mask;

  if (group >= EL_GROUP_MAX) return EL_ACK_ILLEGAL_GROUP;
  if (group <= (EL_GROUP)EL_CORE_GROUP_NULL) return EL_ACK_ILLEGAL_GROUP;     // これは本当に入れてよいか要議論

  info = event_logger_.is_logging_enable[group_idx];
  mask = (uint8_t)(0x01 << group_subidx);
  info = (uint8_t)(info | mask);

  event_logger_.is_logging_enable[group_idx] = info;

  return EL_ACK_OK;
}


EL_ACK EL_disable_logging(EL_GROUP group)
{
  uint32_t group_idx    = (uint32_t)group / 8;
  uint32_t group_subidx = 7 - (uint32_t)group % 8;    // 反転
  uint8_t  info;
  uint8_t  mask;

  if (group >= EL_GROUP_MAX) return EL_ACK_ILLEGAL_GROUP;
  if (group <= (EL_GROUP)EL_CORE_GROUP_NULL) return EL_ACK_ILLEGAL_GROUP;     // これは本当に入れてよいか要議論

  info = event_logger_.is_logging_enable[group_idx];
  mask = (uint8_t)(0x01 << group_subidx);
  mask = (uint8_t)(~mask);                     // ビット反転
  info = (uint8_t)(info & mask);

  event_logger_.is_logging_enable[group_idx] = info;

  return EL_ACK_OK;
}


int EL_is_logging_enable(EL_GROUP group)
{
  uint32_t group_idx    = (uint32_t)group / 8;
  uint32_t group_subidx = 7 - (uint32_t)group % 8;    // 反転
  uint8_t  info;
  uint8_t  mask;
  uint8_t  ret;

  // 不正な group は無効判定
  if (group >= EL_GROUP_MAX) return 0;

  info = event_logger_.is_logging_enable[group_idx];
  mask = (uint8_t)(0x01 << group_subidx);
  ret  = (uint8_t)(info & mask);

  if (ret == 0) return 0;
  return 1;
}


void EL_enable_all_logging(void)
{
  memset(event_logger_.is_logging_enable, 0xFF, sizeof(event_logger_.is_logging_enable));
}


void EL_disable_all_logging(void)
{
  memset(event_logger_.is_logging_enable, 0x00, sizeof(event_logger_.is_logging_enable));
}


#ifdef EL_IS_ENABLE_TLOG
EL_ACK EL_enable_tlog_overwrite(EL_ERROR_LEVEL err_level)
{
  if (err_level < 0)                   return EL_ACK_ILLEGAL_ERROR_LEVEL;
  if (err_level >= EL_ERROR_LEVEL_MAX) return EL_ACK_ILLEGAL_ERROR_LEVEL;

  event_logger_.tlogs[err_level].is_enable_overwrite = 1;
  return EL_ACK_OK;
}


EL_ACK EL_disable_tlog_overwrite(EL_ERROR_LEVEL err_level)
{
  if (err_level < 0)                   return EL_ACK_ILLEGAL_ERROR_LEVEL;
  if (err_level >= EL_ERROR_LEVEL_MAX) return EL_ACK_ILLEGAL_ERROR_LEVEL;

  event_logger_.tlogs[err_level].is_enable_overwrite = 0;
  return EL_ACK_OK;
}


void EL_enable_tlog_overwrite_all(void)
{
  uint8_t err_level;

  for (err_level = 0; err_level < EL_ERROR_LEVEL_MAX; ++err_level)
  {
    if ((EL_ERROR_LEVEL)err_level == EL_ERROR_LEVEL_EH) continue;
    EL_enable_tlog_overwrite((EL_ERROR_LEVEL)err_level);
  }
}


void EL_disable_tlog_overwrite_all(void)
{
  uint8_t err_level;

  for (err_level = 0; err_level < EL_ERROR_LEVEL_MAX; ++err_level)
  {
    if ((EL_ERROR_LEVEL)err_level == EL_ERROR_LEVEL_EH) continue;
    EL_disable_tlog_overwrite((EL_ERROR_LEVEL)err_level);
  }
}


int EL_is_tlog_overwrite_enable(EL_ERROR_LEVEL err_level)
{
  if (err_level < 0)                   return 0;
  if (err_level >= EL_ERROR_LEVEL_MAX) return 0;

  return (int)event_logger_.tlogs[err_level].is_enable_overwrite;
}


const EL_Event* EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL err_level, uint16_t n)
{
  uint16_t capacity;
  uint16_t idx;
  EL_TimeSeriesLog* p_tlog;

  if (err_level < 0 || err_level >= EL_ERROR_LEVEL_MAX)
  {
    // 仕方がないので HIGHを
    err_level = EL_ERROR_LEVEL_HIGH;
  }

  p_tlog = &event_logger_.tlogs[err_level];
  capacity = p_tlog->log_capacity;

  if (n >= capacity)
  {
    // 仕方がないので，最新のものを
    idx = 0;
  }
  else
  {
    idx = (uint16_t)((p_tlog->log_wp - 1 - n + capacity) % capacity);
  }

  return &p_tlog->events[idx];
}
#endif


CCP_CmdRet Cmd_EL_INIT(const CommonCmdPacket* packet)
{
  (void)packet;
  EL_initialize();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_EL_CLEAR_LOG_ALL(const CommonCmdPacket* packet)
{
  (void)packet;

#ifdef EL_IS_ENABLE_TLOG
  EL_clear_all_tlog_();
#endif

#ifdef EL_IS_ENABLE_CLOG
  EL_clear_all_clog_();
#endif

  EL_clear_latest_event_();
  EL_clear_statistics_();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_EL_CLEAR_LOG_BY_ERR_LEVEL(const CommonCmdPacket* packet)
{
  EL_ERROR_LEVEL err_level = (EL_ERROR_LEVEL)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (err_level < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (err_level >= EL_ERROR_LEVEL_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

#ifdef EL_IS_ENABLE_TLOG
  EL_clear_tlog_(err_level);
#endif

#ifdef EL_IS_ENABLE_CLOG
  EL_clear_clog_(err_level);
#endif

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_EL_CLEAR_STATISTICS(const CommonCmdPacket* packet)
{
  (void)packet;
  EL_clear_statistics_();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


#ifdef EL_IS_ENABLE_TLOG
CCP_CmdRet Cmd_EL_CLEAR_TLOG(const CommonCmdPacket* packet)
{
  EL_ERROR_LEVEL err_level = (EL_ERROR_LEVEL)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (err_level < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (err_level >= EL_ERROR_LEVEL_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  EL_clear_tlog_(err_level);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#endif


#ifdef EL_IS_ENABLE_CLOG
CCP_CmdRet Cmd_EL_CLEAR_CLOG(const CommonCmdPacket* packet)
{
  EL_ERROR_LEVEL err_level = (EL_ERROR_LEVEL)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (err_level < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (err_level >= EL_ERROR_LEVEL_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  EL_clear_clog_(err_level);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#endif


CCP_CmdRet Cmd_EL_RECORD_EVENT(const CommonCmdPacket* packet)
{
  EL_GROUP group = (EL_GROUP)CCP_get_param_from_packet(packet, 0, uint32_t);
  uint32_t local = CCP_get_param_from_packet(packet, 1, uint32_t);
  EL_ERROR_LEVEL err_level = (EL_ERROR_LEVEL)CCP_get_param_from_packet(packet, 2, uint8_t);
  uint32_t note = CCP_get_param_from_packet(packet, 3, uint32_t);

  EL_ACK ack = EL_record_event(group, local, err_level, note);

  switch (ack)
  {
  case EL_ACK_OK:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case EL_ACK_TLOG_FULL:
    // 要検討だが，これは正常ではあるのでこれでよし
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case EL_ACK_ILLEGAL_GROUP:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  case EL_ACK_ILLEGAL_ERROR_LEVEL:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  case EL_ACK_DISABLE_LOGGING:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }
}


#ifdef EL_IS_ENABLE_TLOG
CCP_CmdRet Cmd_EL_TLOG_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page_no = CCP_get_param_from_packet(packet, 0, uint8_t);
  EL_ERROR_LEVEL err_level = (EL_ERROR_LEVEL)CCP_get_param_from_packet(packet, 1, uint8_t);

  switch (err_level)
  {
  case EL_ERROR_LEVEL_HIGH:
    if (page_no >= EL_TLOG_TLM_PAGE_MAX_HIGH) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
  case EL_ERROR_LEVEL_MIDDLE:
    if (page_no >= EL_TLOG_TLM_PAGE_MAX_MIDDLE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
#endif
  case EL_ERROR_LEVEL_LOW:
    if (page_no >= EL_TLOG_TLM_PAGE_MAX_LOW) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  case EL_ERROR_LEVEL_EL:
    if (page_no > (EL_TLOG_LOG_SIZE_MAX_EL - 1) / EL_TLOG_TLM_PAGE_SIZE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
#endif
  case EL_ERROR_LEVEL_EH:
    if (page_no > (EL_TLOG_LOG_SIZE_MAX_EH - 1) / EL_TLOG_TLM_PAGE_SIZE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  event_logger_.tlm_info.tlog.page_no = page_no;
  event_logger_.tlm_info.tlog.err_level = err_level;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#endif


#ifdef EL_IS_ENABLE_CLOG
CCP_CmdRet Cmd_EL_CLOG_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page_no = CCP_get_param_from_packet(packet, 0, uint8_t);
  EL_ERROR_LEVEL err_level = (EL_ERROR_LEVEL)CCP_get_param_from_packet(packet, 1, uint8_t);

  switch (err_level)
  {
  case EL_ERROR_LEVEL_HIGH:
    if (page_no >= EL_CLOG_TLM_PAGE_MAX_HIGH) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
  case EL_ERROR_LEVEL_MIDDLE:
    if (page_no >= EL_CLOG_TLM_PAGE_MAX_MIDDLE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
#endif
  case EL_ERROR_LEVEL_LOW:
    if (page_no >= EL_CLOG_TLM_PAGE_MAX_LOW) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  case EL_ERROR_LEVEL_EL:
    if (page_no > (EL_CLOG_LOG_SIZE_MAX_EL - 1) / EL_CLOG_TLM_PAGE_SIZE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
#endif
  case EL_ERROR_LEVEL_EH:
    if (page_no > (EL_CLOG_LOG_SIZE_MAX_EH - 1) / EL_CLOG_TLM_PAGE_SIZE) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
    break;
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  event_logger_.tlm_info.clog.page_no = page_no;
  event_logger_.tlm_info.clog.err_level = err_level;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#endif


CCP_CmdRet Cmd_EL_INIT_LOGGING_SETTINGS(const CommonCmdPacket* packet)
{
  (void)packet;
  EL_enable_all_logging();
  EL_load_default_settings();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_EL_ENABLE_LOGGING(const CommonCmdPacket* packet)
{
  EL_GROUP group = (EL_GROUP)CCP_get_param_from_packet(packet, 0, uint32_t);

  EL_ACK ack = EL_enable_logging(group);

  switch (ack)
  {
  case EL_ACK_OK:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case EL_ACK_ILLEGAL_GROUP:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }
}


CCP_CmdRet Cmd_EL_DISABLE_LOGGING(const CommonCmdPacket* packet)
{
  EL_GROUP group = (EL_GROUP)CCP_get_param_from_packet(packet, 0, uint32_t);

  EL_ACK ack = EL_disable_logging(group);

  switch (ack)
  {
  case EL_ACK_OK:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case EL_ACK_ILLEGAL_GROUP:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }
}


CCP_CmdRet Cmd_EL_ENABLE_LOGGING_ALL(const CommonCmdPacket* packet)
{
  (void)packet;
  EL_enable_all_logging();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_EL_DISABLE_LOGGING_ALL(const CommonCmdPacket* packet)
{
  (void)packet;
  EL_disable_all_logging();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


#ifdef EL_IS_ENABLE_TLOG
CCP_CmdRet Cmd_EL_ENABLE_TLOG_OVERWRITE(const CommonCmdPacket* packet)
{
  EL_ERROR_LEVEL err_level = (EL_ERROR_LEVEL)CCP_get_param_from_packet(packet, 0, uint8_t);

  EL_ACK ack = EL_enable_tlog_overwrite(err_level);

  switch (ack)
  {
  case EL_ACK_OK:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case EL_ACK_ILLEGAL_ERROR_LEVEL:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }
}


CCP_CmdRet Cmd_EL_DISABLE_TLOG_OVERWRITE(const CommonCmdPacket* packet)
{
  EL_ERROR_LEVEL err_level = (EL_ERROR_LEVEL)CCP_get_param_from_packet(packet, 0, uint8_t);

  EL_ACK ack = EL_disable_tlog_overwrite(err_level);

  switch (ack)
  {
  case EL_ACK_OK:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case EL_ACK_ILLEGAL_ERROR_LEVEL:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }
}


CCP_CmdRet Cmd_EL_ENABLE_TLOG_OVERWRITE_ALL(const CommonCmdPacket* packet)
{
  (void)packet;
  EL_enable_tlog_overwrite_all();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_EL_DISABLE_TLOG_OVERWRITE_ALL(const CommonCmdPacket* packet)
{
  (void)packet;
  EL_disable_tlog_overwrite_all();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#endif

#pragma section
