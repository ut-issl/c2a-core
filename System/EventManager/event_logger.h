/**
 * @file
 * @brief アノマリやエラーなどの各種イベントを記録する
 * @note  このイベントをもとに event_handler を発火させることができる
 * @note  ログは TLog (TimeSeriesLog) と CLog (EL_CumulativeLog) の二種類を準備し，必要なもののみを使う
 * @note  今後， NvLog (不揮発Log) が実装予定
 * @note  TLog (TimeSeriesLog)
 *        時系列ログ
 *        ひたすら，新しい EL_Event をテーブルに追記していく．
 *        EL_get_the_nth_tlog_from_the_latest で最新からn番目が取得できる．
 *        これを有効化しないと， event_handler は使えない．
 *        テーブルが満杯になった場合，上書きするかしないかは is_enable_overwrite にて選択可能．
 * @note  CLog (EL_CumulativeLog)
 *        累積ログ
 *        EL_Event (EL_Event.group, EL_Event.local にて特定) ごとの累積カウンタを保持し，時系列ソートされる．
 *        テーブルが満杯になると最も古いものが捨てられる．
 *        以下のような動きをする
 *          フォーマット:
 *            | log index | event | counter | time | dt |
 *          1. 時刻 t1: event1 が発生
 *             | 0 | event1 | 1 | t1 | 0 |
 *          2. 時刻 t2: event2 が発生
 *             | 0 | event2 | 1 | t2 | 0 |
 *             | 1 | event1 | 1 | t1 | 0 |
 *          3. 時刻 t3: event2 が発生
 *             | 0 | event2 | 2 | t3 | t3 - t2 |
 *             | 1 | event1 | 1 | t1 | 0       |
 *          4. 時刻 t4: event1 が発生
 *             | 0 | event1 | 2 | t4 | t4 - t1 |
 *             | 1 | event2 | 2 | t3 | t3 - t2 |
 *          5. 時刻 t5: event3 が発生
 *             | 0 | event3 | 1 | t5 | 0       |
 *             | 1 | event1 | 2 | t4 | t4 - t1 |
 *             | 2 | event2 | 2 | t3 | t3 - t2 |
 */
#ifndef EVENT_LOGGER_H_
#define EVENT_LOGGER_H_

// TODO: FALSHメモリ等の大容量不揮発ボリュームへの書き込み
// TODO: EL そのもの全体をビルドする，しないをえらべる ifdef を追加（メモリのため）
// TODO: ObcTime の初期化が OBCT_clear ではなく memset 0x00 である（現状問題ないが，今後問題になるかも）

#include <stddef.h>
#include <stdint.h>
#include "../../TlmCmd/common_cmd_packet.h"
#include "../TimeManager/obc_time.h"

// EL_GROUP (uint32_t を想定) をここで定義する
#include <src_user/Settings/System/event_logger_group.h>

#define EL_TLOG_TLM_PAGE_SIZE         (28)    //!< TLogにて，テレメ１ページにおろせるlog数
#define EL_TLOG_TLM_PAGE_MAX_HIGH     (4)     //!< TLogで保存するログのページ数 (EL_ERROR_LEVEL_HIGH)
#define EL_TLOG_TLM_PAGE_MAX_MIDDLE   (4)     //!< TLogで保存するログのページ数 (EL_ERROR_LEVEL_MIDDLE)
#define EL_TLOG_TLM_PAGE_MAX_LOW      (4)     //!< TLogで保存するログのページ数 (EL_ERROR_LEVEL_LOW)
#define EL_CLOG_TLM_PAGE_SIZE         (18)    //!< CLogにて，テレメ１ページにおろせるlog数
#define EL_CLOG_TLM_PAGE_MAX_HIGH     (4)     //!< CLogで保存するログのページ数 (EL_ERROR_LEVEL_HIGH)
#define EL_CLOG_TLM_PAGE_MAX_MIDDLE   (4)     //!< CLogで保存するログのページ数 (EL_ERROR_LEVEL_MIDDLE)
#define EL_CLOG_TLM_PAGE_MAX_LOW      (4)     //!< CLogで保存するログのページ数 (EL_ERROR_LEVEL_LOW)

