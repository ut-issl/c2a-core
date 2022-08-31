/**
 * @file
 * @brief イベント発火型処理を行う
 * @note  event_logger の情報を元に指定した BC (対応 BC) を展開する (Event 発火に応じて対応を行う)
 * @note  EH のベストプラクティス
 *          - いろんな箇所で FDIR を組むと，何が発動しているかわかりにくかったり，ログが散逸するので，
 *            できる限りすべての対応を EH で行うと良い
 *            - EH で対応を行う，ということは，以下のログが必ず取れている，ということになる
 *              - EH 対応をトリガするための Event 発行ログ (Event)
 *              - EH にマッチしたという Event ログ (Event)
 *              - EH 対応したという EH 対応ログと対応結果
 *            - ただし， EH 対応を行うためには， Block Cmd を消費するため，メモリ制約の厳しい OBC の場合は，難しい可能性もある
 *              - その場合は， EH_RESPONSE_CONDITION_SINGLE のもの（つまり，ステートレスな対応）を優先して， App 内部でハードコードすることを推奨する
 *              - EH_RESPONSE_CONDITION_SINGLE でないものは，カウンタを実装しなくてはいけないため， EH で処理するほうが望ましい
 *          - init, activate と inactivate
 *            - その EH Rule を 使う前に， init or activate することをおすすめする
 *            - init 時に，内部のカウンタ (EH_Rule.counter) がリセットされるため， EH_RESPONSE_CONDITION_CONTINUOUS, EH_RESPONSE_CONDITION_CUMULATIVE において適切な状態からスタートできる
 *            - モード線維持の SL や，コンポーネントを ON する BC などで init cmd を仕込ませておくと良い
 *            - 一方で，内部処理でカウンタをリセットせずに再度 EH Rule を有効化したい場合は， activate を用いる
 *              - 多段 EH 対応時に， Lv.1 で使う対応 BC を Lv.2 でも呼び出したりする場合に，カウンタをリセットしたくないことがある
 *          - 対応 BC 展開後の activate について
 *            - EH Rule がマッチし，対応 BC が展開された時，その EH_RULE_ID は自動的に inactivate される
 *            - そのため，常に対応し続けたいような Rule については，その対応 BC のどこか（基本的には不感時間などを考慮して末尾など）で activate (or init) する cmd を仕込ませておくと良い
 *          - EH_clear_rule_counter_by_event は適切に使うととても便利（詳細はこの関数の doxygen コメントを参照）
 * @note  多段の EH 対応の組み方
 *        多段の EH を組む場合，
 *          EH_RuleSettings.event.group: EL_CORE_GROUP_EH_MATCH_RULE
 *          EH_RuleSettings.event.local: 下位の EH_RULE_ID
 *          EH_RuleSettings.event.err_level: EL_ERROR_LEVEL_EH
 *        を指定する．
 *        そうすると，上位のルールがマッチした場合，該当するルール対応は実行されなくなる
 *        例えば， UART 不通を考える
 *        設定として
 *          - UART 不通が発生したら EL_UART Event を発行する (Event Logger)
 *          - EL_UART が 5 回発生したら UART ドライバリセットを行う EH_Rule1 を設定する
 *          - EH_Rule1 の対応 BC で EH_Rule1 を再度有効化するようにする
 *          - EH_Rule1 のマッチ (group: EL_CORE_GROUP_EH_MATCH_RULE, local: EH_Rule1 の EH_RULE_ID) が 3 回発生したら UART 回路リセットを行う EH_Rule2 を設定する
 *        とした場合，
 *          1. EL_UART が 5 回発生したら， EH_Rule1 が発火
 *          2. EL_UART が 10 回発生したら， EH_Rule1 が発火
 *          3. EL_UART が 15 回発生したら， EH_Rule1 は発火せずに， EH_Rule2 が発火
 *          4. EL_UART が 20 回発生したら， EH_Rule1 が発火
 *          5. ...
 *        というようになる
 *        このように， EL_CORE_GROUP_EH_MATCH_RULE で発火する EH_Rule は上位ルールとして解釈，実行され，
 *        上位ルールが実行されるときは，下位ルールは実行されなくなる．
 *        また，これは 2 段のみならず， 3 段以上も可能である．
 *        この手法を応用すると，ルールのオーバーライドできる．
 *        例えば，設定として
 *          - UART不通が発生したら EL_UART を発行する
 *          - EL_UART が 5 回発生したら UART ドライバリセットを行う EH_Rule1 を設定する
 *          - EH_Rule1 の対応 BC で EH_Rule1 を再度有効化するようにする
 *          - EH_Rule1 のマッチ (group: EL_CORE_GROUP_EH_MATCH_RULE, local: EH_Rule1 の EH_RULE_ID) が 1 回発生したら UART 回路リセットを行う EH_Rule2 を設定する
 *        とすると，
 *          - EL_UART が 5 回発生したら， EH_Rule1 は発火せずに， EH_Rule2 が発火
 *        となるので，実質的に， EH_Rule2 で EH_Rule1 をオーバーライドすることができる．
 *        このように，下位のルールを，上位の発火条件を変えることで，柔軟にオーバーライドできる．
 * @note  上記の多段の EH 対応の場合に発行される EH Event は，次のようになる．
 *        group / local / err_level / note の順に記す．また，時系列降順でしるす（上が新しい）
 *          1. EL_UART が 5 回発生したら， EH_Rule1 が発火
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule1 / EH / err_level
 *          2. EL_UART が 10 回発生したら， EH_Rule1 が発火
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule1 / EH / err_level
 *          3. EL_UART が 15 回発生したら， EH_Rule1 は発火せずに， EH_Rule2 が発火
 *                EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE / EH_Rule1 / EH / 1
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule2 / EH / err_level
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule1 / EH / err_level
 *          4. EL_UART が 20 回発生したら， EH_Rule1 が発火
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule1 / EH / err_level
 *          5. EL_UART が 25 回発生したら， EH_Rule1 が発火
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule1 / EH / err_level
 *          6. EL_UART が 30 回発生したら， EH_Rule1 は発火せずに， EH_Rule2 も発火せずに， EH_Rule3 が発火
 *                EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE / EH_Rule1 / EH / 1
 *                EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE / EH_Rule2 / EH / 1
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule3 / EH / err_level
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule2 / EH / err_level
 *                EL_CORE_GROUP_EH_MATCH_RULE / EH_Rule1 / EH / err_level
 * @note  多段の EH 対応のため， EL_ERROR_LEVEL_EH の Event をルールに設定することは，
 *        多段対応時 (つまり group == EL_CORE_GROUP_EH_MATCH_RULE) を除いてできない
 * @note  by_event_group 関数について
 *        複数の EH_Rule が同じ 対応 BC を設定している場合，その BC ではそのすべての EH_Rule を再度 activate させないといけない．
 *        それを１つ１つ指定するのはめんどくさいし，漏れが起きやすいので，それらの EH_Rule.settings で共通の event.group を指定しまとめ，
 *        event.group 単位で操作できるようにしたもの．
 *        なお， group での操作によって，想定外の EH_Rule まで操作されないように注意すること
 * @note  EH での Event 発行は以下
 *          - EL_CORE_GROUP_EVENT_HANDLER
 *            - EH に関する様々なエラー
 *            - local id は EH_EL_LOCAL_ID
 *          - EL_CORE_GROUP_EH_MATCH_RULE
 *            - EH_Rule にマッチ
 *            - local id は EH_RULE_ID
 *          - EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE
 *            - EH_Rule にマッチしたが，さらに上位の EH_Rule のマッチ条件を満たしたため，発火はキャンセルされた（上位で発火される）
 *            - local id は EH_RULE_ID
 */
