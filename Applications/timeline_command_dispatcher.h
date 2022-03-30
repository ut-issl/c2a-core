#ifndef TIMELINE_COMMAND_DISPATCHER_H_
#define TIMELINE_COMMAND_DISPATCHER_H_

/**
 * @enum   TL_ID
 * @brief  TimeLineを選ぶときに統一的に使うコード
 * @note   uint8_tを想定
 */
typedef enum
{
  TL_ID_DEPLOY_FROM_GS = 0,
  TL_ID_DEPLOY_BC      = 1,
  TL_ID_DEPLOY_TLM     = 2,
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
extern const int* TLCD_line_no_for_tlm;
extern const cycle_t* TLCD_tl_tlm_updated_at;
extern const CommonCmdPacket* TLCD_tl_list_for_tlm[PH_TL0_CMD_LIST_MAX];
extern const int* TLCD_page_no;

AppInfo TLCD0_create_app(void);
AppInfo TLCD1_create_app(void);
AppInfo TLCD2_create_app(void);


/**
 * @brief CCP が時系列に並ぶように CCP を挿入する
 * @note TimeLine だけでなく TaskList もこれを使い，その場合， now は step_t になることに注意
 * @note asap と違うのは，そこに挿入出来なければ挿入しない
 * @param[in,out] pl: CCP を挿入する PacketList
 * @param[in] packet: 挿入する CCP
 * @param[in] now: 基準時刻 (TimeLine なら現在時刻， TaskList なら現在 step)
 * @return PL_ACK
 */
PL_ACK TLCD_insert_tl_cmd_strictly(PacketList* pl, const CommonCmdPacket* packet, cycle_t now);

/**
 * @brief CCP が時系列に並ぶように CCP を挿入する
 * @note strictly と違うのは同時刻に既に入ってもいてもそこ以降に最速で入れる
 * @param[in,out] pl: CCP を挿入する PacketList
 * @param[in] packet: 挿入する CCP
 * @param[in] now: 基準時刻 (TimeLine なら現在時刻， TaskList なら現在 step)
 * @return PL_ACK
 */
PL_ACK TLCD_insert_tl_cmd_asap(PacketList* pl, const CommonCmdPacket* packet, cycle_t now);

/**
 * @brief TLM の内容を自動更新する.
 * @param[in] line_no TLCD_line_no_for_tlm が入ることを想定している
 * @return uint8_t 引数の line_no を返し, TL TLM にも反映される.
 * @note これが TLM の冒頭で呼ばれることで TLM の内容が勝手に更新される.
 */
uint8_t TLCD_update_tl_list_for_tlm(uint8_t line_no);

CCP_EXEC_STS Cmd_TLCD_CLEAR_ALL_TIMELINE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_CLEAR_TIMELINE_AT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_SOE_FLAG(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_LOUT_FLAG(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_LINE_NO_FOR_TIMELINE_TLM(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_DEPLOY_BLOCK(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_TLCD_CLEAR_ERR_LOG(const CommonCmdPacket* packet);

#endif
