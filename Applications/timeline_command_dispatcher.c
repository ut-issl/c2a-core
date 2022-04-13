#pragma section REPRO
#include "timeline_command_dispatcher.h"
#include "../TlmCmd/packet_handler.h"
#include "../System/TimeManager/time_manager.h"
#include "../System/AnomalyLogger/anomaly_logger.h"
#include "../System/EventManager/event_logger.h"
#include "../TlmCmd/common_cmd_packet_util.h"

#include <string.h> // for memset

static TimelineCommandDispatcher timeline_command_dispatcher_;
const TimelineCommandDispatcher* const timeline_command_dispatcher = &timeline_command_dispatcher_;
static CommonCmdPacket TLCD_null_packet_;

static void TLCD_gs_init_(void);
static void TLCD_gs_dispatch_(void);
static void TLCD_bc_init_(void);
static void TLCD_bc_dispatch_(void);
static void TLCD_tlm_init_(void);
static void TLCD_tlm_dispatch_(void);
#ifdef TLCD_ENABLE_MISSION_TL
static void TLCD_mis_init_(void);
static void TLCD_mis_dispatch_(void);
#endif

/**
 * @brief 指定された lien_no の Timeline コマンドを実行する
 * @param id
 * @return void
 */
static void tlc_dispatcher_(TLCD_ID id);

/**
 * @brief 指定された時刻, id の TL コマンドを削除する
 * @note FIXME: 返り値が PH_ACK なのはおかしい
 * @param[in] id
 * @param[in] time 削除したい TL コマンドが登録されている TI
 * @return PH_ACK
 */
static PH_ACK TLCD_drop_tl_cmd_at_(TLCD_ID id, cycle_t time);

AppInfo TLCD_gs_create_app(void)
{
  return AI_create_app_info("tlcd_gs", TLCD_gs_init_, TLCD_gs_dispatch_);
}

static void TLCD_gs_init_(void)
{
  timeline_command_dispatcher_.dispatcher[TLCD_ID_FROM_GS] = CDIS_init(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]));

  // タイムラインコマンドのテレメトリ変数初期設定
  // すべてのタイムラインで共用なので TLCD_gs で代表して初期化
  timeline_command_dispatcher_.tlm_info_.id = TLCD_ID_FROM_GS;
  timeline_command_dispatcher_.tlm_info_.page_no = 0;
  timeline_command_dispatcher_.tlm_info_.updated_at = 0;

  memset(&TLCD_null_packet_, 0, sizeof(TLCD_null_packet_));
  TLCD_update_tl_list_for_tlm(TLCD_ID_FROM_GS);
}

static void TLCD_gs_dispatch_(void)
{
  tlc_dispatcher_(TLCD_ID_FROM_GS);
}

AppInfo TLCD_bc_create_app(void)
{
  return AI_create_app_info("tlcd_bc", TLCD_bc_init_, TLCD_bc_dispatch_);
}

static void TLCD_bc_init_(void)
{
  timeline_command_dispatcher_.dispatcher[TLCD_ID_DEPLOY_BC] = CDIS_init(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]));
}

static void TLCD_bc_dispatch_(void)
{
  tlc_dispatcher_(TLCD_ID_DEPLOY_BC);
}

AppInfo TLCD_tlm_create_app(void)
{
  return AI_create_app_info("tlcd_tlm", TLCD_tlm_init_, TLCD_tlm_dispatch_);
}

static void TLCD_tlm_init_(void)
{
  timeline_command_dispatcher_.dispatcher[TLCD_ID_DEPLOY_TLM] = CDIS_init(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]));
}

static void TLCD_tlm_dispatch_(void)
{
  tlc_dispatcher_(TLCD_ID_DEPLOY_TLM);
}

#ifdef TLCD_ENABLE_MISSION_TL
AppInfo TLCD_mis_create_app(void)
{
  return AI_create_app_info("tlcd_mis", TLCD_mis_init_, TLCD_mis_dispatch_);
}

static void TLCD_mis_init_(void)
{
  timeline_command_dispatcher_.dispatcher[TLCD_ID_FROM_GS_FOR_MISSION] = CDIS_init(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]));
}