#define EL_TLOG_LOG_SIZE_MAX_EL       (16)    //!< TLogにて保存するログ数 (EL_ERROR_LEVEL_EL)
#define EL_CLOG_LOG_SIZE_MAX_EL       (8)     //!< CLogにて保存するログ数 (EL_ERROR_LEVEL_EL)
#define EL_TLOG_LOG_SIZE_MAX_EH       (16)    //!< TLogにて保存するログ数 (EL_ERROR_LEVEL_EH) 小さめでOK． EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES x 多段EHの最大段数ぐらいあれば
#define EL_CLOG_LOG_SIZE_MAX_EH       (8)     //!< CLogにて保存するログ数 (EL_ERROR_LEVEL_EH) これは EH では使われないので小さくて良い

#define EL_IS_ENABLE_TLOG                     //!< TLog を有効にするか？ → これがないと event_handler は使えない
#define EL_IS_ENABLE_CLOG                     //!< CLog を有効にするか？
// #define EL_IS_ENABLE_NVLOG                 //!< TODO: NvLog (不揮発Log) を有効にするか？
#define EL_IS_ENABLE_EVENT_NOTE               //!< EL_Event.note を有効にするか？ event としては (event_handler などで) 同じだが，補足情報や区別用の id を使うか？
#define EL_IS_ENABLE_MIDDLE_ERROR_LEVEL       //!< EL_ERROR_LEVEL_MIDDLE を有効にするか？ （C2A coreでは使わないので，基本無効でOK．使いたいなら有効に）
#define EL_IS_ENABLE_EL_ERROR_LEVEL           /*!< EL_ERROR_LEVEL_EL を有効にするか？
                                                   再帰呼び出し & 無限ループを防ぐために，EL内部のイベントは別の専用のエラーレベルで記録される．
                                                   これを有効化すると，EL内部のアノマリを記録できる． */

#define SIZE_OF_EL_CLOG_COUNTER_T      (2)    //!< el_clog_counter_t の型サイズ．メモリインパクトがそれなりにあるので，可変に

// 以下 event_logger_params.h にて，以下のような設定を定義する．主にメモリインパクトのために調整可能にしている．
// なお，きちんと動作確認されているのは，EL_IS_ENABLE_MIDDLE_ERROR_LEVEL のみ無効なモードのとき
// #define EL_IS_ENABLE_TLOG: TLog を有効にする？
// #define EL_IS_ENABLE_CLOG: CLog を有効にする？
// #define EL_IS_ENABLE_EVENT_NOTE: EL_Event.note を有効にする？
// #define EL_IS_ENABLE_MIDDLE_ERROR_LEVEL: EL_ERROR_LEVEL_MIDDLE を使えるようにするか？
// #define EL_IS_ENABLE_EL_ERROR_LEVEL: EL_ERROR_LEVEL_EL を使えるようにするか？
// SIZE_OF_EL_CLOG_COUNTER_T: el_clog_counter_t の型サイズ
// 上部で定義されている，各種tableサイズ
#include <src_user/Settings/System/event_logger_params.h>

#define EL_LOG_MINIMUM_REQUIRED           (2)   //!< ログに求められる最小要素数
#define EL_LOG_MINIMUM_REQUIRED_EL_LOG    (4)   //!< ELログに求められる最小要素数

#ifdef EL_IS_ENABLE_TLOG

#if EL_TLOG_TLM_PAGE_SIZE < EL_LOG_MINIMUM_REQUIRED
#error EL_TLOG_TLM_PAGE_SIZE must be >= EL_LOG_MINIMUM_REQUIRED
#endif

