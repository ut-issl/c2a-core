#pragma section REPRO
#include "anomaly_handler.h"

#include "../Library/print.h"
#include <src_user/Library/vt100.h>
#include "../System/TimeManager/time_manager.h"
#include "../Library/endian_memcpy.h"
#include "../TlmCmd/common_cmd_packet_util.h"
#include "timeline_command_dispatcher.h"
// #include "../anomaly_logger_group.h"
// #include "../../TlmCmd/block_command_definitions.h"

static void   AH_init_(void);
static void   AH_load_default_rules_(void);
static void   AH_exec_(void);
static size_t AH_check_rules_(size_t record_id);
static int    AH_is_equal_code_(const AL_AnomalyCode* a,
                                const AL_AnomalyCode* b);
static void   AH_respond_to_anomaly_(size_t id);
static void   AH_print_ah_status_(void);
static void   AH_add_rule_(size_t id, const AH_Rule* ahr);
static void   AH_clear_log_(void);
static void   AH_respond_log_clear(void);

static AnomalyHandler anomaly_handler_;
const AnomalyHandler* const anomaly_handler = &anomaly_handler_;

static AH_AnomalyRespondLog AH_respond_log_;
const AH_AnomalyRespondLog* const AH_respond_log = &AH_respond_log_;

static size_t AH_prev_pos_;
static size_t AH_prev_act_;


AppInfo AH_create_app(void)
{
  return AI_create_app_info("ah", AH_init_, AH_exec_);
}


static void AH_init_(void)
{
  int i;

  // 初期設定はすべてのルールを無効化する
  for (i = 0; i < AH_MAX_RULES; ++i)
  {
    anomaly_handler_.elements[i].is_active = 0;
  }

  // デフォルトのルール構成を読み込み
  AH_load_default_rules_();
  // 対応時刻をゼロクリア
  OBCT_clear(&(anomaly_handler_.respond_at));
  // 初期は範囲外に設定し該当なしを表現
  anomaly_handler_.latest_id = AH_MAX_RULES;
  // 対応実行数をクリア
  anomaly_handler_.action_counter = 0;
  // アノマリ対応検知用の変数をクリア
  AH_prev_act_ = 0;
  // アノマリ処理状況を初期化
  AH_clear_log_();
  // テレトリ用ページ番号は0に初期化
  anomaly_handler_.page_no = 0;

  AH_respond_log_.pointer = 0;

  for (i = 0; i < AH_LOG_MAX; i++)
  {
    AH_respond_log_.log[i].respond_at_master = 0;
    AH_respond_log_.log[i].rule_num = 0;
  }
  AH_respond_log_.page_no = 0;
}


static void AH_load_default_rules_(void)
{
  AH_Rule_default_load();
}


static void AH_exec_(void)
{
  const AL_AnomalyRecord* ar = AL_get_record(anomaly_handler_.al_pointer.pos);   // 初期値は走査開始位置

  // アノマリ個数が変化していない場合
  if (anomaly_handler_.al_pointer.count == anomaly_logger->counter) return;
  // 新たなアノマリが発生した場合
  else if (anomaly_handler_.al_pointer.count < anomaly_logger->counter)
  {
    size_t record_id;

    for (record_id = anomaly_handler_.al_pointer.pos; record_id < anomaly_logger->header; ++record_id)
    {
      size_t prev_id = AH_check_rules_(record_id);   // 位置合わせのため．Reprog時の対症療法なので，修正したい
      (void)prev_id;
      ar = AL_get_record(record_id);

      // 最初の一つ目：前回最後に記録された部分．圧縮時はここに記録される
      if (record_id == anomaly_handler_.al_pointer.pos)
      {
        // もしrun_lengthが増えていれば→新たにこのアノマリが発生していれば
        if (ar->run_length > anomaly_handler_.latest_run_length)
        {
          // 対応をする
          size_t id = AH_check_rules_(record_id);

          if (id != AH_MAX_RULES)
          {
            AH_respond_to_anomaly_(id);
          }
        }
      }
      else
      {
        // anomaly_logger->headerが1以上増えている→新たな種類のアノマリが発生している
        size_t id = AH_check_rules_(record_id);

        if (id != AH_MAX_RULES)
        {
          AH_respond_to_anomaly_(id);
        }
      }
    }

    // 処理終了後、最新位置・回数をAnomalyLogger側と同期
    anomaly_handler_.al_pointer.pos = anomaly_logger->header - 1;
    anomaly_handler_.al_pointer.count = anomaly_logger->counter;
    anomaly_handler_.latest_run_length = ar->run_length;
  }
  // 異常: AnomalyHandlerとAnomalyLoggerの関係が崩れている。
  else
  {
    // 仕方がないので、記録をリセットし強制同期する。
    AH_clear_log_();
  }
}


