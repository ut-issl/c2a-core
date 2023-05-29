#pragma section REPRO
/**
 * @file
 * @brief userがテレメトリ詰まりをやTL溢れを防ぎつつ，またCDHなどがテレメトリを管理しやすくするためのApp
 * @note  利用方法は telemetry_manager.h の最下部を参照（FIXME: あとでdocumentに移す）
 */
#include "telemetry_manager.h"

#include <string.h>     // for memsetなどのmem系

#include "./divided_cmd_utility.h"
#include "../TlmCmd/block_command_loader.h"
#include "../TlmCmd/block_command_executor.h"
#include "../TlmCmd/command_analyze.h"
#include "../Library/print.h"
#include "../Library/endian.h"
#include "../Library/result.h"
#include "../TlmCmd/common_cmd_packet_util.h"
#include <src_user/TlmCmd/block_command_definitions.h>
#include <src_user/TlmCmd/command_definitions.h>
#include <src_user/TlmCmd/telemetry_definitions.h>

// default 設定
// BC にどの TLM_MGR_BC_RPLE を割り当てるか
#define TLM_MGR_BC_RPLE_AT_BC_0   (TLM_MGR_BC_ROLE_HK_TLM)
#define TLM_MGR_BC_RPLE_AT_BC_1   (TLM_MGR_BC_ROLE_HIGH_FREQ_TLM)
#define TLM_MGR_BC_RPLE_AT_BC_2   (TLM_MGR_BC_ROLE_HIGH_FREQ_TLM)
#define TLM_MGR_BC_RPLE_AT_BC_3   (TLM_MGR_BC_ROLE_HIGH_FREQ_TLM)
#define TLM_MGR_BC_RPLE_AT_BC_4   (TLM_MGR_BC_ROLE_LOW_FREQ_TLM)    // TLM_MGR_BC_RPLE_HIGH_FREQ_TLM が固まらないようにここに入れている
#define TLM_MGR_BC_RPLE_AT_BC_5   (TLM_MGR_BC_ROLE_HIGH_FREQ_TLM)
#define TLM_MGR_BC_RPLE_AT_BC_6   (TLM_MGR_BC_ROLE_HIGH_FREQ_TLM)
#define TLM_MGR_BC_RPLE_AT_BC_7   (TLM_MGR_BC_ROLE_HIGH_FREQ_TLM)
#define TLM_MGR_BC_RPLE_AT_BC_8   (TLM_MGR_BC_ROLE_HIGH_FREQ_TLM)
#define TLM_MGR_BC_RPLE_AT_BC_9   (TLM_MGR_BC_ROLE_LOW_FREQ_TLM)    // TLM_MGR_BC_ROLE_HIGH_FREQ_TLM が固まらないようにここに入れている

// user 設定
#include <src_user/Settings/Applications/telemetry_manager_define.h>
#include <src_user/Settings/Applications/telemetry_manager_params.h>

// 現状， BC#9 は low ではないとだめ (deploy と combine するため)
#if !(TLM_MGR_BC_ROLE_AT_BC_9 == TLM_MGR_BC_ROLE_LOW_FREQ_TLM)
#error TLM_MGR_BC_ROLE_AT_BC_9 should be TLM_MGR_BC_ROLE_LOW_FREQ_TLM
#endif

/**
 * @brief  App初期化関数
 * @param  void
 * @return void
 */
static void TLM_MGR_init_by_am_(void);

/**
 * @brief  初期化（分割 1/2）
 * @note   実行時間の問題から分割している
 * @param  void
 * @return RESULT
 */
static RESULT TLM_MGR_init_1_(void);

/**
 * @brief  初期化（分割 2/2）
 * @note   実行時間の問題から分割している
 * @param  void
 * @return RESULT
 */
static RESULT TLM_MGR_init_2_(void);

/**
 * @brief  AppInfo 構造体のクリア
 * @param  void
 * @return RESULT
 */
static RESULT TLM_MGR_clear_info_(void);

/**
 * @brief  BC の設定を登録する
 * @param  cmd_table_idx: 登録する cmd table (bc) の index
 * @param  bc_id:         使う BC の ID
 * @param  bc_role:       その BC の役割
 * @return RESULT
 */
static RESULT TLM_MGR_regigster_bc_settings(uint8_t cmd_table_idx,
                                            bct_id_t bc_id,
                                            TLM_MGR_BC_ROLE bc_role);

/**
 * @brief  TLM_MGR_RegisterInfo に BC 情報を追加
 * @param  cmd_table_idx: 登録する cmd table (bc) の index
 * @param  register_info: 登録先の TLM_MGR_RegisterInfo
 * @return RESULT
 */
static RESULT TLM_MGR_add_bc_settings_to_register_info_(uint8_t cmd_table_idx,
                                                        TLM_MGR_RegisterInfo* register_info);

/**
 * @brief  TLM_MGR_BC_ROLE に応じて TLM_MGR_CmdTable から 1 BC の 1 block から全 cmd を消す (NPO 埋めする)
 * @note   DCU を使っている
 * @param  cmd_code:      DCU を呼び出す元の cmd id
 * @param  bc_role:       TLM_MGR_BC_ROLE
 * @param  register_info: 削除する role の TLM_MGR_RegisterInfo
 * @return RESULT
 */
static CCP_CmdRet TLM_MGR_clear_cmds_based_on_role_(CMD_CODE cmd_code,
                                                    TLM_MGR_BC_ROLE bc_role,
                                                    TLM_MGR_RegisterInfo* register_info);