#define EL_TLOG_LOG_SIZE_MAX_HIGH     (EL_TLOG_TLM_PAGE_SIZE * EL_TLOG_TLM_PAGE_MAX_HIGH)
#if EL_TLOG_TLM_PAGE_MAX_HIGH <= 0
#error EL_TLOG_TLM_PAGE_MAX_HIGH must be a positive number
#endif
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
#define EL_TLOG_LOG_SIZE_MAX_MIDDLE   (EL_TLOG_TLM_PAGE_SIZE * EL_TLOG_TLM_PAGE_MAX_MIDDLE)
#if EL_TLOG_TLM_PAGE_MAX_MIDDLE <= 0
#error EL_TLOG_TLM_PAGE_MAX_MIDDLE must be a positive number
#endif
#else   // EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
#undef EL_TLOG_TLM_PAGE_MAX_MIDDLE
#endif  // EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
#define EL_TLOG_LOG_SIZE_MAX_LOW      (EL_TLOG_TLM_PAGE_SIZE * EL_TLOG_TLM_PAGE_MAX_LOW)
#if EL_TLOG_TLM_PAGE_MAX_LOW <= 0
#error EL_TLOG_TLM_PAGE_MAX_LOW must be a positive number
#endif

#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
#if EL_TLOG_LOG_SIZE_MAX_EL < EL_LOG_MINIMUM_REQUIRED_EL_LOG
#error EL_TLOG_LOG_SIZE_MAX_EL is too small
#endif
#else   // EL_IS_ENABLE_EL_ERROR_LEVEL
#undef EL_TLOG_LOG_SIZE_MAX_EL
#endif  // EL_IS_ENABLE_EL_ERROR_LEVEL

// FIXME: event_logger.h と event_handler.h が相互 include になってしまいビルドできないので，一時的にこのアサーションを無効化
// #include "event_handler.h"
// #if EL_TLOG_LOG_SIZE_MAX_EH <= EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES
// #error EL_TLOG_LOG_SIZE_MAX_EH is too small
// #endif
#if EL_TLOG_LOG_SIZE_MAX_EH <= 0
#error EL_TLOG_LOG_SIZE_MAX_EH must be a positive number
#endif

#else   // EL_IS_ENABLE_TLOG

#undef EL_TLOG_TLM_PAGE_SIZE
#undef EL_TLOG_TLM_PAGE_MAX_HIGH
#undef EL_TLOG_TLM_PAGE_MAX_MIDDLE
#undef EL_TLOG_TLM_PAGE_MAX_LOW
#undef EL_TLOG_LOG_SIZE_MAX_EL
#undef EL_TLOG_LOG_SIZE_MAX_EH

#endif  // EL_IS_ENABLE_TLOG


#ifdef EL_IS_ENABLE_CLOG

#if EL_CLOG_TLM_PAGE_SIZE < EL_LOG_MINIMUM_REQUIRED
#error EL_CLOG_TLM_PAGE_SIZE must be >= EL_LOG_MINIMUM_REQUIRED
#endif

#define EL_CLOG_LOG_SIZE_MAX_HIGH     (EL_CLOG_TLM_PAGE_SIZE * EL_CLOG_TLM_PAGE_MAX_HIGH)
#if EL_CLOG_TLM_PAGE_MAX_HIGH <= 0
#error EL_CLOG_TLM_PAGE_MAX_HIGH must be a positive number
#endif
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
#define EL_CLOG_LOG_SIZE_MAX_MIDDLE   (EL_CLOG_TLM_PAGE_SIZE * EL_CLOG_TLM_PAGE_MAX_MIDDLE)
#if EL_CLOG_TLM_PAGE_MAX_MIDDLE <= 0
#error EL_CLOG_TLM_PAGE_MAX_MIDDLE must be a positive number
#endif
#else   // EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
#undef EL_CLOG_TLM_PAGE_MAX_MIDDLE
#endif  // EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
#define EL_CLOG_LOG_SIZE_MAX_LOW      (EL_CLOG_TLM_PAGE_SIZE * EL_CLOG_TLM_PAGE_MAX_LOW)
#if EL_CLOG_TLM_PAGE_MAX_LOW <= 0
#error EL_CLOG_TLM_PAGE_MAX_LOW must be a positive number
#endif

#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
#if EL_CLOG_LOG_SIZE_MAX_EL < EL_LOG_MINIMUM_REQUIRED_EL_LOG
#error EL_CLOG_LOG_SIZE_MAX_EL is too small
#endif
#else   // EL_IS_ENABLE_EL_ERROR_LEVEL
#undef EL_CLOG_LOG_SIZE_MAX_EL
#endif  // EL_IS_ENABLE_EL_ERROR_LEVEL

