#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

#define TL_TLM_PAGE_SIZE  (32)
#define TL_TLM_PAGE_MAX   (8)

#define PH_GS_CMD_LIST_MAX  (8)
#define PH_RT_CMD_LIST_MAX  (32)
#define PH_TL0_CMD_LIST_MAX (TL_TLM_PAGE_SIZE * TL_TLM_PAGE_MAX)   // コメント追加（2019/08/19）
                                                                   // これが最長じゃないといけない
                                                                   // TLCD_tl_list_for_tlmの長さがこれなので！！
#define PH_TL1_CMD_LIST_MAX (TL_TLM_PAGE_SIZE * 4)
#define PH_TL2_CMD_LIST_MAX (TL_TLM_PAGE_SIZE * 4)
#define PH_MS_TLM_LIST_MAX  (16)
#define PH_ST_TLM_LIST_MAX  (16)
#define PH_RP_TLM_LIST_MAX  (16)

#include <src_user/Settings/TlmCmd/packet_handler_params.h>

// 循環参照を防ぐためにここでinclude
#include "common_tlm_cmd_packet.h"
#include "packet_list.h"
#include "../Applications/timeline_command_dispatcher.h"

typedef enum
{
  PH_SUCCESS,
  PH_REGISTERED,
  PH_FORWARDED,
  PH_INVALID_LENGTH,
  PH_INVALID_CRC,
  PH_INVALID_TO_ID,
  PH_INVALID_DISCRIMINATOR,
  PH_INVALID_CMD_TYPE,
  PH_PL_LIST_FULL,
  PH_TLC_REGISTERD,
  PH_TLC_PAST_TIME,
  PH_TLC_ALREADY_EXISTS,
  PH_TLC_NOT_EXECUTED,
  PH_TLC_NOT_FOUND,
  PH_BC_REGISTERED,
  PH_BC_INVALID_BLOCK_NO,
  PH_BC_INVALID_CMD_NO,
  PH_BC_ISORATED_CMD,
  PH_BC_CMD_TOO_LONG,
  PH_BC_INACTIVE_BLOCK,
  PH_BC_COMBINE_FAILED,
  PH_BC_ROTATE_FAILED,
  PH_BC_ZERO_PERIOD,         // BCT_rotate_block_cmd_で使う(本来このenumに乗せるべきかは要検討)
  PH_TLM_INVALID_DIST_FLAG,
  PH_UNKNOWN
} PH_ACK;

extern PacketList PH_gs_cmd_list;
extern PacketList PH_rt_cmd_list;
extern PacketList PH_tl_cmd_list[TL_ID_MAX];
// extern PacketList PH_hk_tlm_list;    // EQUではMS TLMに統合
extern PacketList PH_ms_tlm_list;
extern PacketList PH_st_tlm_list;
extern PacketList PH_rp_tlm_list;

void PH_init(void);

PH_ACK PH_analyze_packet(const CTCP* packet);

CCP_EXEC_STS PH_dispatch_command(const CTCP* packet);

#endif
