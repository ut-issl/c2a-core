/**
 * @file
 * @brief  ALのCoreパラメタのオーバーライド用ヘッダー
 */
#ifndef ANOMALY_LOGGER_PARAMS_H_
#define ANOMALY_LOGGER_PARAMS_H_

#undef AL_TLM_PAGE_SIZE
#undef AL_TLM_PAGE_MAX
#undef AL_RECORD_MAX

#define AL_TLM_PAGE_SIZE (32)
#define AL_TLM_PAGE_MAX  (4)
#define AL_RECORD_MAX    (AL_TLM_PAGE_SIZE * AL_TLM_PAGE_MAX)

#define AL_DISALBE_AT_C2A_CORE

#endif
