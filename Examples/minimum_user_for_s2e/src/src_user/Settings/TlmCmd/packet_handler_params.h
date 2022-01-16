/**
 * @file
 * @brief  PHのCoreTlmパラメタのオーバーライド用ヘッダー
 */
#ifndef PACKET_HANDLER_PARAMS_H_
#define PACKET_HANDLER_PARAMS_H_

#undef TL_TLM_PAGE_SIZE
#undef TL_TLM_PAGE_MAX

#undef PH_GS_CMD_LIST_MAX
#undef PH_RT_CMD_LIST_MAX
#undef PH_TL0_CMD_LIST_MAX
#undef PH_TL1_CMD_LIST_MAX
#undef PH_TL2_CMD_LIST_MAX
#undef PH_MS_TLM_LIST_MAX
#undef PH_ST_TLM_LIST_MAX
#undef PH_RP_TLM_LIST_MAX

#define TL_TLM_PAGE_SIZE      (32)
#define TL_TLM_PAGE_MAX       (8)

#define PH_GS_CMD_LIST_MAX       (8)
#define PH_RT_CMD_LIST_MAX       (32)
#define PH_TL0_CMD_LIST_MAX      (TL_TLM_PAGE_SIZE * TL_TLM_PAGE_MAX)   // コメント追加（2019/08/19）
                                                                        // これが最長じゃないといけない
                                                                        // TLCD_tl_list_for_tlmの長さがこれなので！！
#define PH_TL1_CMD_LIST_MAX      (TL_TLM_PAGE_SIZE * 4)
#define PH_TL2_CMD_LIST_MAX      (TL_TLM_PAGE_SIZE * 4)
#define PH_MS_TLM_LIST_MAX       (16)
#define PH_ST_TLM_LIST_MAX       (16)
#define PH_RP_TLM_LIST_MAX       (16)

#endif
