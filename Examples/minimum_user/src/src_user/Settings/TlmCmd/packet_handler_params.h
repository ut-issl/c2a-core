/**
 * @file
 * @brief  PHのCoreTlmパラメタのオーバーライド用ヘッダー
 */
#ifndef PACKET_HANDLER_PARAMS_H_
#define PACKET_HANDLER_PARAMS_H_

// TL_MIS, DR の利用の有無で必要な PL が変わるので， include する
#include "../Applications/timeline_command_dispatcher_define.h"
#include "../Applications/data_recorder_define.h"


#undef TL_TLM_PAGE_SIZE
#undef TL_TLM_PAGE_MAX

#undef PH_GSC_LIST_MAX
#undef PH_RTC_LIST_MAX
#undef PH_TLC_GS_LIST_MAX
#undef PH_TLC_BC_LIST_MAX
#undef PH_TLC_TLM_LIST_MAX
#undef PH_TLC_MIS_LIST_MAX
#undef PH_MS_TLM_LIST_MAX
#undef PH_ST_TLM_LIST_MAX
#undef PH_RP_TLM_LIST_MAX

#define TL_TLM_PAGE_SIZE      (32)
#define TL_TLM_PAGE_MAX       (8)

#define PH_GSC_LIST_MAX       (8)
#define PH_RTC_LIST_MAX       (32)
#define PH_TLC_GS_LIST_MAX    (TL_TLM_PAGE_SIZE * TL_TLM_PAGE_MAX)  // コメント追加（2019/08/19）
                                                                    // これが最長じゃないといけない
                                                                    // timeline_command_dispatcher.tlm_info_.tl_list の長さがこれなので
#define PH_TLC_BC_LIST_MAX    (TL_TLM_PAGE_SIZE * 4)
#define PH_TLC_TLM_LIST_MAX   (TL_TLM_PAGE_SIZE * 4)
#ifdef TLCD_ENABLE_MISSION_TL
#define PH_TLC_MIS_LIST_MAX   (TL_TLM_PAGE_SIZE * 4)   // とりあえず TL_DEPLOY_BC, TL_DEPLOY_TLM と同じ長さにした
#endif
#define PH_MS_TLM_LIST_MAX    (16)
#ifdef DR_ENABLE
#define PH_ST_TLM_LIST_MAX    (16)
#define PH_RP_TLM_LIST_MAX    (16)
#endif

#endif