static void TLCD_mis_dispatch_(void)
{
  tlc_dispatcher_(TLCD_ID_FROM_GS_FOR_MISSION);
}
#endif

static void tlc_dispatcher_(TLCD_ID id)
{
  PL_ACK ack = PL_check_tl_cmd(&(PH_tl_cmd_list[id]),
                               TMGR_get_master_total_cycle());

  switch (ack)
  {
  case PL_TLC_ON_TIME:
    // 実行予定時刻ちょうど
    CDIS_dispatch_command(&timeline_command_dispatcher_.dispatcher[id]);
    break;

  case PL_TLC_PAST_TIME:

    // 実行予定時刻超過
    if (timeline_command_dispatcher_.dispatcher[id].lockout == 0)
    {
      // Lockout無効の場合はアノマリーを記録。
      // Lockout有効の場合はアノマリーの連続生成を防ぐため時刻超過状態でもアノマリーを記録しない。
#ifndef AL_DISALBE_AT_C2A_CORE
      AL_add_anomaly(AL_CORE_GROUP_TLCD, (uint32_t)id);
#endif
      EL_record_event((EL_GROUP)EL_CORE_GROUP_TLCD_PAST_TIME,
                      (uint32_t)id,
                      EL_ERROR_LEVEL_LOW,
                      (uint32_t)CCP_get_ti( (const CommonCmdPacket*)(PL_get_head(&PH_tl_cmd_list[id])->packet) ));

      if (timeline_command_dispatcher_.dispatcher[id].stop_on_error == 1)
      {
        // Lockout無効かつSOE有効の場合
        // Lockout有効とし、タイムラインコマンド処理を停止。
        timeline_command_dispatcher_.dispatcher[id].lockout = 1;
      }
    }

    // コマンド実行処理呼び出し。
    // Lockoutとなった場合は呼び出してもコマンドは実行されない。
    CDIS_dispatch_command(&timeline_command_dispatcher_.dispatcher[id]);
    break;

  case PL_TLC_NOT_YET:
    // 実行すべきコマンドなし。
    break;

  default:
    break;
  }
}

TLCD_ID TLCD_update_tl_list_for_tlm(TLCD_ID id)
{
  PL_Node* pos;
  int i;

  if (id >= TLCD_ID_MAX) return TLCD_ID_MAX;

  pos = (PL_Node*)PL_get_head(&(PH_tl_cmd_list[id]));    // const_cast
  // テレメ情報生成時刻を記録
  timeline_command_dispatcher_.tlm_info_.updated_at = TMGR_get_master_total_cycle();

  // 全リスト内容をクリア
  for (i = 0; i < PH_TL0_CMD_LIST_MAX; ++i)
  {
    timeline_command_dispatcher_.tlm_info_.tl_list[i] = &TLCD_null_packet_;
  }

  // 登録されているTLコマンドをリストに書き込み
  for (i = 0; pos != NULL; ++i)
  {
    timeline_command_dispatcher_.tlm_info_.tl_list[i] = (const CommonCmdPacket*)(pos->packet);
    pos = pos->next;
  }

  return id;
}

