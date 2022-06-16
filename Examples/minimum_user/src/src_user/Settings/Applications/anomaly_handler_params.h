/**
 * @file
 * @brief  AHのCoreAppパラメタのオーバーライド用ヘッダー
 */
#ifndef ANOMALY_HANDLER_PARAMS_H_
#define ANOMALY_HANDLER_PARAMS_H_

#undef AH_TLM_PAGE_SIZE
#undef AH_TLM_PAGE_MAX
#undef AH_MAX_RULES

#define AH_TLM_PAGE_SIZE (32)
#define AH_TLM_PAGE_MAX  (4)
#define AH_MAX_RULES (AH_TLM_PAGE_SIZE * AH_TLM_PAGE_MAX)

#undef AH_LOG_TLM_PAGE_SIZE
#undef AH_LOG_TLM_PAGE_MAX
#undef AH_LOG_MAX

#define AH_LOG_TLM_PAGE_SIZE (32)
#define AH_LOG_TLM_PAGE_MAX  (4)
#define AH_LOG_MAX (AH_LOG_TLM_PAGE_SIZE * AH_LOG_TLM_PAGE_MAX)

#endif
