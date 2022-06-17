/**
 * @file
 * @brief event_handler の各種設定
 *        各パラメタ類の詳細は event_handler.h を参照すること
 */
#ifndef EVENT_HANDLER_PARAMS_H_
#define EVENT_HANDLER_PARAMS_H_

#undef EH_RULE_TLM_PAGE_SIZE
#undef EH_RULE_TLM_PAGE_MAX
#undef EH_LOG_TLM_PAGE_SIZE
#undef EH_LOG_TLM_PAGE_MAX
#undef EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES
#undef EH_MAX_RESPONSE_NUM_DEFAULT
#undef EH_MAX_CHECK_EVENT_NUM_DEFAULT
#undef EH_MAX_MULTI_LEVEL_NUM_DEFAULT


#define EH_RULE_TLM_PAGE_SIZE (20)
#define EH_RULE_TLM_PAGE_MAX  (8)
#define EH_LOG_TLM_PAGE_SIZE  (64)
#define EH_LOG_TLM_PAGE_MAX   (2)
#define EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES   (4)
#define EH_MAX_RESPONSE_NUM_DEFAULT           (8)
#define EH_MAX_CHECK_EVENT_NUM_DEFAULT        (64)
#define EH_MAX_MULTI_LEVEL_NUM_DEFAULT        (4)

#endif