static size_t AH_check_rules_(size_t record_id)
{
  const AL_AnomalyRecord* ar = AL_get_record(record_id);
  int is_latest = (record_id == anomaly_logger->header);
  size_t id;

  // 全ルールを順次走査
  for (id = 0; id < AH_MAX_RULES; ++id)
  {
    AH_Element ahe = anomaly_handler_.elements[id];

    // ルールが無効の場合はスキップ
    if (!(ahe.is_active)) continue;
    // コード不一致の場合はスキップ
    else if (!AH_is_equal_code_(&(ahe.rule.code), &(ar->code))) continue;

    // コードに対応するルールが存在。判定条件を評価。
    switch (ahe.rule.cond)
    {
    case AH_SINGLE:
      // コードが存在した時点で無条件合致。
      return id;

    case AH_CONTINUOUS:

      // 連続回数を超過した場合に条件合致。
      if (ahe.rule.threshold < ar->run_length) return id;

      break;

    case AH_CUMULATE:

      // 積算回数を超過した場合に条件合致。
      if (ahe.counter <= ar->run_length)
      {
        return id;
      }
      // 最新レコードに対する比較でない場合は積算数を更新する。
      else if (!is_latest)
      {
        ahe.counter -= ar->run_length;
      }

      break;
    }
  }

  return AH_MAX_RULES; // 該当なしの場合は登録上限を返す。
}


static int AH_is_equal_code_(const AL_AnomalyCode* a,
                             const AL_AnomalyCode* b)
{
  return ((a->group == b->group) && (a->local == b->local));
}


static void AH_respond_to_anomaly_(size_t id)
{
  // これが呼ばれてるには，anomaly_handler_.elements[].is_active == 1は保証されている．

  // 対応ブロックコマンドをリアルタイムコマンドで展開
  CommonCmdPacket packet;
  // 通常BCなのでTLC1に展開
  CCP_form_block_deploy_cmd(&packet, TL_ID_DEPLOY_BC, anomaly_handler_.elements[id].rule.bc_id);
  PH_dispatch_command(&packet);

  // 実行したルールを記録し回数を更新
  anomaly_handler_.respond_at = TMGR_get_master_clock();
  anomaly_handler_.latest_id = id;
  ++anomaly_handler_.action_counter;

  if (AH_respond_log_.pointer < AH_LOG_MAX)
  {
    AH_respond_log_.log[AH_respond_log_.pointer].respond_at_master = TMGR_get_master_total_cycle();
    AH_respond_log_.log[AH_respond_log_.pointer].rule_num = id;
    AH_respond_log_.pointer += 1;
  }

  // 合致したルールは無効化する。
  anomaly_handler_.elements[id].is_active = 0;
}


AppInfo print_ah_status(void)
{
  return AI_create_app_info("ahs", NULL, AH_print_ah_status_);
}


static void AH_print_ah_status_(void)
{
  VT100_erase_line();
  Printf("AH: ID %d, ACT %d, AT <%d, %d, %d>, POS %d, CTR %d\n",
         anomaly_handler_.latest_id, anomaly_handler_.action_counter,
         anomaly_handler_.respond_at.total_cycle, anomaly_handler_.respond_at.mode_cycle, anomaly_handler_.respond_at.step,
         anomaly_handler_.al_pointer.pos, anomaly_handler_.al_pointer.count);

  // 新種のアノマリが発生した場合警告音1回
  if (AH_prev_pos_ != anomaly_handler_.al_pointer.pos)
  {
    Printf("\a");
    AH_prev_pos_ = anomaly_handler_.al_pointer.pos;
  }

  // アノマリ対応が発生した場合警告音2回
  if (AH_prev_act_ != anomaly_handler_.action_counter)
  {
    Printf("\a\a");
    AH_prev_act_ = anomaly_handler_.action_counter;
  }
}


