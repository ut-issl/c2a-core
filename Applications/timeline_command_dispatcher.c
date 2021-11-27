#pragma section REPRO
#include "timeline_command_dispatcher.h"

#include <string.h> // for memcpy

#include "../CmdTlm/packet_handler.h"
#include "../System/TimeManager/time_manager.h"
#include "../System/AnomalyLogger/anomaly_logger.h"
#include "../System/EventManager/event_logger.h"
#include "../Library/endian_memcpy.h"

static CommandDispatcher timeline_command_dispatcher_[TL_ID_MAX];
const CommandDispatcher* const timeline_command_dispatcher = timeline_command_dispatcher_;

static int TLCD_line_no_for_tlm_;
const int* TLCD_line_no_for_tlm;
static cycle_t TLCD_tl_tlm_updated_at_;
const cycle_t* TLCD_tl_tlm_updated_at;
static CTCP TLCD_null_packet_;
const CTCP* TLCD_tl_list_for_tlm[PH_TL0_CMD_LIST_MAX]; // TL0が最長なのでそれに合わせる。
static int TLCD_page_no_;
const int* TLCD_page_no;

static void TLCD0_init_(void);
static void TLCD0_dispatch_(void);
static void TLCD1_init_(void);
static void TLCD1_dispatch_(void);
static void TLCD2_init_(void);
static void TLCD2_dispatch_(void);
static void tlc_dispatcher_(int line_no);
static PH_ACK drop_tl_cmd_at_(int line_no, cycle_t time);

AppInfo TLCD0_create_app(void)
{
  return create_app_info("tlc0d",
                         TLCD0_init_,
                         TLCD0_dispatch_);
}

static void TLCD0_init_(void)
{
  // TLC0 Dispatcherの初期化処理
  timeline_command_dispatcher_[0] = CDIS_init(&(PH_tl_cmd_list[0]));

  // タイムラインコマンドのテレメトリ変数初期設定
  // すべてのタイムラインで共用なのでTL0で代表して初期化
  TLCD_line_no_for_tlm = &TLCD_line_no_for_tlm_;
  TLCD_line_no_for_tlm_ = 0;
  TLCD_tl_tlm_updated_at = &TLCD_tl_tlm_updated_at_;
  memset(&TLCD_null_packet_, 0, sizeof(TLCD_null_packet_));
  TLCD_update_tl_list_for_tlm(0);

  // テレメトリページ番号を初期値0に設定
  TLCD_page_no_ = 0;
  TLCD_page_no = &TLCD_page_no_;
}

static void TLCD0_dispatch_(void)
{
  tlc_dispatcher_(0);
}

AppInfo TLCD1_create_app(void)
{
  return create_app_info("tlc1d",
                         TLCD1_init_,
                         TLCD1_dispatch_);
}

static void TLCD1_init_(void)
{
  timeline_command_dispatcher_[1] = CDIS_init(&(PH_tl_cmd_list[1]));
}

static void TLCD1_dispatch_(void)
{
  tlc_dispatcher_(1);
}

AppInfo TLCD2_create_app(void)
{
  return create_app_info("tlc2d",
                         TLCD2_init_,
                         TLCD2_dispatch_);
}

static void TLCD2_init_(void)
{
  timeline_command_dispatcher_[2] = CDIS_init(&(PH_tl_cmd_list[2]));
}

static void TLCD2_dispatch_(void)
{
  tlc_dispatcher_(2);
}

static void tlc_dispatcher_(int line_no)
{
  PL_ACK ack = PL_check_tl_cmd(&(PH_tl_cmd_list[line_no]),
                               (size_t)(TMGR_get_master_total_cycle()));

  switch (ack)
  {
  case PL_TLC_ON_TIME:
    // 実行予定時刻ちょうど
    CDIS_dispatch_command(&timeline_command_dispatcher_[line_no]);
    break;

  case PL_TLC_PAST_TIME:

    // 実行予定時刻超過
    if (timeline_command_dispatcher_[line_no].lockout == 0)
    {
      // Lockout無効の場合はアノマリーを記録。
      // Lockout有効の場合はアノマリーの連続生成を防ぐため時刻超過状態でもアノマリーを記録しない。
#ifndef AL_DISALBE_AT_C2A_CORE
      AL_add_anomaly(AL_CORE_GROUP_TLCD, (uint32_t)line_no);
#endif
      EL_record_event((EL_GROUP)EL_CORE_GROUP_TLCD_PAST_TIME,
                      (uint32_t)line_no,
                      EL_ERROR_LEVEL_LOW,
                      (uint32_t)CCP_get_ti(&PL_get_head(&PH_tl_cmd_list[line_no])->packet));

      if (timeline_command_dispatcher_[line_no].stop_on_error == 1)
      {
        // Lockout無効かつSOE有効の場合
        // Lockout有効とし、タイムラインコマンド処理を停止。
        timeline_command_dispatcher_[line_no].lockout = 1;
      }
    }

    // コマンド実行処理呼び出し。
    // Lockoutとなった場合は呼び出してもコマンドは実行されない。
    CDIS_dispatch_command(&timeline_command_dispatcher_[line_no]);
    break;

  case PL_TLC_NOT_YET:
    // 実行すべきコマンドなし。
    break;

  default:
    break;
  }
}

