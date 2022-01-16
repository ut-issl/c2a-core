#pragma section REPRO
/**
 * @file
 * @brief モード遷移の制御
 */
#include "mode_manager.h"

#include <string.h>

#include "../TimeManager/time_manager.h"
#include "../TaskManager/task_dispatcher.h"
#include "../AnomalyLogger/anomaly_logger.h"
#include "../EventManager/event_logger.h"
#include "../../TlmCmd/block_command_executor.h"
#include "../../TlmCmd/common_tlm_cmd_packet_util.h"
#include <src_user/TlmCmd/command_definitions.h>
#include "../../TlmCmd/packet_handler.h"
#include "../../Applications/timeline_command_dispatcher.h"
#include "../../Library/endian_memcpy.h"

/**
 * @brief Cmd_MM_START_TRANSITIONの実体
 * @param[in] id: 移行先の Mode ID
 * @return MM_ACK
 */
static MM_ACK MM_start_transition_(MD_MODEID id);

/**
 * @brief Cmd_MM_FINISH_TRANSITIONの実体
 * @param void
 * @return MM_ACK
 */
static MM_ACK MM_finish_transition_(void);

/**
 * @brief (モード遷移に対応する)ブロックコマンドのタイムラインへの展開を行う
 * @param[in] index: 展開する BC ID
 * @return MM_ACK
 */
static void MM_deploy_block_cmd_(bct_id_t index);

/**
 * @brief モード遷移テーブルをすべて未定義に初期化する
 * @return void
 */
static void MM_clear_transition_table_(void);

static ModeManager mode_manager_;
const ModeManager* const mode_manager = &mode_manager_;

void MM_initialize(void)
{
  // 各モード・モード遷移に対応するブロックコマンドの読み込み
  MD_load_mode_list();

  MM_clear_transition_table_();
  MD_load_transition_table();

  // 起動直後かつモード遷移実行中ではないとして初期化
  mode_manager_.stat = MM_STATUS_FINISHED;
  mode_manager_.previous_id = MD_MODEID_START_UP;
  mode_manager_.current_id = MD_MODEID_START_UP;

  // 起動直後に一度のみ実行したいコマンドはこのモード遷移で実行する
  MM_start_transition_(MD_MODEID_INITIAL);
}

void MM_clear_transition_table_(void)
{
  int from, to;

  // 初期化 全てを未定義に設定。
  for (from = 0; from < MD_MODEID_MODE_MAX; ++from)
  {
    for (to = 0; to < MD_MODEID_MODE_MAX; ++to)
    {
      mode_manager_.transition_table[from][to] = MM_NOT_DEFINED;
    }
  }
}

/**
 * @brief
 * モード遷移後にタスクリストとして実行するブロックコマンドを設定するコマンド
 */