CCP_EXEC_STS Cmd_AH_REGISTER_RULE(const CommonCmdPacket* packet)
{
  enum
  {
    ID, GROUP, LOCAL, COND, THRESHOLD, BC
  };
  const uint8_t* param = CCP_get_param_head(packet);
  AH_Rule ahr;

  if (CCP_get_param_len(packet) != (5 + SIZE_OF_BCT_ID_T))
  {
    // パラメータ長確認(6Bytes)
    return CCP_EXEC_ILLEGAL_LENGTH;
  }
  else if (param[ID] >= AH_MAX_RULES)
  {
    // 登録指定位置が許容範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
  else if (param[COND] > AH_CUMULATE)
  {
    // 判定条件が定義されたものと一致しない
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  ahr.code.group = (uint32_t)param[GROUP];
  ahr.code.local = (uint32_t)param[LOCAL];
  ahr.cond       = (AH_CONDITION)param[COND];
  ahr.threshold  = (size_t)param[THRESHOLD];
  endian_memcpy(&(ahr.bc_id), &(param[BC]), SIZE_OF_BCT_ID_T);

  AH_add_rule_((size_t)param[ID], &ahr);

  return CCP_EXEC_SUCCESS;
}


static void AH_add_rule_(size_t id, const AH_Rule* ahr)
{
  anomaly_handler_.elements[id].is_active = 0; // 登録時点では無効とする
  anomaly_handler_.elements[id].rule = *ahr;
  anomaly_handler_.elements[id].counter = ahr->threshold;
}


CCP_EXEC_STS Cmd_AH_ACTIVATE_RULE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t id;

  id = param[0];            // 非明示的なcast
  if (id >= AH_MAX_RULES)
  {
    // 指定位置が範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  AH_activate_rule(id);
  return CCP_EXEC_SUCCESS;
}


void AH_activate_rule(size_t id)
{
  anomaly_handler_.elements[id].is_active = 1;
  anomaly_handler_.elements[id].counter = anomaly_handler_.elements[id].rule.threshold;
}


CCP_EXEC_STS Cmd_AH_INACTIVATE_RULE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t id;

  id = param[0];            // 非明示的なcast
  if (id >= AH_MAX_RULES)
  {
    // 指定位置が範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  AH_inactivate_rule(id);
  return CCP_EXEC_SUCCESS;
}


void AH_inactivate_rule(size_t id)
{
  anomaly_handler_.elements[id].is_active = 0;
}


CCP_EXEC_STS Cmd_AH_CLEAR_LOG(const CommonCmdPacket* packet)
{
  (void)packet;
  AH_clear_log_();
  return CCP_EXEC_SUCCESS;
}


static void AH_clear_log_(void)
{
  // AnomalyHnadler内の保持情報を初期化
  anomaly_handler_.al_pointer.count = 0;
  anomaly_handler_.al_pointer.pos = 0;
  // 前回のrun_lengthを初期化
  anomaly_handler_.latest_run_length = 0;
  // アノマリ発生検知用変数を初期化
  AH_prev_pos_ = 0;
  // AnomalyLoggerのリストをクリア
  AL_clear();
}


CCP_EXEC_STS Cmd_AH_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page;

  page = CCP_get_param_head(packet)[0];

  if (page >= AH_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  anomaly_handler_.page_no = page;
  return CCP_EXEC_SUCCESS;
}


static void AH_respond_log_clear(void)
{
  int i = 0;
  for (i = 0; i < AH_LOG_MAX; i++)
  {
    AH_respond_log_.log[i].respond_at_master = 0;
    AH_respond_log_.log[i].rule_num = 0;
  }
    AH_respond_log_.pointer = 0;
}


CCP_EXEC_STS Cmd_AHRES_LOG_CLEAR(const CommonCmdPacket* packet)
{
  (void)packet;
  AH_respond_log_clear();
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_AHRES_LOG_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page;

  page = CCP_get_param_head(packet)[0];

  if (page >= AH_LOG_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  AH_respond_log_.page_no = page;
  return CCP_EXEC_SUCCESS;
}


// 2019-02-04
// ユーザー定義部分離により，追加
void AH_add_rule(size_t id, const AH_Rule* ahr, uint8_t is_active)
{
  AH_add_rule_(id, ahr);
  anomaly_handler_.elements[id].is_active = is_active;
}

#pragma section