CCP_EXEC_STS Cmd_TLCD_CLEAR_ALL_TIMELINE(const CommonCmdPacket* packet)
{
  TLCD_ID id = (TLCD_ID)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (id >= TLCD_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  CDIS_clear_command_list(&timeline_command_dispatcher_.dispatcher[id]);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TLCD_CLEAR_TIMELINE_AT(const CommonCmdPacket* packet)
{
  TLCD_ID id = (TLCD_ID)CCP_get_param_from_packet(packet, 0, uint8_t);
  cycle_t time = CCP_get_param_from_packet(packet, 1, cycle_t);

  if (id >= TLCD_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  if (TLCD_drop_tl_cmd_at_(id, time) == PH_ACK_SUCCESS)
  {
    return CCP_EXEC_SUCCESS;
  }
  else
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
}

static PH_ACK TLCD_drop_tl_cmd_at_(TLCD_ID id, cycle_t time)
{
  int i;

  PL_Node* prev = NULL;
  PL_Node* current = (PL_Node*)PL_get_head(&(PH_tl_cmd_list[id])); // const_cast
  int active_nodes_num = PL_count_active_nodes(&PH_tl_cmd_list[id]);

  if (current == NULL) return PH_ACK_PACKET_NOT_FOUND;

  for (i = 0; i < active_nodes_num; ++i)
  {
    if (CCP_get_ti( (const CommonCmdPacket*)(current->packet) ) == time)
    {
      PL_drop_node(&(PH_tl_cmd_list[id]), prev, current);
      break;
    }
    if (PL_get_next(current) == NULL) return PH_ACK_PACKET_NOT_FOUND;

    prev = current;
    current = current->next;
  }

  return PH_ACK_SUCCESS;
}

// FIXME: ELのイベント記録を追加する
CCP_EXEC_STS Cmd_TLCD_DEPLOY_BLOCK(const CommonCmdPacket* packet)
{
  TLCD_ID id = (TLCD_ID)CCP_get_param_from_packet(packet, 0, uint8_t);
  bct_id_t block_no = CCP_get_param_from_packet(packet, 1, bct_id_t);
  PL_ACK   ack;

  if (CCP_get_param_len(packet) != (1 + SIZE_OF_BCT_ID_T))
  {
    // パラメータはTLライン番号(1Byte)とブロック番号。
    // 一致しない場合は異常判定。
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  if (id >= TLCD_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  if (block_no >= BCT_MAX_BLOCKS)
  {
    // 指定されたブロック番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  ack = PL_deploy_block_cmd(&(PH_tl_cmd_list[id]), block_no, TMGR_get_master_total_cycle());

  if (ack != PL_SUCCESS)
  {
    EL_record_event((EL_GROUP)EL_CORE_GROUP_TLCD_DEPLOY_BLOCK,
                    (uint32_t)ack,
                    EL_ERROR_LEVEL_LOW,
                    (uint32_t)id);
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }

  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_TLCD_CLEAR_ERR_LOG(const CommonCmdPacket* packet)
{
  TLCD_ID id = (TLCD_ID)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (id >= TLCD_ID_MAX)
  {
    // ライン番号が不正。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // 当該コマンド処理機能のエラー記録を解除。
  CDIS_clear_error_status(&timeline_command_dispatcher_.dispatcher[id]);
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_TLCD_SET_SOE_FLAG(const CommonCmdPacket* packet)
{
  TLCD_ID id = (TLCD_ID)CCP_get_param_from_packet(packet, 0, uint8_t);
  uint8_t flag = CCP_get_param_from_packet(packet, 1, uint8_t);

  if (id >= TLCD_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  if ((flag != 0) && (flag != 1))
  {
    // フラグ内容が0/1でないなら異常判定。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // 異常時実行中断フラグを設定
  timeline_command_dispatcher_.dispatcher[id].stop_on_error = flag;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TLCD_SET_LOUT_FLAG(const CommonCmdPacket* packet)
{
  TLCD_ID id = (TLCD_ID)CCP_get_param_from_packet(packet, 0, uint8_t);
  uint8_t flag = CCP_get_param_from_packet(packet, 1, uint8_t);

  if (id >= TLCD_ID_MAX)
  {
    // 存在しないライン番号が指定された場合は異常判定。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  if ((flag != 0) && (flag != 1))
  {
    // フラグ情報が0/1でない場合は異常判定。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // コマンド実行フラグを設定。
  timeline_command_dispatcher_.dispatcher[id].lockout = flag;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TLCD_SET_ID_FOR_TLM(const CommonCmdPacket* packet)
{
  TLCD_ID id = (TLCD_ID)CCP_get_param_from_packet(packet, 0, uint8_t);

  if (id >= TLCD_ID_MAX)
  {
    // 指定されたライン番号が存在しない場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  timeline_command_dispatcher_.tlm_info_.id = id;

  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_TLCD_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page = CCP_get_param_from_packet(packet, 0, uint8_t);

  if (page >= TL_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  timeline_command_dispatcher_.tlm_info_.page_no = page;
  return CCP_EXEC_SUCCESS;
}

#pragma section