#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include "event_logger.h"

#ifdef EL_IS_ENABLE_TLOG

#include <stddef.h>
#include "../TimeManager/obc_time.h"
#include "../../TlmCmd/block_command_table.h"
#include <src_user/Settings/System/EventHandlerRules/event_handler_rules.h>

#define EH_RULE_TLM_PAGE_SIZE (20)  //!< event_handler のルールテーブルの1テレメトリパケット(=1ページ)に格納されるルール数（ページネーション用）
#define EH_RULE_TLM_PAGE_MAX  (8)   //!< event_handler のルールテーブルのページ数（ページネーション用）

#define EH_LOG_TLM_PAGE_SIZE  (64)  //!< EH対応のログテーブルの1テレメトリパケット(=1ページ)に格納されるログ数（ページネーション用）
#define EH_LOG_TLM_PAGE_MAX   (2)   //!< EH対応のログテーブルのページ数（ページネーション用）

#define EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES   (4)     //!< EL_Event の ID が重複した EH_Rule を最大何個まで重複させてよいか (uint8_t を想定)
#define EH_MAX_RESPONSE_NUM_DEFAULT           (8)     //!< 一度の実行で対応する最大数（初期値） (uint8_t を想定)
#define EH_MAX_CHECK_EVENT_NUM_DEFAULT        (64)    /*!< 一度の実行でチェックする event_logger の event log の最大値（初期値） (uint16_t を想定)
                                                           TL内での実行時間を調整するために設定する． */
