#ifndef TIMELINE_COMMAND_DISPATCHER_H_
#define TIMELINE_COMMAND_DISPATCHER_H_

#include <src_user/Settings/Applications/timeline_command_dispatcher_define.h>

/**
 * @enum   TLCD_ID
 * @brief  TimeLineを選ぶときに統一的に使うコード
 * @note   uint8_tを想定
 */
typedef enum
{
  TLCD_ID_FROM_GS = 0,
  TLCD_ID_DEPLOY_BC,
  TLCD_ID_DEPLOY_TLM,
#ifdef TLCD_ENABLE_MISSION_TL
  TLCD_ID_FROM_GS_FOR_MISSION,
#endif
  TLCD_ID_MAX
} TLCD_ID;
// FIXME: TL本数を可変にできるようにする

// 循環参照を防ぐためにここでinclude
#include "../TlmCmd/command_dispatcher.h"
#include "../TlmCmd/common_cmd_packet.h"
#include "../TlmCmd/packet_handler.h"
#include "../System/ApplicationManager/app_info.h"

/**
 * @struct TimelineCommandDispatcher
 * @brief Timeline コマンドの実行部と, TLテレメの情報を保持する
 */
typedef struct
{
  CommandDispatcher dispatcher[TLCD_ID_MAX];
  struct
  {
    TLCD_ID id;
    uint8_t page_no;
    cycle_t updated_at;
    const CommonCmdPacket* tl_list[PH_TLC_GS_LIST_MAX]; // TL0が最長なのでそれに合わせる。
  } tlm_info_;
} TimelineCommandDispatcher;

extern const TimelineCommandDispatcher* const timeline_command_dispatcher;

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
 * @brief TL2 (TLM を登録する用の Timeline) の実行 App を作成する
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
 * @brief テレメの内容を自動更新する.
 * @param[in] id テレメを表示する TL の id
 * @return TLCD_ID 引数の id を返し, TL テレメにも反映される.
 * @note これが TLM の冒頭で呼ばれることで TLM の内容が勝手に更新される.
 */
TLCD_ID TLCD_update_tl_list_for_tlm(TLCD_ID id);

CCP_EXEC_STS Cmd_TLCD_CLEAR_ALL_TIMELINE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_CLEAR_TIMELINE_AT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_DEPLOY_BLOCK(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_CLEAR_ERR_LOG(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_SOE_FLAG(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_LOUT_FLAG(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_ID_FOR_TLM(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);

#endif
