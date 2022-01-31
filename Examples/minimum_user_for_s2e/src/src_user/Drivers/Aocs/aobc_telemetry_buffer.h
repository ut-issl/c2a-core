/**
 * @file
 * @brief  テレメトリバッファー（テレメ中継）
 * @note   このコードは自動生成されています！
 */
#ifndef AOBC_TELEMETRY_BUFFER_H_
#define AOBC_TELEMETRY_BUFFER_H_

#include "./aobc.h"

#define AOBC_TELEMETRY_BUFFE_SIZE (512)

typedef struct
{
  struct
  {
    int     size;
    uint8_t buffer[AOBC_TELEMETRY_BUFFE_SIZE];
  } aobc_aobc;
  struct
  {
    int     size;
    uint8_t buffer[AOBC_TELEMETRY_BUFFE_SIZE];
  } aobc_hk;
} AOBC_Buffer;

extern const AOBC_Buffer* const aobc_buffer;

void AOBC_buffer_init(void);

DS_ERR_CODE AOBC_buffer_tlm_contents(DS_StreamConfig* p_stream_config, AOBC_Driver* aobc_driver);

#endif