#if EL_CLOG_LOG_SIZE_MAX_EH <= 0
#error EL_CLOG_LOG_SIZE_MAX_EH must be a positive number
#endif

#if SIZE_OF_EL_CLOG_COUNTER_T == 1
typedef uint8_t el_clog_counter_t;
#elif SIZE_OF_EL_CLOG_COUNTER_T == 2
typedef uint16_t el_clog_counter_t;
#elif SIZE_OF_EL_CLOG_COUNTER_T == 4
typedef uint32_t el_clog_counter_t;
#else
#error Illegal value for SIZE_OF_EL_CLOG_COUNTER_T
#endif

#else    // EL_IS_ENABLE_CLOG

#undef EL_CLOG_TLM_PAGE_SIZE
#undef EL_CLOG_TLM_PAGE_MAX_HIGH
#undef EL_CLOG_TLM_PAGE_MAX_MIDDLE
#undef EL_CLOG_TLM_PAGE_MAX_LOW
#undef EL_CLOG_LOG_SIZE_MAX_EL
#undef EL_CLOG_LOG_SIZE_MAX_EH

#endif  // EL_IS_ENABLE_CLOG


/**
 * @enum  EL_CORE_GROUP
 * @brief event_logger の Event Group の C2A core 使用分
 * @note  uint32_t を想定
 */
typedef enum
{
  EL_CORE_GROUP_NULL = 0,             //!< 初期値 or NULL．これは 0 であることを要求 (clear で memset 0x00 をするなら)
  EL_CORE_GROUP_EVENT_LOGGER,         //!< EL
  EL_CORE_GROUP_MODE_MANAGER,
  EL_CORE_GROUP_TASK_DISPATCHER,
  EL_CORE_GROUP_APP_MANAGER,
  EL_CORE_GROUP_TLCD_PAST_TIME,
  EL_CORE_GROUP_TLCD_DEPLOY_BLOCK,
  EL_CORE_GROUP_DCU,
  EL_CORE_GROUP_EVENT_HANDLER,        //!< EH （詳細は event_handler.h 参照）
  EL_CORE_GROUP_EH_MATCH_RULE,        //!< EH_Rule でマッチした（詳細は event_handler.h 参照）
  EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE,  //!< EH_Rule でマッチしたが，そのルールで対応せずに，上位のルールで対応させた（詳細は event_handler.h 参照）
  EL_CORE_GROUP_COMMAND_ANALYZE,
  EL_CORE_GROUP_CDIS_INTERNAL_ERR,
  EL_CORE_GROUP_CDIS_EXEC_ERR_STS,
  EL_CORE_GROUP_CDIS_EXEC_ERR_CODE,
  // TODO: Driver Super
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  EL_CORE_GROUP_EL_DROP_CLOG1,        //!< EL CLogs で古いエラーを上書きするとき (group, err_level を保存)
  EL_CORE_GROUP_EL_DROP_CLOG2,        //!< EL CLogs で古いエラーを上書きするとき (local, count を保存)
#endif
  EL_CORE_GROUP_MAX
} EL_CORE_GROUP;


#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
/**
 * @enum  EL_EVENT_LOCAL
 * @brief EL_CORE_GROUP_EVENT_LOGGER の local id
 * @note  uint32_t を想定
 */
typedef enum
{
  EL_EVENT_LOCAL_UNKNOWN_ERR,             //!< 不明のエラー
#ifdef EL_IS_ENABLE_TLOG
  EL_EVENT_LOCAL_TLOG_HIGH_HALF_FULL,     //!< TLog のテーブルの半分が埋まった (HIGH)
  EL_EVENT_LOCAL_TLOG_HIGH_FULL,          //!< TLog のテーブルが埋まった．wpは最初に戻り，古いものから上書きしていく (HIGH)
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
  EL_EVENT_LOCAL_TLOG_MIDDLE_HALF_FULL,   //!< TLog のテーブルの半分が埋まった (MIDDLE)
  EL_EVENT_LOCAL_TLOG_MIDDLE_FULL,        //!< TLog のテーブルが埋まった．wpは最初に戻り，古いものから上書きしていく (MIDDLE)
#endif
  EL_EVENT_LOCAL_TLOG_LOW_HALF_FULL,      //!< TLog のテーブルの半分が埋まった (LOW)
  EL_EVENT_LOCAL_TLOG_LOW_FULL,           //!< TLog のテーブルが埋まった．wpは最初に戻り，古いものから上書きしていく (LOW)
#endif
  EL_EVENT_LOCAL_MAX
} EL_EVENT_LOCAL;
#endif


