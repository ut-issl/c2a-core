/**
 * @file
 * @brief  CTP 型の定義
 */
#ifndef COMMON_TLM_PACKET_DEFINE_H_
#define COMMON_TLM_PACKET_DEFINE_H_

#include <src_core/TlmCmd/Ccsds/TCPacket.h>

// CommonTlmPacket として TCP をつかう
// FIXME: TlmSpacePacket に差し替える
typedef TCP CommonTlmPacket;

#endif
