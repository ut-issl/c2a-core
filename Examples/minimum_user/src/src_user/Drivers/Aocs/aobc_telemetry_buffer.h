/**
 * @file
 * @brief  テレメトリバッファー（テレメ中継）
 * @note   このコードは自動生成されています！
 */
#ifndef AOBC_TELEMETRY_BUFFER_H_
#define AOBC_TELEMETRY_BUFFER_H_

#include "./aobc_telemetry_definitions.h"
#include <src_core/Drivers/Super/driver_super.h>
#include <src_core/TlmCmd/common_tlm_packet.h>
#include <src_core/TlmCmd/telemetry_frame.h>

typedef struct AOBC_Driver AOBC_Driver;

#define AOBC_MAX_TLM_NUM (256)

typedef struct
{
  CommonTlmPacket packet;   //!< 最新のテレメパケットを保持
  uint8_t is_null_packet;   //!< 一度でもテレメを受信しているか？（空配列が読み出されるのを防ぐため）
} AOBC_TlmBufferElem;

typedef struct
{
  AOBC_TlmBufferElem tlm[AOBC_MAX_TLM_NUM];   //!< TLM ID ごとに保持
} AOBC_TlmBuffer;

void AOBC_init_tlm_buffer(AOBC_Driver* aobc_driver);

DS_ERR_CODE AOBC_buffer_tlm_packet(DS_StreamConfig* p_stream_config, AOBC_Driver* aobc_driver);

TF_TLM_FUNC_ACK AOBC_pick_up_tlm_buffer(const AOBC_Driver* aobc_driver, AOBC_TLM_CODE tlm_id, uint8_t* packet, uint16_t* len, uint16_t max_len);

#endif