/**
 * @enum  EL_ERROR_LEVEL
 * @brief event_logger の エラーレベル
 * @note  uint8_t を想定
 * @note  C2A Core では HIGH, LOW しか使わないため， MIDDLE はユーザー定義とする
 */
typedef enum
{
  EL_ERROR_LEVEL_HIGH = 0,
#ifdef EL_IS_ENABLE_MIDDLE_ERROR_LEVEL
  EL_ERROR_LEVEL_MIDDLE,
#endif
  EL_ERROR_LEVEL_LOW,
#ifdef EL_IS_ENABLE_EL_ERROR_LEVEL
  EL_ERROR_LEVEL_EL,
#endif
  EL_ERROR_LEVEL_EH,    /*!< 多段の EH 対応を実現するためのエラーレベル
                             EL_CORE_GROUP_EH_MATCH_RULE, EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE が記録される */
  EL_ERROR_LEVEL_MAX
} EL_ERROR_LEVEL;


/**
 * @enum  EL_ACK
 * @brief event_logger の汎用返り値
 * @note  uint8_t を想定
 */
typedef enum
{
  EL_ACK_OK,                    //!< 正常終了
  EL_ACK_ILLEGAL_GROUP,         //!< 不正な EL_Event.group
  EL_ACK_ILLEGAL_ERROR_LEVEL,   //!< 不正な EL_Event.err_level
  EL_ACK_DISABLE_LOGGING,       //!< ロギングが無効により，棄却
  EL_ACK_TLOG_FULL,             //!< TLogの追記が不能のため，TLog記録ができず
  EL_ACK_OTHER_ERR              //!< その他のエラー
} EL_ACK;


/**
 * @struct EL_Event
 * @brief  イベント
 */
typedef struct
{
  EL_GROUP group;             //!< イベントを特定する group id
  uint32_t local;             /*!< それぞれの group ごとに詳細情報を付与するための id
                                   enumでもよいし，なにかのエラーコードでも良いし，例えば書き込みが失敗したメモリアドレス，などでも良い
                                   groupごとに定義する */
  EL_ERROR_LEVEL err_level;   //!< エラーレベル
  ObcTime  time;              //!< 発生時刻
#ifdef EL_IS_ENABLE_EVENT_NOTE
  uint32_t note;              /*!< Event は group, local で同定されるが，さらに補足情報を追加する場合に使う．
                                   event_handler などでは無視される
                                   補足情報を追加する場合や，同じ event でも発生場所等で区別したい場合に使える． */
#endif
} EL_Event;


/**
 * @struct EL_EventStatistics
 * @brief  イベント統計情報
 */
typedef struct
{
  uint32_t record_counter_total;                //!< イベント記録数
  uint32_t record_counters[EL_ERROR_LEVEL_MAX]; //!< エラーレベルごとの記録数
} EL_EventStatistics;


#ifdef EL_IS_ENABLE_TLOG
/**
 * @struct EL_TimeSeriesLog
 * @brief  時系列ログ (TLog) のログ構造定義
 */
typedef struct
{
  EL_Event* events;               //!< 記録するイベントテーブル（サイズ可変のためポインタ）
  uint16_t  log_capacity;         //!< イベントテーブルのキャパシティ（サイズ）
  uint16_t  log_wp;               //!< イベントテーブルの write pointer．次回書き込み場所
  uint8_t   is_enable_overwrite;  //!< TLogの上書きを許可するか？（デフォルトでは許可）
  uint8_t   is_table_overflow;    //!< 上書きが無効の場合，溢れて追記できなかったか？
} EL_TimeSeriesLog;
#endif