#define EH_MAX_MULTI_LEVEL_NUM_DEFAULT        (4)     //!< 多段の EH 対応の設定可能な最大段数（初期値） (uint8_t を想定)

// 以下のファイルにて，次のパラメタを上書き設定できる
// EH_RULE_TLM_PAGE_SIZE
// EH_RULE_TLM_PAGE_MAX
// EH_LOG_TLM_PAGE_SIZE
// EH_LOG_TLM_PAGE_MAX
// EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES
// EH_MAX_RESPONSE_NUM_DEFAULT
// EH_MAX_CHECK_EVENT_NUM_DEFAULT
// EH_MAX_MULTI_LEVEL_NUM_DEFAULT
#include <src_user/Settings/System/event_handler_params.h>

#define EH_RULE_MAX   (EH_RULE_ID)(EH_RULE_TLM_PAGE_SIZE * EH_RULE_TLM_PAGE_MAX)
                                              /*!< 最大何個のルール ( EL_Event - EH_Rule 対応) を保持できるか
                                                   基本的に， EH_RULE_ID として使うので，キャストする
                                                   enum上で定義しないのは， EH_Rule の最大値を，初期に定義する数よりも多くしたいため
                                                   また，この値は， Rule 未定義値（ないしは初期値）としても使う */
#define EH_LOG_MAX    (EH_LOG_TLM_PAGE_SIZE * EH_LOG_TLM_PAGE_MAX)  //!< 最大何個の EH 対応ログを保持できるか

/**
 * @enum   EH_REGISTER_ACK
 * @brief  EH_register_rule の返り値
 * @note   uint8_t を想定
 */
typedef enum
{
  EH_REGISTER_ACK_OK = 0,                   //!< 正常に登録完了
  EH_REGISTER_ACK_ERR_FULL,                 //!< これ以上登録できない
  EH_REGISTER_ACK_ERR_RULE_OVERWRITE,       //!< すでに同じ ID にルールが登録されているため棄却
  EH_REGISTER_ACK_ERR_DUPLICATE_FULL,       //!< これ以上， EL_Event の重複した EH_Rule を登録できない
  EH_REGISTER_ACK_ILLEGAL_RULE_ID,          //!< 不正な EH_RULE_ID
  EH_REGISTER_ACK_ILLEGAL_GROUP,            //!< 不正な EL_Event.group
  EH_REGISTER_ACK_ILLEGAL_ERROR_LEVEL,      //!< 不正な EL_Event.err_level
  EH_REGISTER_ACK_ILLEGAL_MATCH_FLAG,       //!< 不正な should_match_err_level
  EH_REGISTER_ACK_ILLEGAL_CONDITION_TYPE,   //!< 不正な EH_RESPONSE_CONDITION_TYPE
  EH_REGISTER_ACK_ILLEGAL_COUNT_THRESHOLD,  //!< 不正な condition.count_threshold
  EH_REGISTER_ACK_ILLEGAL_BCT_ID,           //!< 不正な deploy_bct_id
  EH_REGISTER_ACK_ILLEGAL_ACTIVE_FLAG,      //!< 不正な is_active
  EH_REGISTER_ACK_ILLEGAL_MULTI_LEVEL,      //!< 多段 EH 対応での不正な設定
  EH_REGISTER_ACK_UNKNOWN_ERR               //!< 不明なエラー
} EH_REGISTER_ACK;