uint8_t TLCD_update_tl_list_for_tlm(uint8_t line_no)
{
  PL_Node* pos;
  int i;

  if (line_no >= TL_ID_MAX) return TL_ID_MAX;

  pos = (PL_Node*)PL_get_head(&(PH_tl_cmd_list[line_no]));
  // テレメ情報生成時刻を記録
  TLCD_tl_tlm_updated_at_ = TMGR_get_master_total_cycle();

  // 全リスト内容をクリア
  for (i = 0; i < PH_TL0_CMD_LIST_MAX; ++i)
  {
    TLCD_tl_list_for_tlm[i] = &TLCD_null_packet_;
  }

  // 登録されているTLコマンドをリストに書き込み
  for (i = 0; pos != NULL; ++i)
  {
    TLCD_tl_list_for_tlm[i] = &(pos->packet);
    pos = pos->next;
  }

  return line_no;
}

CCP_EXEC_STS Cmd_TLCD_CLEAR_ALL_TIMELINE(const CTCP* packet)
{
  int line_no;

  line_no = CCP_get_param_head(packet)[0];

  if (line_no >= TL_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  CDIS_clear_command_list(&timeline_command_dispatcher_[line_no]);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TLCD_CLEAR_TIMELINE_AT(const CTCP* packet)
{
  const unsigned char* param = CCP_get_param_head(packet);
  int line_no;
  cycle_t time;

  // ライン番号を読み込み
  line_no = param[0]; // 非明示的なcast

  if (line_no >= TL_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // 指定TIを読み込み
  endian_memcpy(&time, packet + 1, sizeof(time));

  if (drop_tl_cmd_at_(line_no, time) == PH_SUCCESS)
  {
    return CCP_EXEC_SUCCESS;
  }
  else
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
}

static PH_ACK drop_tl_cmd_at_(int line_no, cycle_t time)
{
  int i;

  PL_Node* prev = NULL;
  PL_Node* current = (PL_Node*)PL_get_head(&(PH_tl_cmd_list[line_no])); // const_cast
  int active_nodes_num = PL_count_active_nodes(&PH_tl_cmd_list[line_no]);

  if (current == NULL) return PH_TLC_NOT_FOUND;

  for (i = 0; i < active_nodes_num; ++i)
  {
    if (CCP_get_ti(&(current->packet)) == time)
    {
      PL_drop_node(&(PH_tl_cmd_list[line_no]), prev, current);
      break;
    }
    if (PL_get_next(current) == NULL) return PH_TLC_NOT_FOUND;

    prev = current;
    current = current->next;
  }

  return PH_SUCCESS;
}


CCP_EXEC_STS Cmd_TLCD_SET_SOE_FLAG(const CTCP* packet)
{
  const unsigned char* param = CCP_get_param_head(packet);
  uint8_t line_no;
  uint8_t flag;

  // パラメータ読み出し&内容確認
  line_no = param[0];

  if (line_no >= TL_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  flag = param[1];

  if ((flag != 0) && (flag != 1))
  {
    // フラグ内容が0/1でないなら異常判定。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // 異常時実行中断フラグを設定
  timeline_command_dispatcher_[line_no].stop_on_error = flag;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TLCD_SET_LOUT_FLAG(const CTCP* packet)
{
  const unsigned char* param = CCP_get_param_head(packet);
  uint8_t line_no;
  uint8_t flag;

  // パラメータ読み出し&値チェック。
  line_no = param[0];

  if (line_no >= TL_ID_MAX)
  {
    // 存在しないライン番号が指定された場合は異常判定。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  flag = param[1];

  if ((flag != 0) && (flag != 1))
  {
    // フラグ情報が0/1でない場合は異常判定。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // コマンド実行フラグを設定。
  timeline_command_dispatcher_[line_no].lockout = flag;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TLCD_SET_LINE_NO_FOR_TIMELINE_TLM(const CTCP* packet)
{
  int line_no;

  // ライン番号を読み込み
  line_no = CCP_get_param_head(packet)[0];

  if (line_no >= TL_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  TLCD_line_no_for_tlm_ = line_no;

  return CCP_EXEC_SUCCESS;
}

// FIXME: ELのイベント記録を追加する
CCP_EXEC_STS Cmd_TLCD_DEPLOY_BLOCK(const CTCP* packet)
{
  int      line_no;
  bct_id_t block_no;
  PL_ACK   ack;
  const uint8_t* param = CCP_get_param_head(packet);

  if (CCP_get_param_len(packet) != (1 + SIZE_OF_BCT_ID_T))
  {
    // パラメータはTLライン番号(1Byte)とブロック番号。
    // 一致しない場合は異常判定。
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // ライン番号を読み込み
  line_no = param[0];

  if (line_no >= TL_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  endian_memcpy(&block_no, &param[1], SIZE_OF_BCT_ID_T);

  if (block_no >= BCT_MAX_BLOCKS)
  {
    // 指定されたブロック番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  ack = PL_deploy_block_cmd(&(PH_tl_cmd_list[line_no]), block_no, (size_t)(TMGR_get_master_total_cycle()));

  if (ack != PL_SUCCESS)
  {
    EL_record_event((EL_GROUP)EL_CORE_GROUP_TLCD_DEPLOY_BLOCK,
                    (uint32_t)ack,
                    EL_ERROR_LEVEL_LOW,
                    (uint32_t)line_no);
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TLCD_SET_PAGE_FOR_TLM(const CTCP* packet)
{
  uint8_t page;

  page = CCP_get_param_head(packet)[0];

  if (page >= TL_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  TLCD_page_no_ = page;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TLCD_CLEAR_ERR_LOG(const CTCP* packet)
{
  uint8_t line_no;

  line_no = CCP_get_param_head(packet)[0];

  if (line_no >= TL_ID_MAX)
  {
    // ライン番号が不正。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // 当該コマンド処理機能のエラー記録を解除。
  CDIS_clear_error_status(&timeline_command_dispatcher_[line_no]);
  return CCP_EXEC_SUCCESS;
}

#pragma section