/**
 * @brief  テレメ生成などのコマンドを TLM MGR に登録する
 * @param  role:         TLM_MGR_BC_ROLE
 * @param  cmd_type:     TLM_MGR_CMD_TYPE
 * @param  apid:         APID
 * @param  tlm_id:       TLM_CODE
 * @param  dr_partition: dr partition (DR 関連の時だけ)
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_register_(TLM_MGR_BC_ROLE role,
                                          TLM_MGR_CMD_TYPE cmd_type,
                                          APID apid,
                                          TLM_CODE tlm_id,
                                          uint8_t dr_partition);

/**
 * @brief  TLM_MGR_CMD_TYPE に応じて， TLM MGR に登録する TLC を生成する
 * @param[out]  packet:       TLC packet
 * @param[in]   ti:           TI
 * @param[in]   cmd_type:     TLM_MGR_CMD_TYPE
 * @param[in]   apid:         APID
 * @param[in]   tlm_id:       TLM_CODE
 * @param[in]   dr_partition: dr partition (DR 関連の時だけ)
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_form_register_tlc_(CommonCmdPacket* packet,
                                                   cycle_t ti,
                                                   TLM_MGR_CMD_TYPE cmd_type,
                                                   APID apid,
                                                   TLM_CODE tlm_id,
                                                   uint8_t dr_partition);

/**
 * @brief  TLM MGR に登録する TLC (TLM_MGR_CMD_TYPE_TG_GENERATE_MS_TLM) を生成する
 * @param[out]  packet:       TLC packet
 * @param[in]   ti:           TI
 * @param[in]   tlm_id:       TLM_CODE
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_form_tg_generate_ms_tlm_(CommonCmdPacket* packet,
                                                         cycle_t ti,
                                                         TLM_CODE tlm_id);

/**
 * @brief  TLM MGR に登録する TLC (TLM_MGR_CMD_TYPE_TG_GENERATE_ST_TLM) を生成する
 * @param[out]  packet:       TLC packet
 * @param[in]   ti:           TI
 * @param[in]   tlm_id:       TLM_CODE
 * @param[in]   dr_partition: dr partition (DR 関連の時だけ)
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_form_tg_generate_st_tlm_(CommonCmdPacket* packet,
                                                         cycle_t ti,
                                                         TLM_CODE tlm_id,
                                                         uint8_t dr_partition);

/**
 * @brief  TLM MGR に登録する TLC (TLM_MGR_CMD_TYPE_TG_FORWARD_AS_MS_TLM) を生成する
 * @param[out]  packet:       TLC packet
 * @param[in]   ti:           TI
 * @param[in]   apid:         APID
 * @param[in]   tlm_id:       TLM_CODE
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_form_tg_forward_as_ms_tlm_(CommonCmdPacket* packet,
                                                           cycle_t ti,
                                                           APID apid,
                                                           TLM_CODE tlm_id);

/**
 * @brief  TLM MGR に登録する TLC (TLM_MGR_CMD_TYPE_TG_FORWARD_AS_ST_TLM) を生成する
 * @param[out]  packet:       TLC packet
 * @param[in]   ti:           TI
 * @param[in]   apid:         APID
 * @param[in]   tlm_id:       TLM_CODE
 * @param[in]   dr_partition: dr partition (DR 関連の時だけ)
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_form_tg_forward_as_st_tlm_(CommonCmdPacket* packet,
                                                           cycle_t ti,
                                                           APID apid,
                                                           TLM_CODE tlm_id,
                                                           uint8_t dr_partition);

/**
 * @brief  TLM MGR に登録する TLC (TLM_MGR_CMD_TYPE_DR_REPLAY_TLM) を生成する
 * @param[out]  packet:       TLC packet
 * @param[in]   ti:           TI
 * @param[in]   dr_partition: dr partition (DR 関連の時だけ)
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_form_dr_replay_tlm_(CommonCmdPacket* packet, cycle_t ti, uint8_t dr_partition);

/**
 * @brief  テレメ生成などのコマンドを TLM MGR から削除する
 * @param  role:         TLM_MGR_BC_ROLE
 * @param  cmd_type:     TLM_MGR_CMD_TYPE
 * @param  apid:         APID
 * @param  tlm_id:       TLM_CODE
 * @param  dr_partition: dr partition (DR 関連の時だけ)
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_delete_(TLM_MGR_BC_ROLE role,
                                        TLM_MGR_CMD_TYPE cmd_type,
                                        APID apid,
                                        TLM_CODE tlm_id,
                                        uint8_t dr_partition);

/**
 * @brief  TLM MGR に上書きして削除するための NOP TLC を生成する
 * @param[out]  packet:       TLC packet
 * @param[in]   ti:           TI
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_form_nop_tlc_(CommonCmdPacket* packet, cycle_t ti);

/**
 * @brief  TLM_MGR_BC_ROLE に応じた TLM_MGR_RegisterInfo を取得する
 * @param  bc_role: TLM_MGR_BC_ROLE
 * @return TLM_MGR_RegisterInfo
 */
static TLM_MGR_RegisterInfo* TLM_MGR_get_regitster_info_from_bc_role_(TLM_MGR_BC_ROLE bc_role);

/**
 * @brief  TLM_MGR_CmdTableCmdElem を更新する
 * @param[in,out]  cmd_elem:     更新する TLM_MGR_CmdTableCmdElem
 * @param[in]      cmd_type:     TLM_MGR_CMD_TYPE
 * @param[in]      apid:         APID
 * @param[in]      tlm_id:       TLM_CODE
 * @param[in]      dr_partition: dr partition
 * @return void
 */
static void TLM_MGR_update_cmd_elem_of_cmd_table_(TLM_MGR_CmdTableCmdElem* cmd_elem,
                                                  TLM_MGR_CMD_TYPE cmd_type,
                                                  APID apid,
                                                  TLM_CODE tlm_id,
                                                  uint8_t dr_partition);

/**
 * @brief  TLM_MGR_CmdTableCmdElem をクリアする
 * @note   0x00 埋めでクリアする
 * @param[in,out]  cmd_elem: クリアする TLM_MGR_CmdTableCmdElem
 * @return void
 */
static void TLM_MGR_clear_cmd_elem_of_cmd_table_(TLM_MGR_CmdTableCmdElem* cmd_elem);

/**
 * @brief  次に TLM MGR に登録するコマンドの BCT_Pos と TLM_MGR_CmdTableCmdElem を取得する
 * @param[out]  next_cmd_pos:  BCT_Pos
 * @param[out]  next_cmd_elem: TLM_MGR_CmdTableCmdElem
 * @param[in]   register_info: 登録しようとしている TLM_MGR_RegisterInfo
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_get_next_register_cmd_pos_(BCT_Pos* next_cmd_pos,
                                                           TLM_MGR_CmdTableCmdElem* next_cmd_elem,
                                                           const TLM_MGR_RegisterInfo* register_info);

/**
 * @brief  すでに登録されているコマンドのうち，末尾にあるコマンドの BCT_Pos と TLM_MGR_CmdTableCmdElem を取得する
 * @param[out]  next_cmd_pos:  BCT_Pos
 * @param[out]  next_cmd_elem: TLM_MGR_CmdTableCmdElem
 * @param[in]   register_info: 登録しようとしている TLM_MGR_RegisterInfo
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_get_last_registered_cmd_pos_(BCT_Pos* last_cmd_pos,
                                                             TLM_MGR_CmdTableCmdElem* last_cmd_elem,
                                                             const TLM_MGR_RegisterInfo* register_info);

/**
 * @brief  すでに登録されているコマンドのから，指定したコマンドを検索し，その BCT_Pos と TLM_MGR_CmdTableCmdElem を取得する
 * @param[out]  next_cmd_pos:  BCT_Pos
 * @param[out]  next_cmd_elem: TLM_MGR_CmdTableCmdElem
 * @param[in]   register_info: 検索対象の TLM_MGR_RegisterInfo
 * @param[in]   cmd_type:      検索対象の TLM_MGR_CMD_TYPE
 * @param[in]   apid:          検索対象の APID
 * @param[in]   tlm_id:        検索対象の TLM_CODE
 * @param[in]   dr_partition:  検索対象の dr partition
 * @return TLM_MGR_ERR_CODE
 */