/**
 * @enum   EH_CHECK_RULE_ACK
 * @brief  EH_Rule 登録状況確認用
 * @note   EH_{in}activate_rule の返り値などにも用いる
 * @note   uint8_t を想定
 */
typedef enum
{
  EH_CHECK_RULE_ACK_OK = 0,           //!< 正常 or ルール登録済み
  EH_CHECK_RULE_ACK_INVALID_RULE_ID,  //!< EH_RULE_ID が不正
  EH_CHECK_RULE_ACK_UNREGISTERED      //!< ルールが未登録
} EH_CHECK_RULE_ACK;

/**
 * @enum   EH_RESPONSE_CONDITION_TYPE
 * @brief  EL_Event 登録に対して，どのように EH 対応させるかの種類
 * @note   uint8_t を想定
 */
typedef enum
{
  EH_RESPONSE_CONDITION_SINGLE,       //!< Event 登録に対して即座に EH 対応
  EH_RESPONSE_CONDITION_CONTINUOUS,   //!< Event 登録後 N 秒以内に再び同じ Event 登録が発生することが M 回連続で発生した場合に EH 対応
  EH_RESPONSE_CONDITION_CUMULATIVE,   //!< Event が M 回登録した場合に EH 対応
  EH_RESPONSE_CONDITION_TYPE_MAX
} EH_RESPONSE_CONDITION_TYPE;

/**
 * @struct EH_RuleSettings
 * @brief  EH_register_rule の引数．Rule登録に必要な情報
 */
typedef struct
{
  struct
  {
    EL_GROUP group;                           //!< 引っ掛ける EL_Event.group
    uint32_t local;                           //!< 引っ掛ける EL_Event.local
    EL_ERROR_LEVEL err_level;                 /*!< 引っ掛ける EL_Event.err_level
                                                   should_match_err_level == 0 のときはなんでもよい */
  } event;                                    //!< EH に対応する EL_Event
  uint8_t should_match_err_level;             //!< EL_Event の一致に，エラーレベル一致を含めるか？
  struct
  {
    EH_RESPONSE_CONDITION_TYPE type;          //!< EH 対応の条件種類
    uint16_t count_threshold;                 /*!< 条件に回数がある場合に使う，対応までの Event 回数閾値
                                                   type == EH_RESPONSE_CONDITION_SINGLE のときはなんでもよい */
    uint32_t time_threshold_ms;               /*!< 連続発生とみなす時間 [ms]
                                                   type == EH_RESPONSE_CONDITION_CONTINUOUS のときのみ有効 */
  } condition;                                //!< EH 対応条件
  bct_id_t deploy_bct_id;                     //!< EH 対応に展開する BCT id
  uint8_t  is_active;                         //!< 対応が有効か？ 対応後，自動的に無効になる
} EH_RuleSettings;

/**
 * @struct EH_Rule
 * @brief  EL_Event に対して対応する EH_Rule
 */
typedef struct
{
  EH_RuleSettings settings;                   //!< 設定値
  uint16_t counter;                           //!< EH 対応までのカウンタ
  ObcTime  last_event_time;                   //!< 最後に発生した EL_Event の時刻
} EH_Rule;

/**
 * @struct EH_RuleTable
 * @brief  EH_Rule のテーブル
 */
typedef struct
{
  EH_Rule  rules[EH_RULE_MAX];      //!< idx は EH_RULE_ID
  uint16_t registered_rule_num;     //!< 登録済み rule 数
} EH_RuleTable;

/**
 * @struct EH_RuleSortIndex
 * @brief  EH_RuleTable から EH_Rule を二分探索して検索できるようにするためのソート済みindex
 * @note   {group, local, duplicate_id} から rule_id を引くためにつかう
 * @note   sorted_idxes[registered_rule_num] 以降の未登録は， group == EL_CORE_GROUP_NULL, rule_id == EH_RULE_MAX と初期化
 */
typedef struct
{
  EL_GROUP group;           //!< EH_Rule.event.group
  uint32_t local;           //!< EH_Rule.event.local
  uint8_t  duplicate_id;    //!< 同じ {group. local} の rule がある場合の，重複ID
  EH_RULE_ID rule_id;       //!< EventHandler.rules の idx
} EH_RuleSortedIndex;