CCP_EXEC_STS Cmd_MM_SET_MODE_LIST(const CTCP* packet)
{
  MD_MODEID mode;
  bct_id_t  bc_index;
  const uint8_t* param = CCP_get_param_head(packet);

  if (CCP_get_param_len(packet) != (1 + SIZE_OF_BCT_ID_T))
  {
    // パラメータはパケットヘッダとuint8_t 2個（mode, index)。
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // どのモードにどのブロックコマンドを登録するかを引数から読み出す
  mode = (MD_MODEID)param[0];
  endian_memcpy(&bc_index, param + 1, SIZE_OF_BCT_ID_T);

  mode_manager_.mm_ack = MM_set_mode_list(mode, bc_index);
  if (mode_manager_.mm_ack != MM_SUCCESS)
  {
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
  return CCP_EXEC_SUCCESS;
}

MM_ACK MM_set_mode_list(MD_MODEID mode, bct_id_t  bc_index)
{
  if (mode >= MD_MODEID_MODE_MAX)
  {
    // 定義されていないモード番号が指定された場合
    return MM_BAD_ID;
  }
  else if (bc_index > MM_NOT_DEFINED)
  {
    // 範囲外のブロックコマンド番号が指定された場合
    return MM_BAD_BC_INDEX;
  }
  else if (bc_index != MM_NOT_DEFINED && BCE_is_active(bc_index) != 1)
  {
    // 無効化されているブロック番号が指定された場合
    // 定義なし用の値を特別扱いしている。
    return MM_INACTIVE_BLOCK;
  }

  mode_manager_.mode_list[mode] = bc_index;

  return MM_SUCCESS;
}

/**
 * @brief
 * モード遷移時に実行するブロックコマンドを設定するコマンド
 */
CCP_EXEC_STS Cmd_MM_SET_TRANSITION_TABLE(const CTCP* packet)
{
  unsigned char from, to;
  bct_id_t bc_index;
  const uint8_t* param = CCP_get_param_head(packet);

  if (CCP_get_param_len(packet) != 1 + 1 + SIZE_OF_BCT_ID_T)
  {
    // コマンドはパケットヘッダとuint8_t 3個（from, to, index)。
    return CCP_EXEC_ILLEGAL_LENGTH;
  }

  // どのモード遷移にどのブロックコマンドを登録するかを引数から読み出す
  from = param[0];
  to   = param[1];
  endian_memcpy(&bc_index, param + 2, SIZE_OF_BCT_ID_T);

  mode_manager_.mm_ack = MM_set_transition_table((MD_MODEID)from, (MD_MODEID)to, bc_index);
  if (mode_manager_.mm_ack != MM_SUCCESS)
  {
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
  return CCP_EXEC_SUCCESS;
}

MM_ACK MM_set_transition_table(MD_MODEID from,
    MD_MODEID to,
    bct_id_t  bc_index)
{
  if ((from >= MD_MODEID_MODE_MAX) || (to >= MD_MODEID_MODE_MAX))
  {
    // 定義されていないモード番号が指定された場合
    return MM_BAD_ID;
  }
  else if (bc_index > MM_NOT_DEFINED)
  {
    // 範囲外のブロックコマンド番号が指定された場合
    return MM_BAD_BC_INDEX;
  }
  else if (bc_index != MM_NOT_DEFINED && BCE_is_active(bc_index) != 1)
  {
    // 無効化されているブロック番号が指定された場合
    // 定義なし用の値を特別扱いしている。
    return MM_INACTIVE_BLOCK;
  }

  mode_manager_.transition_table[from][to] = bc_index;

  return MM_SUCCESS;
}

/**
 * @brief
 * モード遷移を開始するコマンド
 */
CCP_EXEC_STS Cmd_MM_START_TRANSITION(const CTCP* packet)
{
  MD_MODEID id;

  // どのモードに遷移するかを引数から読み出す
  id = (MD_MODEID)CCP_get_param_head(packet)[0];

  // エラー情報を保存
  mode_manager_.mm_ack = MM_start_transition_(id);
  if (mode_manager_.mm_ack != MM_SUCCESS)
  {
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
  return CCP_EXEC_SUCCESS;
}

static MM_ACK MM_start_transition_(MD_MODEID id)
{
  size_t bc_index = MM_NOT_DEFINED;

  if (id >= MD_MODEID_MODE_MAX)
  {
    // 定義されていないモード番号が指定された場合
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_MODE_MANAGER, MM_BAD_ID);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_MODE_MANAGER, MM_BAD_ID, EL_ERROR_LEVEL_LOW, (uint32_t)id);
    return MM_BAD_ID;
  }
  else if (mode_manager_.stat != MM_STATUS_FINISHED)
  {
    // 別のモード遷移を実行中の場合
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_MODE_MANAGER, MM_OVERWRITE);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_MODE_MANAGER, MM_OVERWRITE, EL_ERROR_LEVEL_LOW, (uint32_t)mode_manager_.current_id);
    return MM_OVERWRITE;
  }

  // 実行するブロックコマンドを読み込んでみる
  bc_index = mode_manager_.transition_table[mode_manager_.current_id][id];

  if (bc_index == MM_NOT_DEFINED)
  {
    // 実行したいモード遷移に対応するブロックコマンドが登録されていない場合
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_MODE_MANAGER, MM_ILLEGAL_MOVE);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_MODE_MANAGER, MM_ILLEGAL_MOVE, EL_ERROR_LEVEL_LOW, (uint32_t)bc_index);
    return MM_ILLEGAL_MOVE;
  }

  // ここまでくればモード遷移を実行
  TMGR_clear_master_mode_cycle();
  MM_deploy_block_cmd_(mode_manager_.transition_table[mode_manager_.current_id][id]);
  mode_manager_.previous_id = mode_manager_.current_id;
  mode_manager_.current_id = id;
  mode_manager_.stat = MM_STATUS_IN_PROGRESS;

  return MM_SUCCESS;
}

