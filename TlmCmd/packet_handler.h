/**
 * @file
 * @brief C2A 全体を流れる Common Packet の配送を制御する
 */
#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

#include "common_tlm_cmd_packet.h"
#include "packet_list.h"
#include "../Applications/timeline_command_dispatcher_id_define.h"

#define TL_TLM_PAGE_SIZE  (32)
#define TL_TLM_PAGE_MAX   (8)

#define PH_GSC_LIST_MAX  (8)
#define PH_RTC_LIST_MAX  (32)
#define PH_TLC_GS_LIST_MAX (TL_TLM_PAGE_SIZE * TL_TLM_PAGE_MAX)   // これが最長じゃないといけない
                                                                   // TLCD_tl_list_for_tlmの長さがこれなので！！
#define PH_TLC_BC_LIST_MAX (TL_TLM_PAGE_SIZE * 4)
#define PH_TLC_TLM_LIST_MAX (TL_TLM_PAGE_SIZE * 4)
#define PH_TLC_MIS_LIST_MAX (TL_TLM_PAGE_SIZE * 4)   // とりあえず TL1,2 と同じ長さにした
#define PH_MS_TLM_LIST_MAX  (16)
#define PH_ST_TLM_LIST_MAX  (16)
#define PH_RP_TLM_LIST_MAX  (16)

// 以下で，上記の PL のキューサイズを再定義する
// また， data_recorder_define.h の #define DR_ENABLE をコメントアウトすると，
// DR 関連 PL がすべて無効となり，メモリが節約できる
#include <src_user/Settings/TlmCmd/packet_handler_params.h>

#ifndef DR_ENABLE
#ifdef PH_ST_TLM_LIST_MAX
#undef PH_ST_TLM_LIST_MAX
#endif
#ifdef PH_RP_TLM_LIST_MAX
#undef PH_RP_TLM_LIST_MAX
#endif
#endif

// FIXME: 整理したい
typedef enum
{
  PH_ACK_SUCCESS,             //!< OK
  PH_ACK_FORWARDED,           //!< 別 OBC に転送された
  PH_ACK_PL_LIST_FULL,        //!< PL が一杯だった
  PH_ACK_PACKET_NOT_FOUND,    //!< パケットが無い
  PH_ACK_INVALID_PACKET,      //!< 無効な Packet
  PH_ACK_TLC_SUCCESS,         //!< TL 登録に成功した
  PH_ACK_TLC_PAST_TIME,       //!< 既に実行時刻を過ぎている
  PH_ACK_TLC_ALREADY_EXISTS,  //!< 同 TI に既に packet がいる
  PH_ACK_BC_SUCCESS,          //!< BC 登録に成功した
  PH_ACK_BC_INVALID_BLOCK_NO, //!< 無効な BC 番号だった
  PH_ACK_BC_ISORATED_CMD,     //!< 飛ばして BC 登録しようとした
  PH_ACK_BC_CMD_TOO_LONG,     //!< CMD が BC には長すぎる
  PH_ACK_UNKNOWN
} PH_ACK;

extern PacketList PH_gs_cmd_list;
extern PacketList PH_rt_cmd_list;
extern PacketList PH_tl_cmd_list[TLCD_ID_MAX];
// extern PacketList PH_hk_tlm_list;    // 現在は MS_TLM に統合されている（ TODO: 今後また分離させても良いかも．要検討）
extern PacketList PH_ms_tlm_list;
#ifdef DR_ENABLE
extern PacketList PH_st_tlm_list;
extern PacketList PH_rp_tlm_list;
#endif

/**
 * @brief Packet Handler を初期化
 * @param  void
 * @return void
 */
void PH_init(void);

/**
 * @brief  CTCP を解析する
 * @note   パケット解析とは，パケットを適切なキューにエンキューすることである（実行することではない）
 * @note   Common Packet とは， GS から送られてくるものだけでなく，
 *         C2A 内部で発行されるものや，他の OBC から送られてくるものなどもある
 * @note   Cmd か Tlm かが確定している場合， PH_analyze_tlm_packet, PH_analyze_cmd_packet を用いる
 * @param  packet: 解析するパケット
 * @return PH_ACK
 */
PH_ACK PH_analyze_packet(const CommonTlmCmdPacket* packet);

/**
 * @brief  CTP を解析する
 * @note   パケット解析とは，パケットを適切なキューにエンキューすることである（実行することではない）
 * @param  packet: 解析するパケット
 * @return PH_ACK
 */
PH_ACK PH_analyze_tlm_packet(const CommonTlmPacket* packet);

/**
 * @brief  CCP を解析する
 * @note   パケット解析とは，パケットを適切なキューにエンキューすることである（実行することではない）
 * @param  packet: 解析するパケット
 * @return PH_ACK
 */
PH_ACK PH_analyze_cmd_packet(const CommonCmdPacket* packet);

/**
 * @brief  CCP をコマンドとして解釈して実行，ないしは別機器へ配送する
 * @param  packet: 実行 or 配送するコマンドパケット
 * @return CCP_CmdRet
 */
CCP_CmdRet PH_dispatch_command(const CommonCmdPacket* packet);

/**
 * @brief CCP_EXEC_TYPE から PacketList を取得する
 * @note  引数が不正 or 無い場合 NULL を返す
 * @param[in] type: CCP_EXEC_TYPE
 * @return PacketList の const ポインタ
 */
const PacketList* PH_get_packet_list_from_exec_type(CCP_EXEC_TYPE type);

#endif