/**
 * @struct EH_Log
 * @brief  EH 対応のログ
 */
typedef struct
{
  EH_RULE_ID   rule_id;
  cycle_t      respond_time_in_master_cycle;
  CCP_EXEC_STS deploy_cmd_ack;        // FIXME: 将来的に，最後に BC 展開したときのエラー情報が取得できるようになったら，それにする
} EH_Log;

/**
 * @struct EH_LogTable
 * @brief  EH 対応のログテーブル
 */
typedef struct
{
  EH_Log   logs[EH_LOG_MAX];
  uint32_t respond_counter;
  uint16_t log_wp;
} EH_LogTable;

/**
 * @struct EH_ElEventCounter
 * @brief  EL イベントカウンタ． EL のイベント記録情報と照らし合わせて，対応すべきイベントを探索するために使う
 * @note   EL_EventStatistics に対応する
 */
typedef struct
{
  uint32_t counter_total;                  //!< 前回 EH_execute 実行時の EL でのイベント記録数
  uint32_t counters[EL_ERROR_LEVEL_MAX];   //!< エラーレベルごとの記録数
} EH_ElEventCounter;

/**
 * @struct EH_ExecSettings
 * @brief  EH 実行時設定
 */
typedef struct
{
  uint8_t  max_response_num;      /*!< 一度の EH_execute の実行で対応する最大数
                                       初期値は EH_MAX_RESPONSE_NUM_DEFAULT
                                       EL_Event のIDが重複した EH_Rule もあるので，実際の対応数はこれを上回る可能性がある */
  uint16_t max_check_event_num;   /*!< 一度の実行でチェックする event_logger の event log の最大値
                                       初期値は EH_MAX_CHECK_EVENT_NUM_DEFAULT */
  uint8_t  max_multi_level_num;   /*!< 多段の EH 対応の設定可能な最大段数
                                       初期値は EH_MAX_MULTI_LEVEL_NUM_DEFAULT */
} EH_ExecSettings;

/**
 * @struct EH_ExecInfo
 * @brief  EH 実行時情報
 */
typedef struct
{
  uint8_t current_multi_level;   //!< 現在処理してる多段 EH 対応レベル
} EH_ExecInfo;

/**
 * @struct EH_RegisterFromCmd
 * @brief  コマンド経由で EH_Rule を登録するときに使う内部状態変数
 */
typedef struct
{
  EH_RULE_ID      rule_id;      //!< 登録先 EH_RULE_ID
  EH_RuleSettings settings;     //!< 登録する EH_RuleSettings
  EH_REGISTER_ACK register_ack; //!< EH_register_rule の返り値
} EH_RegisterFromCmd;

/**
 * @struct EH_TlmInfo
 * @brief  tlmのページネーションの情報保持
 */
typedef struct
{
  struct
  {
    uint8_t page_no;
    EH_RULE_ID target_rule_id;
  } rule;
  struct
  {
    uint8_t page_no;
  } rule_sorted_index;
  struct
  {
    uint8_t page_no;
  } log;
} EH_TlmInfo;

/**
 * @struct EventHandler
 * @brief  EventHandler のInfo構造体
 */
typedef struct
{
  EH_RuleTable       rule_table;                //!< EH_Rule のテーブル
  EH_RuleSortedIndex sorted_idxes[EH_RULE_MAX]; //!< rules の二分探索用 idx
  EH_LogTable        log_table;                 //!< EH 対応ログテーブル
  EH_ElEventCounter  el_event_counter;          //!< EH_ElEventCounter
  EH_ExecSettings    exec_settings;             //!< 実行時設定
  EH_ExecInfo        exec_info;                 //!< 実行時情報
  EH_RegisterFromCmd reg_from_cmd;              //!< コマンド経由で EH_Rule を登録するときに使う内部状態変数
  EH_TlmInfo         tlm_info;                  //!< tlmのための情報
} EventHandler;

extern const EventHandler* const event_handler;


/**
 * @brief  event_handler の初期化
 * @param  void
 * @return void
 */
void EH_initialize(void);

/**
 * @brief  event_handler の実行
 *
 *         EL にて登録されたイベントを走査し，対応するBCを展開する
 * @note   event_utility app などから定期実行されることを想定
 * @param  void
 * @return void
 */