#ifdef EL_IS_ENABLE_CLOG
/**
 * @struct EL_CLogElement
 * @brief  累積ログ (CLog) の１要素
 * note    group, local が同じイベントを同一とみなす
 */
typedef struct
{
  EL_Event          event;                  //!< 記録するイベントと，最後に記録されたイベント情報
  el_clog_counter_t count;                  //!< 累積カウンタ
  ObcTime           delta_record_time;      //!< 直近２つのイベント発生時刻の差
} EL_CLogElement;


/**
 * @struct EL_CumulativeLog
 * @brief  累積ログ (CLog) のログ構造定義
 */
typedef struct
{
  EL_CLogElement* logs;         //!< 累積カウンタを保持するイベントテーブル．[log_order[0]] が最新．（サイズ可変のためポインタ）
  uint16_t*       log_orders;   //!< logs を時系列順に並び替える sort_key（サイズ可変のためポインタ）
  uint16_t        log_capacity; //!< イベントテーブルのキャパシティ（サイズ）
} EL_CumulativeLog;
#endif


typedef struct
{
#ifdef EL_IS_ENABLE_TLOG
  struct
  {
    uint8_t page_no;            //!< tlm で下ろすログページ
    EL_ERROR_LEVEL err_level;   //!< tlm で下ろす情報のエラーレベル
  } tlog;
#endif
#ifdef EL_IS_ENABLE_CLOG
  struct
  {
    uint8_t page_no;            //!< tlm で下ろすログページ
    EL_ERROR_LEVEL err_level;   //!< tlm で下ろす情報のエラーレベル
  } clog;
#endif
} EL_TlmInfo;


typedef struct
{
#ifdef EL_IS_ENABLE_TLOG
  EL_TimeSeriesLog tlogs[EL_ERROR_LEVEL_MAX];
#endif
#ifdef EL_IS_ENABLE_CLOG
  EL_CumulativeLog clogs[EL_ERROR_LEVEL_MAX];
#endif

  EL_EventStatistics statistics;    //!< 統計情報
  EL_TlmInfo tlm_info;              //!< tlmのための情報

  uint8_t is_logging_enable[EL_GROUP_MAX / 8 + 1];  //!< 有効かフラグ．メモリ削減のため， bit 単位で保存

  EL_Event latest_event;
} EventLogger;


extern const EventLogger* const event_logger;


/**
 * @brief  event_logger の初期化
 * @param  void
 * @return void
 */
void EL_initialize(void);

/**
 * @brief  イベント (EL_Event) を記録
 * @note   C2A 上の任意の場所から発行可能
 * @param  group: EL_Event.group
 * @param  local: EL_Event.local
 * @param  err_level: EL_Event.err_level (EL_ERROR_LEVEL_EL は禁止)
 * @param  note: EL_Event.note
 * @return EL_ACK
 */
EL_ACK EL_record_event(EL_GROUP group, uint32_t local, EL_ERROR_LEVEL err_level, uint32_t note);

/**
 * @brief  event_logger の EL_Event.group の有効化
 * @param  group: 有効化する EL_Event.group
 * @retval EL_ACK_OK: 正常終了
 * @retval EL_ACK_ILLEGAL_GROUP: 不正な EL_Event.group
 */
EL_ACK EL_enable_logging(EL_GROUP group);

/**
 * @brief  event_logger の EL_Event.group の無効化
 * @param  group: 無効化する EL_Event.group
 * @retval EL_ACK_OK: 正常終了
 * @retval EL_ACK_ILLEGAL_GROUP: 不正な EL_Event.group
 */
EL_ACK EL_disable_logging(EL_GROUP group);

/**
 * @brief  event_logger の EL_Event.group が有効か？
 * @param  group: 無効化する EL_Event.group
 * @retval 0: 無効 or 無効な引数
 * @retval 1: 有効
 */
int EL_is_logging_enable(EL_GROUP group);

/**
 * @brief  event_logger の全 EL_Event.group の有効化
 * @param  void
 * @return void
 */