static TLM_MGR_ERR_CODE TLM_MGR_find_registered_cmd_pos_(BCT_Pos* found_cmd_pos,
                                                         TLM_MGR_CmdTableCmdElem* found_cmd_elem,
                                                         const TLM_MGR_RegisterInfo* register_info,
                                                         TLM_MGR_CMD_TYPE cmd_type,
                                                         APID apid,
                                                         TLM_CODE tlm_id,
                                                         uint8_t dr_partition);

/**
 * @brief  TLM_MGR_CmdTableCmdElem が指定したものと等しいかチェックする
 * @param  cmd_elem:     等しいかチェックされる TLM_MGR_CmdTableCmdElem
 * @param  cmd_type:     等しいかチェックする TLM_MGR_CMD_TYPE
 * @param  apid:         等しいかチェックする APID
 * @param  tlm_id:       等しいかチェックする TLM_CODE
 * @param  dr_partition: 等しいかチェックする dr partition
 * @return RESULT
 */
static RESULT TLM_MGR_check_same_cmd_(const TLM_MGR_CmdTableCmdElem* cmd_elem,
                                      TLM_MGR_CMD_TYPE cmd_type,
                                      APID apid,
                                      TLM_CODE tlm_id,
                                      uint8_t dr_partition);

/**
 * @brief  master BC を deploy する BC の構築
 * @param  void
 * @return void
 */
static void TLM_MGR_load_deploy_bc_(void);

/**
 * @brief  BC 全体を展開していく master BC の構築
 * @param  void
 * @return void
 */
static void TLM_MGR_load_master_bc_(void);

/**
 * @brief  NOP で埋められた BC の構築
 * @param  void
 * @return void
 */
static void TLM_MGR_load_nop_bc_(void);

/**
 * @brief  指定した BC を NOP x TLM_MGR_MAX_CMD_NUM_PER_BC で埋める
 * @param  bc_id: NOP でうめる BC ID
 * @return void
 */
static void TLM_MGR_clear_bc_to_nop_(bct_id_t bc_id);

/**
 * @brief  TLM_MGR_ERR_CODE から CCP_CmdRet に変換する
 * @param  code: TLM_MGR_ERR_CODE
 * @return CCP_CmdRet
 */
static CCP_CmdRet TLM_MGR_conv_err_code_to_ccp_cmd_ret_(TLM_MGR_ERR_CODE code);


static TelemetryManager telemetry_manager_;
const TelemetryManager* const telemetry_manager = &telemetry_manager_;

static CommonCmdPacket TLM_MGR_packet_;

AppInfo TLM_MGR_create_app(void)
{
  return AI_create_app_info("telemetry_manager", TLM_MGR_init_by_am_, NULL);
}


static void TLM_MGR_init_by_am_(void)
{
  telemetry_manager_.is_inited = 0;
}


// FIXME: 実行時間やばい． Cmd_TLM_MGR_INIT を直す時に直す
// BCT の初期化より前なので，AppInit にできない．
static RESULT TLM_MGR_init_1_(void)
{
  TLM_MGR_clear_info_();
  return RESULT_OK;
}


static RESULT TLM_MGR_init_2_(void)
{
  CCP_CmdRet ret = CCP_form_and_exec_rtc(Cmd_CODE_TLM_MGR_INIT_MASTER_AND_DEPLOY_BC, NULL, 0);
  if (ret.exec_sts != CCP_EXEC_SUCCESS) return RESULT_ERR;
  return RESULT_OK;
}


static RESULT TLM_MGR_clear_info_(void)
{
  RESULT ret;
  memset(&telemetry_manager_, 0x00, sizeof(telemetry_manager_));

  telemetry_manager_.is_inited = 0;

  // TOOD: TLM_MGR_USE_BC_NUM が 10 であることを想定したコードになってる
  ret = TLM_MGR_regigster_bc_settings(0, BC_TLM_MGR_0, TLM_MGR_BC_ROLE_AT_BC_0);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(1, BC_TLM_MGR_1, TLM_MGR_BC_ROLE_AT_BC_1);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(2, BC_TLM_MGR_2, TLM_MGR_BC_ROLE_AT_BC_2);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(3, BC_TLM_MGR_3, TLM_MGR_BC_ROLE_AT_BC_3);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(4, BC_TLM_MGR_4, TLM_MGR_BC_ROLE_AT_BC_4);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(5, BC_TLM_MGR_5, TLM_MGR_BC_ROLE_AT_BC_5);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(6, BC_TLM_MGR_6, TLM_MGR_BC_ROLE_AT_BC_6);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(7, BC_TLM_MGR_7, TLM_MGR_BC_ROLE_AT_BC_7);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(8, BC_TLM_MGR_8, TLM_MGR_BC_ROLE_AT_BC_8);
  if (ret != RESULT_OK) return RESULT_ERR;
  ret = TLM_MGR_regigster_bc_settings(9, BC_TLM_MGR_9, TLM_MGR_BC_ROLE_AT_BC_9);
  if (ret != RESULT_OK) return RESULT_ERR;

  // deploy とあわせる最後は LOW であることが必須
  // 今後 ROLE を動的に変更可能なようにすることを見据えて
  if (TLM_MGR_BC_ROLE_AT_BC_9 != TLM_MGR_BC_ROLE_LOW_FREQ_TLM)
  {
    return RESULT_ERR;
  }

  telemetry_manager_.master_bc_id = BC_TLM_MGR_MASTER;
  telemetry_manager_.deploy_bc_id = BC_TLM_MGR_DEPLOY;
  return RESULT_OK;
}


static RESULT TLM_MGR_regigster_bc_settings(uint8_t cmd_table_idx,
                                            bct_id_t bc_id,
                                            TLM_MGR_BC_ROLE bc_role)
{
  TLM_MGR_CmdTableInBlock* cmds_in_block;
  TLM_MGR_RegisterInfo* register_info;
  if (cmd_table_idx >= TLM_MGR_USE_BC_NUM) return RESULT_ERR;

  cmds_in_block = &telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx];
  cmds_in_block->bc_role = bc_role;
  cmds_in_block->bc_id = bc_id;

  register_info = TLM_MGR_get_regitster_info_from_bc_role_(bc_role);
  if (register_info == NULL) return RESULT_ERR;
  return TLM_MGR_add_bc_settings_to_register_info_(cmd_table_idx, register_info);
}