void EH_execute(void);

/**
 * @brief  ルールの登録
 * @note   基本的には， EH_load_default_rules 内部でのみ使われることを想定
 * @param  settings: EH_RuleSettings
 * @param  id: EH_RULE_ID
 * @return EH_REGISTER_ACK
 */
EH_REGISTER_ACK EH_register_rule(EH_RULE_ID id, const EH_RuleSettings* settings);

/**
 * @brief  ルールの初期化
 *
 *         EH_activate_rule した後， EH_clear_rule_counter が実行される
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @param  id: EH_RULE_ID
 * @return EH_CHECK_RULE_ACK
 */
EH_CHECK_RULE_ACK EH_init_rule(EH_RULE_ID id);

/**
 * @brief  ルールの初期化 (multi-level)
 *
 *         EH_activate_rule した後， EH_clear_rule_counter が実行される
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @param  id: EH_RULE_ID
 * @return EH_CHECK_RULE_ACK
 */
EH_CHECK_RULE_ACK EH_init_rule_for_multi_level(EH_RULE_ID id);

/**
 * @brief  EH_RuleSettings.event.group 指定による一括でのルールの初期化
 *
 *         EH_activate_rule した後， EH_clear_rule_counter が実行される
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @note   group での操作によって，想定外の EH_Rule まで操作されないように注意すること
 * @param  group: EL_GROUP
 * @return void
 */
void EH_init_rule_by_event_group(EL_GROUP group);

/**
 * @brief  EH_RuleSettings.event.group 指定による一括でのルールの初期化 (multi-level)
 *
 *         EH_activate_rule した後， EH_clear_rule_counter が実行される
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @note   group での操作によって，想定外の EH_Rule まで操作されないように注意すること
 * @param  group: EL_GROUP
 * @return void
 */
void EH_init_rule_by_event_group_for_multi_level(EL_GROUP group);

/**
 * @brief  ルールの有効化
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @note   rule_counter はクリアされない
 * @param  id: EH_RULE_ID
 * @return EH_CHECK_RULE_ACK
 */
EH_CHECK_RULE_ACK EH_activate_rule(EH_RULE_ID id);

/**
 * @brief  ルールの無効化
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @param  id: EH_RULE_ID
 * @return EH_CHECK_RULE_ACK
 */
EH_CHECK_RULE_ACK EH_inactivate_rule(EH_RULE_ID id);

/**
 * @brief  ルールの有効化 (multi-level)
 * @note   多段の場合，指定した EH_RULE_ID より下位のすべてのルールを有効化
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @param  id: EH_RULE_ID
 * @return EH_CHECK_RULE_ACK
 */
EH_CHECK_RULE_ACK EH_activate_rule_for_multi_level(EH_RULE_ID id);

/**
 * @brief  ルールの無効化 (multi-level)
 * @note   多段の場合，指定した EH_RULE_ID より下位のすべてのルールを無効化
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @param  id: EH_RULE_ID
 * @return EH_CHECK_RULE_ACK
 */
EH_CHECK_RULE_ACK EH_inactivate_rule_for_multi_level(EH_RULE_ID id);

/**
 * @brief  EH_RuleSettings.event.group 指定による一括でのルールの有効化
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @note   rule_counter はクリアされない
 * @note   group での操作によって，想定外の EH_Rule まで操作されないように注意すること
 * @param  group: EL_GROUP
 * @return void
 */
void EH_activate_rule_by_event_group(EL_GROUP group);

/**
 * @brief  EH_RuleSettings.event.group 指定による一括でのルールの無効化
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @note   group での操作によって，想定外の EH_Rule まで操作されないように注意すること
 * @param  group: EL_GROUP
 * @return void
 */
void EH_inactivate_rule_by_event_group(EL_GROUP group);

/**
 * @brief  EH_RuleSettings.event.group 指定による一括でのルールの有効化 (multi-level)
 * @note   多段の場合，指定した EL_GROUP で指定された EH_RULE_ID より下位のすべてのルールを有効化
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @note   group での操作によって，想定外の EH_Rule まで操作されないように注意すること
 * @param  group: EL_GROUP
 * @return void
 */
void EH_activate_rule_by_event_group_for_multi_level(EL_GROUP group);