/**
 * @brief
 * モード遷移終了を示すコマンド
 * モード遷移のブロックコマンドの最後に入れて使う
 * 入っていない場合、タスクリストが遷移先のモードに置き換わらないので注意
 */
CCP_EXEC_STS Cmd_MM_FINISH_TRANSITION(const CTCP* packet)
{
  (void)packet;

  mode_manager_.mm_ack = MM_finish_transition_();
  if (mode_manager_.mm_ack != MM_SUCCESS)
  {
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
  return CCP_EXEC_SUCCESS;
}

static MM_ACK MM_finish_transition_(void)
{
  TDSP_ACK ack = TDSP_UNKNOWN;

  if (mode_manager_.stat != MM_STATUS_IN_PROGRESS)
  {
    // モード遷移が実行中でない場合
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_MODE_MANAGER, MM_NOT_IN_PROGRESS);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_MODE_MANAGER, MM_NOT_IN_PROGRESS, EL_ERROR_LEVEL_HIGH, (uint32_t)mode_manager_.current_id);
    return MM_NOT_IN_PROGRESS;
  }

  TMGR_clear_master_mode_cycle(); // これをstart/finish両方でしているのはなぜ？

  // タスクリストを、遷移後のモードに登録されたブロックコマンドへ更新
  ack = TDSP_set_task_list_id(MM_get_tasklist_id_of_mode(mode_manager_.current_id));

  switch (ack)
  {
  case TDSP_SUCCESS:
    break;

  default:
#ifndef AL_DISALBE_AT_C2A_CORE
    AL_add_anomaly(AL_CORE_GROUP_MODE_MANAGER, MM_TL_LOAD_FAILED);
#endif
    EL_record_event((EL_GROUP)EL_CORE_GROUP_MODE_MANAGER, MM_TL_LOAD_FAILED, EL_ERROR_LEVEL_HIGH, (uint32_t)ack);
    break;
  }

  mode_manager_.stat = MM_STATUS_FINISHED;

  return MM_SUCCESS;
}

static void MM_deploy_block_cmd_(bct_id_t bc_index)
{
  CTCP packet;

  CCP_form_block_deploy_cmd(&packet, TL_ID_DEPLOY_BC, bc_index);

  PH_dispatch_command(&packet);
}

CCP_EXEC_STS Cmd_MM_UPDATE_TRANSITION_TABLE_FOR_TLM(const CTCP* packet)
{
  (void)packet;
  MM_update_transition_table_for_tlm();

  return CCP_EXEC_SUCCESS;
}

uint16_t MM_update_transition_table_for_tlm(void)
{
  uint8_t from, to;
  uint16_t idx = 0;

  memset(mode_manager_.transition_table_for_tlm, 0x00, sizeof(mode_manager_.transition_table_for_tlm));

  for (from = 0; from < MD_MODEID_MODE_MAX; ++from)
  {
    for (to = 0; to < MD_MODEID_MODE_MAX; ++to)
    {
      if (mode_manager_.transition_table[from][to] == MM_NOT_DEFINED) continue;

      mode_manager_.transition_table_for_tlm[idx].from = from;
      mode_manager_.transition_table_for_tlm[idx].to = to;
      mode_manager_.transition_table_for_tlm[idx].bc_index = mode_manager_.transition_table[from][to];
      ++idx;
    }
  }

  return idx;
}

bct_id_t MM_get_tasklist_id_of_mode(MD_MODEID mode)
{
  if (mode >= MD_MODEID_MODE_MAX) return 0;
  return mode_manager_.mode_list[mode];
}

#pragma section