void EL_enable_all_logging(void);

/**
 * @brief  event_logger の全 EL_Event.group の無効化
 * @param  void
 * @return void
 */
void EL_disable_all_logging(void);

#ifdef EL_IS_ENABLE_TLOG
/**
 * @brief  TLog のイベントテーブルの上書きを有効化
 * @param  err_level: 有効化する EL_Event.err_level
 * @retval EL_ACK_OK: 正常終了
 * @retval EL_ACK_ILLEGAL_ERROR_LEVEL: 不正な EL_Event.err_level
 */
EL_ACK EL_enable_tlog_overwrite(EL_ERROR_LEVEL err_level);

/**
 * @brief  TLog のイベントテーブルの上書きを無効化
 * @param  err_level: 無効化する EL_Event.err_level
 * @retval EL_ACK_OK: 正常終了
 * @retval EL_ACK_ILLEGAL_ERROR_LEVEL: 不正な EL_Event.err_level
 */
EL_ACK EL_disable_tlog_overwrite(EL_ERROR_LEVEL err_level);

/**
 * @brief  TLog のイベントテーブルの上書きをすべて有効化
 * @note   EL_ERROR_LEVEL_EH は基本的にユーザーが操作するものではないので，ここでは有効化されない
 * @param  void
 * @return void
 */
void EL_enable_tlog_overwrite_all(void);

/**
 * @brief  TLog のイベントテーブルの上書きをすべて無効化
 * @note   EL_ERROR_LEVEL_EH は基本的にユーザーが操作するものではないので，ここでは無効化されない
 * @param  void
 * @return void
 */
void EL_disable_tlog_overwrite_all(void);

/**
 * @brief  TLog のイベントテーブルの上書きが有効か？
 * @param  err_level: 無効化する EL_Event.err_level
 * @retval 0: 無効 or 無効な引数
 * @retval 1: 有効
 */
int EL_is_tlog_overwrite_enable(EL_ERROR_LEVEL err_level);

/**
 * @brief  TLog のイベントテーブルの最新からn番目のイベントを取得
 * @note   n が大きすぎる場合は，最新のものを返す
 * @note   err_level が不正な場合は， EL_ERROR_LEVEL_HIGH のものを返す
 * @note   つまり，アサーションエラーを出さない
 * @param  err_level: 取得する EL_Event.err_level
 * @param  n: n番目（0のとき最新）
 * @return EL_Event
 */
const EL_Event* EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL err_level, uint16_t n);
#endif

CCP_CmdRet Cmd_EL_INIT(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_CLEAR_LOG_ALL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_CLEAR_LOG_BY_ERR_LEVEL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_CLEAR_STATISTICS(const CommonCmdPacket* packet);

#ifdef EL_IS_ENABLE_TLOG
CCP_CmdRet Cmd_EL_CLEAR_TLOG(const CommonCmdPacket* packet);
#endif

#ifdef EL_IS_ENABLE_CLOG
CCP_CmdRet Cmd_EL_CLEAR_CLOG(const CommonCmdPacket* packet);
#endif

CCP_CmdRet Cmd_EL_RECORD_EVENT(const CommonCmdPacket* packet);

#ifdef EL_IS_ENABLE_TLOG
CCP_CmdRet Cmd_EL_TLOG_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);
#endif

#ifdef EL_IS_ENABLE_CLOG
CCP_CmdRet Cmd_EL_CLOG_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);
#endif

CCP_CmdRet Cmd_EL_INIT_LOGGING_SETTINGS(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_ENABLE_LOGGING(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_DISABLE_LOGGING(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_ENABLE_LOGGING_ALL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_DISABLE_LOGGING_ALL(const CommonCmdPacket* packet);

#ifdef EL_IS_ENABLE_TLOG
CCP_CmdRet Cmd_EL_ENABLE_TLOG_OVERWRITE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_DISABLE_TLOG_OVERWRITE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_ENABLE_TLOG_OVERWRITE_ALL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EL_DISABLE_TLOG_OVERWRITE_ALL(const CommonCmdPacket* packet);
#endif

#endif