/**
 * @brief  EH_RuleSettings.event.group 指定による一括でのルールの無効化 (multi-level)
 * @note   多段の場合，指定した EL_GROUP で指定された EH_RULE_ID より下位のすべてのルールを無効化
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @note   group での操作によって，想定外の EH_Rule まで操作されないように注意すること
 * @param  group: EL_GROUP
 * @return void
 */
void EH_inactivate_rule_by_event_group_for_multi_level(EL_GROUP group);

/**
 * @brief ルールが有効かどうか取得する
 * @param[in]  id: EH_RULE_ID
 * @return uint8_t: 有効かどうか
 * @note ID が無効の場合は inactive (0) を返す
 */
uint8_t EH_get_rule_is_active(EH_RULE_ID id);

/**
 * @brief  EH_Rule の counter をセット
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @param  id: EH_RULE_ID
 * @param  counter: セットする counter
 * @return EH_CHECK_RULE_ACK
 */
EH_CHECK_RULE_ACK EH_set_rule_counter(EH_RULE_ID id, uint16_t counter);

/**
 * @brief  EH_Rule の counter を 0 クリア
 * @note   基本的にはコマンドで操作するので，直接使うことはあまり想定していない
 * @param  id: EH_RULE_ID
 * @return EH_CHECK_RULE_ACK
 */
EH_CHECK_RULE_ACK EH_clear_rule_counter(EH_RULE_ID id);

/**
 * @brief  EH_Rule の counter を EL_Event 指定で 0 クリア
 * @note   もしアノマリが発生したら， EL_record_event, 正常なら EH_clear_rule_counter_by_event という使い方を想定している
 * @note   そのため，EL_record_event とインターフェースをあわせている
 * @param  group: EL_Event.group
 * @param  local: EL_Event.local
 * @param  err_level: EL_Event.err_level
 * @return EH_CHECK_RULE_ACK
 */
void EH_clear_rule_counter_by_event(EL_GROUP group, uint32_t local, EL_ERROR_LEVEL err_level);

/**
 * @brief  イベントカウンタを EL のそれに合わせる
 * @note   EL 側をリセットした際に呼び出さないと，不整合が発生する（まあ，勝手に解消されるけど）
 * @param  void
 * @return void
 */
void EH_match_event_counter_to_el(void);

/**
 * @brief  EH_LogTable の最新からn番目の対応ログを取得
 * @note   n が大きすぎる場合は，最新のものを返す
 * @param  n: n番目（0のとき最新）
 * @return EH_Log
 */
const EH_Log* EH_get_the_nth_log_from_the_latest(uint16_t n);


CCP_CmdRet Cmd_EH_INIT(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_CLEAR_ALL_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_LOAD_DEFAULT_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_REGISTER_RULE_EVENT_PARAM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_REGISTER_RULE_CONDITION_PARAM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_REGISTER_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_DELETE_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_INIT_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_INIT_RULE_FOR_MULTI_LEVEL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_ACTIVATE_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_INACTIVATE_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_RULE_COUNTER(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_CLEAR_RULE_COUNTER(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_CLEAR_RULE_COUNTER_BY_EVENT(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_CLEAR_LOG(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_MAX_RESPONSE_NUM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_MAX_CHECK_EVENT_NUM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_MAX_MULTI_LEVEL_NUM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_PAGE_OF_RULE_TABLE_FOR_TLM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_PAGE_OF_LOG_TABLE_FOR_TLM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM(const CommonCmdPacket* packet);

/**
 * @brief 新しい EL_Event 発生を検出するためのカウンタを強制的に EL のカウンタに合わせる
 */
CCP_CmdRet Cmd_EH_MATCH_EVENT_COUNTER_TO_EL(const CommonCmdPacket* packet);

// by_event_group 関数
CCP_CmdRet Cmd_EH_INIT_RULE_BY_EVENT_GROUP(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_INIT_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_ACTIVATE_RULE_BY_EVENT_GROUP(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_INACTIVATE_RULE_BY_EVENT_GROUP(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_ACTIVATE_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_EH_INACTIVATE_RULE_BY_EVENT_GROUP_FOR_MULTI_LEVEL(const CommonCmdPacket* packet);

#endif  // EL_IS_ENABLE_TLOG

#endif