static RESULT TLM_MGR_add_bc_settings_to_register_info_(uint8_t cmd_table_idx,
                                                        TLM_MGR_RegisterInfo* register_info)
{
  if (cmd_table_idx >= TLM_MGR_USE_BC_NUM) return RESULT_ERR;
  if (register_info->cmd_table_idxes_size >= TLM_MGR_USE_BC_NUM) return RESULT_ERR;

  register_info->cmd_table_idxes[register_info->cmd_table_idxes_size] = cmd_table_idx;
  register_info->cmd_table_idxes_size++;

  return RESULT_OK;
}


static CCP_CmdRet TLM_MGR_clear_cmds_based_on_role_(CMD_CODE cmd_code,
                                                    TLM_MGR_BC_ROLE bc_role,
                                                    TLM_MGR_RegisterInfo* register_info)
{
  uint16_t exec_counter;
  uint8_t cmd_table_idx;
  uint8_t bc_num = register_info->cmd_table_idxes_size;

  switch (DCU_check_in(cmd_code, &exec_counter))
  {
  case DCU_STATUS_FINISHED:   // FALLTHROUGH
  case DCU_STATUS_PROGRESS:
    break;
  default:
    // DCU_STATUS_ABORTED_BY_ERR
    // DCU_STATUS_ABORTED_BY_CMD
    // がここに
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  if (exec_counter == bc_num)
  {
    register_info->registered_cmd_num = 0;
    DCU_report_finish(cmd_code, CCP_EXEC_SUCCESS);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  }

  cmd_table_idx = register_info->cmd_table_idxes[exec_counter];
  TLM_MGR_clear_bc_to_nop_(telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx].bc_id);

  // 再帰実行
  if (DCU_register_next(cmd_code, NULL, 0) != DCU_ACK_OK)
  {
    DCU_report_err(cmd_code, CCP_EXEC_ILLEGAL_CONTEXT);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


static TLM_MGR_ERR_CODE TLM_MGR_register_(TLM_MGR_BC_ROLE role,
                                          TLM_MGR_CMD_TYPE cmd_type,
                                          APID apid,
                                          TLM_CODE tlm_id,
                                          uint8_t dr_partition)
{
  TLM_MGR_ERR_CODE ret;
  TLM_MGR_RegisterInfo* register_info;
  BCT_Pos register_pos;
  TLM_MGR_CmdTableCmdElem* register_cmd_elem;
  BCT_ACK bct_ack;

  register_info = TLM_MGR_get_regitster_info_from_bc_role_(role);
  if (register_info == NULL) return TLM_MGR_ERR_CIDE_INVALID_BC_ROLE;

  ret = TLM_MGR_get_next_register_cmd_pos_(&register_pos, register_cmd_elem, register_info);
  if (ret != TLM_MGR_ERR_CODE_OK) return ret;

  ret = TLM_MGR_form_register_tlc_(&TLM_MGR_packet_,
                                   (cycle_t)register_pos.cmd,
                                   cmd_type,
                                   apid,
                                   tlm_id,
                                   dr_partition);
  if (ret != TLM_MGR_ERR_CODE_OK) return ret;

  // すでに NOP で埋められており，かつ activate 済なので，直接 BCT にコマンドを挿入できる．
  bct_ack = BCT_overwrite_cmd(&register_pos, &TLM_MGR_packet_);
  if (bct_ack != BCT_SUCCESS) return TLM_MGR_ERR_CODE_OTHER_ERR;

  // cmd_table の更新
  TLM_MGR_update_cmd_elem_of_cmd_table_(register_cmd_elem,
                                        cmd_type,
                                        apid,
                                        tlm_id,
                                        dr_partition);

  register_info->registered_cmd_num++;
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_ERR_CODE TLM_MGR_form_register_tlc_(CommonCmdPacket* packet,
                                                   cycle_t ti,
                                                   TLM_MGR_CMD_TYPE cmd_type,
                                                   APID apid,
                                                   TLM_CODE tlm_id,
                                                   uint8_t dr_partition)
{
  switch (cmd_type)
  {
  case TLM_MGR_CMD_TYPE_TG_GENERATE_MS_TLM:
    return TLM_MGR_form_tg_generate_ms_tlm_(packet, ti, tlm_id);
  case TLM_MGR_CMD_TYPE_TG_GENERATE_ST_TLM:
    return TLM_MGR_form_tg_generate_st_tlm_(packet, ti, tlm_id, dr_partition);
  case TLM_MGR_CMD_TYPE_TG_FORWARD_AS_MS_TLM:
    return TLM_MGR_form_tg_forward_as_ms_tlm_(packet, ti, apid, tlm_id);
  case TLM_MGR_CMD_TYPE_TG_FORWARD_AS_ST_TLM:
    return TLM_MGR_form_tg_forward_as_st_tlm_(packet, ti, apid, tlm_id, dr_partition);
  case TLM_MGR_CMD_TYPE_DR_REPLAY_TLM:
    return TLM_MGR_form_dr_replay_tlm_(packet, ti, dr_partition);
  case TLM_MGR_CMD_TYPE_UNREGISTERED:   // FALLTHROUGH
  default:
    return TLM_MGR_ERR_CIDE_INVALID_CMD_TYPE;
  }
}


static TLM_MGR_ERR_CODE TLM_MGR_form_tg_generate_ms_tlm_(CommonCmdPacket* packet,
                                                         cycle_t ti,
                                                         TLM_CODE tlm_id)
{
  CCP_UTIL_ACK ret;
  uint8_t param[1];

  param[0] = (uint8_t)tlm_id;

  ret = CCP_form_tlc(packet,
                     ti,
                     Cmd_CODE_TG_GENERATE_MS_TLM,
                     param,
                     1);
  if (ret != CCP_UTIL_ACK_OK) return TLM_MGR_ERR_CODE_OTHER_ERR;
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_ERR_CODE TLM_MGR_form_tg_generate_st_tlm_(CommonCmdPacket* packet,
                                                         cycle_t ti,
                                                         TLM_CODE tlm_id,
                                                         uint8_t dr_partition)
{
  CCP_UTIL_ACK ret;
  uint8_t param[2];

  param[0] = (uint8_t)tlm_id;
  param[1] = dr_partition;

  ret = CCP_form_tlc(packet,
                     ti,
                     Cmd_CODE_TG_GENERATE_ST_TLM,
                     param,
                     2);
  if (ret != CCP_UTIL_ACK_OK) return TLM_MGR_ERR_CODE_OTHER_ERR;
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_ERR_CODE TLM_MGR_form_tg_forward_as_ms_tlm_(CommonCmdPacket* packet,
                                                           cycle_t ti,
                                                           APID apid,
                                                           TLM_CODE tlm_id)
{
  CCP_UTIL_ACK ret;
  uint16_t temp;
  uint8_t param[3];

  temp = (uint16_t)apid;
  ENDIAN_memcpy(&param[0], &temp, sizeof(temp));
  param[2] = (uint8_t)tlm_id;

  ret = CCP_form_tlc(packet,
                     ti,
                     Cmd_CODE_TG_FORWARD_AS_MS_TLM,
                     param,
                     3);
  if (ret != CCP_UTIL_ACK_OK) return TLM_MGR_ERR_CODE_OTHER_ERR;
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_ERR_CODE TLM_MGR_form_tg_forward_as_st_tlm_(CommonCmdPacket* packet,
                                                           cycle_t ti,
                                                           APID apid,
                                                           TLM_CODE tlm_id,
                                                           uint8_t dr_partition)
{
  CCP_UTIL_ACK ret;
  uint16_t temp;
  uint8_t param[4];

  temp = (uint16_t)apid;
  ENDIAN_memcpy(&param[0], &temp, sizeof(temp));
  param[2] = (uint8_t)tlm_id;
  param[3] = dr_partition;

  ret = CCP_form_tlc(packet,
                     ti,
                     Cmd_CODE_TG_FORWARD_AS_ST_TLM,
                     param,
                     4);
  if (ret != CCP_UTIL_ACK_OK) return TLM_MGR_ERR_CODE_OTHER_ERR;
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_ERR_CODE TLM_MGR_form_dr_replay_tlm_(CommonCmdPacket* packet, cycle_t ti, uint8_t dr_partition)
{
#ifdef TLM_MGR_ENABLE_DR_REPLAY_TLM
  CCP_UTIL_ACK ret;
  uint8_t param[2];

  param[0] = dr_partition;
  param[1] = 1;

  ret = CCP_form_tlc(packet,
                     ti,
                     Cmd_CODE_DR_REPLAY_TLM,
                     param,
                     2);
  if (ret != CCP_UTIL_ACK_OK) return TLM_MGR_ERR_CODE_OTHER_ERR;
  return TLM_MGR_ERR_CODE_OK;
#else
  (void)packet;
  (void)ti;
  (void)dr_partition;
  return TLM_MGR_ERR_CODE_OTHER_ERR;
#endif
}


static TLM_MGR_ERR_CODE TLM_MGR_delete_(TLM_MGR_BC_ROLE role,
                                        TLM_MGR_CMD_TYPE cmd_type,
                                        APID apid,
                                        TLM_CODE tlm_id,
                                        uint8_t dr_partition)
{
  TLM_MGR_ERR_CODE ret;
  TLM_MGR_RegisterInfo* register_info;
  BCT_Pos delete_pos;
  BCT_Pos last_pos;
  TLM_MGR_CmdTableCmdElem* delete_cmd_elem;
  TLM_MGR_CmdTableCmdElem* last_cmd_elem;
  BCT_ACK bct_ack;

  register_info = TLM_MGR_get_regitster_info_from_bc_role_(role);
  if (register_info == NULL) return TLM_MGR_ERR_CIDE_INVALID_BC_ROLE;

  ret = TLM_MGR_find_registered_cmd_pos_(&delete_pos,
                                         delete_cmd_elem,
                                         register_info,
                                         cmd_type,
                                         apid,
                                         tlm_id,
                                         dr_partition);
  if (ret != TLM_MGR_ERR_CODE_OK) return ret;
  ret = TLM_MGR_get_last_registered_cmd_pos_(&last_pos,
                                             last_cmd_elem,
                                             register_info);
  if (ret != TLM_MGR_ERR_CODE_OK) return ret;

  if (delete_cmd_elem == last_cmd_elem)
  {
    // 消す対象が末端
    // なにもしない
  }
  else
  {
    // 消す対象が末端ではない
    // 末尾で消す対象を上書き
    bct_ack = BCT_load_cmd(&last_pos, &TLM_MGR_packet_);
    if (bct_ack != BCT_SUCCESS) return TLM_MGR_ERR_CODE_OTHER_ERR;
    bct_ack = BCT_overwrite_cmd(&delete_pos, &TLM_MGR_packet_);
    if (bct_ack != BCT_SUCCESS) return TLM_MGR_ERR_CODE_OTHER_ERR;

    TLM_MGR_update_cmd_elem_of_cmd_table_(delete_cmd_elem,
                                          last_cmd_elem->cmd_type,
                                          last_cmd_elem->apid,
                                          last_cmd_elem->tlm_id,
                                          last_cmd_elem->dr_partition);
  }

  // 末尾の削除
  // TODO: TI が正しいかテストでチェック
  ret = TLM_MGR_form_nop_tlc_(&TLM_MGR_packet_, (cycle_t)last_pos.cmd);
  if (ret != TLM_MGR_ERR_CODE_OK) return ret;

  // すでに NOP で埋められており，かつ activate 済なので，直接 BCT にコマンドを挿入できる．
  bct_ack = BCT_overwrite_cmd(&last_pos, &TLM_MGR_packet_);
  if (bct_ack != BCT_SUCCESS) return TLM_MGR_ERR_CODE_OTHER_ERR;

  TLM_MGR_clear_cmd_elem_of_cmd_table_(last_cmd_elem);

  register_info->registered_cmd_num--;
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_ERR_CODE TLM_MGR_form_nop_tlc_(CommonCmdPacket* packet, cycle_t ti)
{
  CCP_UTIL_ACK ret;
  ret = CCP_form_tlc(packet, ti, Cmd_CODE_NOP, NULL, 0);
  if (ret != CCP_UTIL_ACK_OK) return TLM_MGR_ERR_CODE_OTHER_ERR;
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_RegisterInfo* TLM_MGR_get_regitster_info_from_bc_role_(TLM_MGR_BC_ROLE bc_role)
{
  switch (bc_role)
  {
  case TLM_MGR_BC_ROLE_HK_TLM:
    return &telemetry_manager_.register_info.hk;
  case TLM_MGR_BC_ROLE_HIGH_FREQ_TLM:
    return &telemetry_manager_.register_info.high_freq;
  case TLM_MGR_BC_ROLE_LOW_FREQ_TLM:
    return &telemetry_manager_.register_info.low_freq;
  default:
    return NULL;
  }
}


static void TLM_MGR_update_cmd_elem_of_cmd_table_(TLM_MGR_CmdTableCmdElem* cmd_elem,
                                                  TLM_MGR_CMD_TYPE cmd_type,
                                                  APID apid,
                                                  TLM_CODE tlm_id,
                                                  uint8_t dr_partition)
{
  cmd_elem->cmd_type = cmd_type;
  cmd_elem->apid = apid;
  cmd_elem->tlm_id = tlm_id;
  cmd_elem->dr_partition = dr_partition;
}


static void TLM_MGR_clear_cmd_elem_of_cmd_table_(TLM_MGR_CmdTableCmdElem* cmd_elem)
{
  // クリアは 0 埋めでよい
  memset(cmd_elem, 0x00, sizeof(*cmd_elem));
}


static TLM_MGR_ERR_CODE TLM_MGR_get_next_register_cmd_pos_(BCT_Pos* next_cmd_pos,
                                                           TLM_MGR_CmdTableCmdElem* next_cmd_elem,
                                                           const TLM_MGR_RegisterInfo* register_info)
{
  uint8_t idx_of_cmd_table_idxes;
  uint8_t cmd_table_idx;
  bct_id_t block;
  uint8_t cmd_pos;

  if (register_info->cmd_table_idxes_size == 0) return TLM_MGR_ERR_CODE_CMD_FULL;
  if (register_info->registered_cmd_num >= register_info->cmd_table_idxes_size * TLM_MGR_MAX_CMD_NUM_PER_BC)
  {
    return TLM_MGR_ERR_CODE_CMD_FULL;
  }

  idx_of_cmd_table_idxes = register_info->registered_cmd_num / register_info->cmd_table_idxes_size;
  cmd_pos = register_info->registered_cmd_num % register_info->cmd_table_idxes_size;

  cmd_table_idx = register_info->cmd_table_idxes[idx_of_cmd_table_idxes];

  next_cmd_elem = &telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx].cmds[cmd_pos];

  block = telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx].bc_id;
  if (BCT_make_pos(next_cmd_pos, block, cmd_pos) != BCT_SUCCESS)
  {
    return TLM_MGR_ERR_CIDE_BCT_ERR;
  }
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_ERR_CODE TLM_MGR_get_last_registered_cmd_pos_(BCT_Pos* last_cmd_pos,
                                                             TLM_MGR_CmdTableCmdElem* last_cmd_elem,
                                                             const TLM_MGR_RegisterInfo* register_info)
{
  uint8_t idx_of_cmd_table_idxes;
  uint8_t cmd_table_idx;
  bct_id_t block;
  uint8_t cmd_pos;

  if (register_info->registered_cmd_num == 0)
  {
    return TLM_MGR_ERR_CODE_CMD_NOT_FOUND;
  }

  idx_of_cmd_table_idxes = (register_info->registered_cmd_num - 1) / register_info->cmd_table_idxes_size;
  cmd_pos = (register_info->registered_cmd_num - 1) % register_info->cmd_table_idxes_size;

  cmd_table_idx = register_info->cmd_table_idxes[idx_of_cmd_table_idxes];

  last_cmd_elem = &telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx].cmds[cmd_pos];

  block = telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx].bc_id;
  if (BCT_make_pos(last_cmd_pos, block, cmd_pos) != BCT_SUCCESS)
  {
    return TLM_MGR_ERR_CIDE_BCT_ERR;
  }
  return TLM_MGR_ERR_CODE_OK;
}


static TLM_MGR_ERR_CODE TLM_MGR_find_registered_cmd_pos_(BCT_Pos* found_cmd_pos,
                                                         TLM_MGR_CmdTableCmdElem* found_cmd_elem,
                                                         const TLM_MGR_RegisterInfo* register_info,
                                                         TLM_MGR_CMD_TYPE cmd_type,
                                                         APID apid,
                                                         TLM_CODE tlm_id,
                                                         uint8_t dr_partition)
{
  uint8_t idx_of_cmd_table_idxes;
  bct_id_t block;
  uint8_t cmd_pos;
  uint8_t cmd_table_idx;
  TLM_MGR_CmdTableCmdElem* cmd_elem;
  uint8_t count = 0;

  if (register_info->registered_cmd_num == 0)
  {
    return TLM_MGR_ERR_CODE_CMD_NOT_FOUND;
  }

  for (cmd_pos = 0; cmd_pos < TLM_MGR_MAX_CMD_NUM_PER_BC; ++cmd_pos)
  {
    for (idx_of_cmd_table_idxes = 0; idx_of_cmd_table_idxes < register_info->cmd_table_idxes_size; ++idx_of_cmd_table_idxes)
    {
      cmd_table_idx = register_info->cmd_table_idxes[idx_of_cmd_table_idxes];
      cmd_elem = &telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx].cmds[cmd_pos];

      RESULT ret = TLM_MGR_check_same_cmd_(cmd_elem,
                                           cmd_type,
                                           apid,
                                           tlm_id,
                                           dr_partition);
      // 見つかった！
      if (ret == RESULT_OK) break;

      count++;
      if (count == register_info->registered_cmd_num) return TLM_MGR_ERR_CODE_CMD_NOT_FOUND;
    }
  }

  found_cmd_elem = cmd_elem;

  block = telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx].bc_id;
  if (BCT_make_pos(found_cmd_pos, block, cmd_pos) != BCT_SUCCESS)
  {
    return TLM_MGR_ERR_CIDE_BCT_ERR;
  }
  return TLM_MGR_ERR_CODE_OK;
}


static RESULT TLM_MGR_check_same_cmd_(const TLM_MGR_CmdTableCmdElem* cmd_elem,
                                      TLM_MGR_CMD_TYPE cmd_type,
                                      APID apid,
                                      TLM_CODE tlm_id,
                                      uint8_t dr_partition)
{
  switch (cmd_type)
  {
  case TLM_MGR_CMD_TYPE_TG_GENERATE_MS_TLM:
    if (cmd_elem->cmd_type == TLM_MGR_CMD_TYPE_TG_GENERATE_MS_TLM &&
        cmd_elem->tlm_id == tlm_id)
    {
      return RESULT_OK;
    }
    break;
  case TLM_MGR_CMD_TYPE_TG_GENERATE_ST_TLM:
    if (cmd_elem->cmd_type == TLM_MGR_CMD_TYPE_TG_GENERATE_ST_TLM &&
        cmd_elem->tlm_id == tlm_id &&
        cmd_elem->dr_partition == dr_partition)
    {
      return RESULT_OK;
    }
    break;
  case TLM_MGR_CMD_TYPE_TG_FORWARD_AS_MS_TLM:
    if (cmd_elem->cmd_type == TLM_MGR_CMD_TYPE_TG_FORWARD_AS_MS_TLM &&
        cmd_elem->apid == apid &&
        cmd_elem->tlm_id == tlm_id)
    {
      return RESULT_OK;
    }
  case TLM_MGR_CMD_TYPE_TG_FORWARD_AS_ST_TLM:
    if (cmd_elem->cmd_type == TLM_MGR_CMD_TYPE_TG_FORWARD_AS_ST_TLM &&
        cmd_elem->apid == apid &&
        cmd_elem->tlm_id == tlm_id &&
        cmd_elem->dr_partition == dr_partition)
    {
      return RESULT_OK;
    }
  case TLM_MGR_CMD_TYPE_DR_REPLAY_TLM:
    if (cmd_elem->cmd_type == TLM_MGR_CMD_TYPE_DR_REPLAY_TLM &&
        cmd_elem->dr_partition == dr_partition)
    {
      return RESULT_OK;
    }
  case TLM_MGR_CMD_TYPE_UNREGISTERED:   // FALLTHROUGH
  default:
    return RESULT_ERR;
  }
  return RESULT_ERR;
}


static void TLM_MGR_load_deploy_bc_(void)
{
  BCL_tool_register_deploy(0, telemetry_manager_.master_bc_id, TLCD_ID_DEPLOY_TLM);
  // BC#9 は TLM_MGR_BC_ROLE_LOW_FREQ_TLM なはず
  BCL_tool_register_rotate(1, telemetry_manager_.cmd_table.cmds_in_block[TLM_MGR_USE_BC_NUM - 1].bc_id);
}


static void TLM_MGR_load_master_bc_(void)
{
  // 1 - TLM_MGR_USE_BC_NUM までの TLM_MGR_USE_BC_NUM 個登録する． TLM_MGR_USE_BC_NUM は deploy も一緒に
  uint8_t cmd_table_idx;

  for (cmd_table_idx = 0; cmd_table_idx < (TLM_MGR_USE_BC_NUM - 1); ++cmd_table_idx)
  {
    TLM_MGR_CmdTableInBlock* cmds_in_block = &telemetry_manager_.cmd_table.cmds_in_block[cmd_table_idx];
    switch (cmds_in_block->bc_role)
    {
    case TLM_MGR_BC_ROLE_HK_TLM:        // FALLTHROUGH
    case TLM_MGR_BC_ROLE_HIGH_FREQ_TLM:
      BCL_tool_register_combine(cmd_table_idx + 1, cmds_in_block->bc_id);
      break;
    case TLM_MGR_BC_ROLE_LOW_FREQ_TLM:
      BCL_tool_register_rotate(cmd_table_idx + 1, cmds_in_block->bc_id);
      break;
    default:
      break;
    }
  }

  // TLM_MGR_USE_BC_NUM 番目は deploy も一緒に
  BCL_tool_register_combine(TLM_MGR_USE_BC_NUM, telemetry_manager_.deploy_bc_id);
}


static void TLM_MGR_load_nop_bc_(void)
{
  cycle_t ti;

  for (ti = 0; ti < TLM_MGR_MAX_CMD_NUM_PER_BC; ++ti)
  {
    BCL_tool_register_cmd(ti, Cmd_CODE_NOP);
  }
}


static void TLM_MGR_clear_bc_to_nop_(bct_id_t bc_id)
{
  BCL_load_bc(bc_id, TLM_MGR_load_nop_bc_);
  // この中で BCT_activate_block() もされることに注意
}


static CCP_CmdRet TLM_MGR_conv_err_code_to_ccp_cmd_ret_(TLM_MGR_ERR_CODE code)
{
  switch (code)
  {
  case TLM_MGR_ERR_CODE_OK:
    return CCP_make_cmd_ret(CCP_EXEC_SUCCESS, 0);
  case TLM_MGR_ERR_CIDE_INVALID_BC_ROLE:    // FALLTHROUGH
  case TLM_MGR_ERR_CIDE_INVALID_CMD_TYPE:
    return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_PARAMETER, (uint32_t)code);
  case TLM_MGR_ERR_CODE_CMD_FULL:           // FALLTHROUGH
  case TLM_MGR_ERR_CIDE_BCT_ERR:            // FALLTHROUGH
  case TLM_MGR_ERR_CODE_OTHER_ERR:
  default:
    return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, (uint32_t)code);
  }
}


CCP_CmdRet Cmd_TLM_MGR_INIT(const CommonCmdPacket* packet)
{
  RESULT ret;
  uint16_t exec_counter;
  const uint16_t exec_counter_offset = 2;
  (void)packet;

  switch (DCU_check_in(Cmd_CODE_TLM_MGR_INIT, &exec_counter))
  {
  case DCU_STATUS_FINISHED:   // FALLTHROUGH
  case DCU_STATUS_PROGRESS:
    break;
  default:
    // DCU_STATUS_ABORTED_BY_ERR
    // DCU_STATUS_ABORTED_BY_CMD
    // がここに
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  if (exec_counter == 0)
  {
    ret = TLM_MGR_init_1_();
  }
  else if (exec_counter == 1)
  {
    ret = TLM_MGR_init_2_();
  }
  else if (exec_counter >= exec_counter_offset + TLM_MGR_USE_BC_NUM)
  {
    ret = RESULT_ERR;
  }
  else
  {
    TLM_MGR_clear_bc_to_nop_(telemetry_manager_.cmd_table.cmds_in_block[exec_counter - exec_counter_offset].bc_id);
    ret = RESULT_OK;
  }

  if (ret != RESULT_OK)
  {
    DCU_report_err(Cmd_CODE_TLM_MGR_INIT, CCP_EXEC_ILLEGAL_CONTEXT);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  if (exec_counter == exec_counter_offset + TLM_MGR_USE_BC_NUM - 1)
  {
    DCU_report_finish(Cmd_CODE_TLM_MGR_INIT, CCP_EXEC_SUCCESS);
    telemetry_manager_.is_inited = 1;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  }

  // 再帰実行
  if (DCU_register_next(Cmd_CODE_TLM_MGR_INIT, NULL, 0) != DCU_ACK_OK)
  {
    DCU_report_err(Cmd_CODE_TLM_MGR_INIT, CCP_EXEC_ILLEGAL_CONTEXT);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_TLM_MGR_INIT_MASTER_AND_DEPLOY_BC(const CommonCmdPacket* packet)
{
  (void)packet;

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  BCL_load_bc(telemetry_manager_.master_bc_id, TLM_MGR_load_master_bc_);
  BCL_load_bc(telemetry_manager_.deploy_bc_id, TLM_MGR_load_deploy_bc_);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_TLM_MGR_CLEAR_HK_TLM(const CommonCmdPacket* packet)
{
  (void)packet;
  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  // DCU を使う
  return TLM_MGR_clear_cmds_based_on_role_(Cmd_CODE_TLM_MGR_CLEAR_HK_TLM,
                                           TLM_MGR_BC_ROLE_HK_TLM,
                                           &telemetry_manager_.register_info.hk);
}


CCP_CmdRet Cmd_TLM_MGR_CLEAR_HIGH_FREQ_TLM(const CommonCmdPacket* packet)
{
  (void)packet;
  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  // DCU を使う
  return TLM_MGR_clear_cmds_based_on_role_(Cmd_CODE_TLM_MGR_CLEAR_HIGH_FREQ_TLM,
                                           TLM_MGR_BC_ROLE_HIGH_FREQ_TLM,
                                           &telemetry_manager_.register_info.high_freq);
}


CCP_CmdRet Cmd_TLM_MGR_CLEAR_LOW_FREQ_TLM(const CommonCmdPacket* packet)
{
  (void)packet;
  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  // DCU を使う
  return TLM_MGR_clear_cmds_based_on_role_(Cmd_CODE_TLM_MGR_CLEAR_LOW_FREQ_TLM,
                                           TLM_MGR_BC_ROLE_LOW_FREQ_TLM,
                                           &telemetry_manager_.register_info.low_freq);
}


CCP_CmdRet Cmd_TLM_MGR_CLEAR_USER_TLM(const CommonCmdPacket* packet)
{
  (void)packet;
  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  // NOTE: DCU を多重で使っている！ (つまり， DCU_LOG_MAX が 2 以上でないとだめ)
  CCP_form_and_exec_rtc(Cmd_CODE_TLM_MGR_CLEAR_HIGH_FREQ_TLM, NULL, 0);
  CCP_form_and_exec_rtc(Cmd_CODE_TLM_MGR_CLEAR_LOW_FREQ_TLM, NULL, 0);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_TLM_MGR_START_TLM(const CommonCmdPacket* packet)
{
  CCP_CmdRet cmd_ret;
  (void)packet;

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  cmd_ret = CCP_form_and_exec_rtc(Cmd_CODE_TLM_MGR_STOP_TLM, NULL, 0);
  if (cmd_ret.exec_sts != CCP_EXEC_SUCCESS)
  {
    CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, 0);
  }

  cmd_ret = CCP_form_and_exec_block_deploy_cmd(TLCD_ID_DEPLOY_TLM, telemetry_manager_.master_bc_id);
  if (cmd_ret.exec_sts != CCP_EXEC_SUCCESS)
  {
    CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, 1);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_TLM_MGR_STOP_TLM(const CommonCmdPacket* packet)
{
  uint8_t param[1];
  (void)packet;
  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  param[0] = TLCD_ID_DEPLOY_TLM;
  return CCP_form_and_exec_rtc(Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE, param, 1);
}


CCP_CmdRet Cmd_TLM_MGR_REGISTER_GENERATE_MS_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_register_(bc_role, TLM_MGR_CMD_TYPE_TG_GENERATE_MS_TLM, APID_UNKNOWN, tlm_id, 0);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_REGISTER_GENERATE_ST_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);
  uint8_t dr_partition = CCP_get_param_from_packet(packet, 2, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_register_(bc_role, TLM_MGR_CMD_TYPE_TG_GENERATE_ST_TLM, APID_UNKNOWN, tlm_id, dr_partition);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_REGISTER_FORWARD_AS_MS_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  APID apid = (APID)CCP_get_param_from_packet(packet, 1, uint16_t);
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 2, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_register_(bc_role, TLM_MGR_CMD_TYPE_TG_FORWARD_AS_MS_TLM, apid, tlm_id, 0);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_REGISTER_FORWARD_AS_ST_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  APID apid = (APID)CCP_get_param_from_packet(packet, 1, uint16_t);
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 2, uint8_t);
  uint8_t dr_partition = CCP_get_param_from_packet(packet, 3, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_register_(bc_role, TLM_MGR_CMD_TYPE_TG_FORWARD_AS_ST_TLM, apid, tlm_id, dr_partition);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_REGISTER_REPLAY_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  uint8_t dr_partition = CCP_get_param_from_packet(packet, 1, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_register_(bc_role, TLM_MGR_CMD_TYPE_DR_REPLAY_TLM, APID_UNKNOWN, TLM_CODE_MAX, dr_partition);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_DELETE_GENERATE_MS_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_delete_(bc_role, TLM_MGR_CMD_TYPE_TG_GENERATE_MS_TLM, APID_UNKNOWN, tlm_id, 0);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_DELETE_GENERATE_ST_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);
  uint8_t dr_partition = CCP_get_param_from_packet(packet, 2, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_delete_(bc_role, TLM_MGR_CMD_TYPE_TG_GENERATE_ST_TLM, APID_UNKNOWN, tlm_id, dr_partition);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_DELETE_FORWARD_AS_MS_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  APID apid = (APID)CCP_get_param_from_packet(packet, 1, uint16_t);
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 2, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_delete_(bc_role, TLM_MGR_CMD_TYPE_TG_FORWARD_AS_MS_TLM, apid, tlm_id, 0);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_DELETE_FORWARD_AS_ST_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  APID apid = (APID)CCP_get_param_from_packet(packet, 1, uint16_t);
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 2, uint8_t);
  uint8_t dr_partition = CCP_get_param_from_packet(packet, 3, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_delete_(bc_role, TLM_MGR_CMD_TYPE_TG_FORWARD_AS_ST_TLM, apid, tlm_id, dr_partition);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}


CCP_CmdRet Cmd_TLM_MGR_DELETE_REPLAY_TLM(const CommonCmdPacket* packet)
{
  TLM_MGR_ERR_CODE err_code;
  TLM_MGR_BC_ROLE bc_role = (TLM_MGR_BC_ROLE)CCP_get_param_from_packet(packet, 0, uint8_t);
  uint8_t dr_partition = CCP_get_param_from_packet(packet, 1, uint8_t);

  if (telemetry_manager_.is_inited == 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  err_code = TLM_MGR_delete_(bc_role, TLM_MGR_CMD_TYPE_DR_REPLAY_TLM, APID_UNKNOWN, TLM_CODE_MAX, dr_partition);
  return TLM_MGR_conv_err_code_to_ccp_cmd_ret_(err_code);
}

#pragma section
