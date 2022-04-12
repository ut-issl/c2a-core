#ifndef TIMELINE_COMMAND_DISPATCHER_H_
#define TIMELINE_COMMAND_DISPATCHER_H_

#include <src_user/Settings/Applications/timeline_command_dispatcher_define.h>

/**
 * @enum   TL_ID
 * @brief  TimeLineを選ぶときに統一的に使うコード
 * @note   uint8_tを想定
 */
typedef enum
{
  TL_ID_FROM_GS = 0,
  TL_ID_DEPLOY_BC,
  TL_ID_DEPLOY_TLM,
#ifdef TLCD_ENABLE_MISSION_TL
  TL_ID_FROM_GS_FOR_MISSION,
#endif
  TL_ID_MAX
} TL_ID;
// FIXME: 全体的に， line_no が int なので， enum にする
// FIXME: TLを3本以上にできるようにする

// 循環参照を防ぐためにここでinclude
#include "../TlmCmd/command_dispatcher.h"
#include "../TlmCmd/common_cmd_packet.h"
#include "../TlmCmd/packet_handler.h"
#include "../System/ApplicationManager/app_info.h"

// FIXME: 構造体にする
extern const CommandDispatcher* const timeline_command_dispatcher;
extern const TL_ID* TLCD_line_no_for_tlm;
extern const cycle_t* TLCD_tl_tlm_updated_at;
extern const CommonCmdPacket* TLCD_tl_list_for_tlm[PH_TL0_CMD_LIST_MAX];
extern const uint8_t* TLCD_page_no;

/**
 * @brief TL0 (GS から登録されるバス用の Timeline) の実行 App を作成する
 * @param void
 * @return AppInfo
 */
AppInfo TLCD_gs_create_app(void);

/**
 * @brief TL1 (BC を展開する用の Timeline) の実行 App を作成する
 * @param void
 * @return AppInfo
 */
AppInfo TLCD_bc_create_app(void);

/**
 * @brief TL2 (TLM を展開する用の Timeline) の実行 App を作成する
 * @param void
 * @return AppInfo
 */
AppInfo TLCD_tlm_create_app(void);

#ifdef TLCD_ENABLE_MISSION_TL
/**
 * @brief TL3 (GS から登録されるミッション用の Timeline) の実行 App を作成する
 * @param void
 * @return AppInfo
 */
AppInfo TLCD_mis_create_app(void);
#endif

/**
 * @brief TLM の内容を自動更新する.
 * @param[in] line_no TLCD_line_no_for_tlm が入ることを想定している
 * @return uint8_t 引数の line_no を返し, TL TLM にも反映される.
 * @note これが TLM の冒頭で呼ばれることで TLM の内容が勝手に更新される.
 */
TL_ID TLCD_update_tl_list_for_tlm(TL_ID line_no);

/**
 * @brief tl_list のうち指定された位置の tl_cmd の packet を取得する
 * @note TLテレメで用いる
 * @param offset その page_no の中で0起算で何番目の packet か
 * @return packet へのポインタ
 */
const CommonCmdPacket* TLCD_get_packet_for_tl_tlm(uint8_t offset);

CCP_EXEC_STS Cmd_TLCD_CLEAR_ALL_TIMELINE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_CLEAR_TIMELINE_AT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_SOE_FLAG(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_LOUT_FLAG(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_LINE_NO_FOR_TIMELINE_TLM(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_DEPLOY_BLOCK(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_CLEAR_ERR_LOG(const CommonCmdPacket* packet);

#endif
