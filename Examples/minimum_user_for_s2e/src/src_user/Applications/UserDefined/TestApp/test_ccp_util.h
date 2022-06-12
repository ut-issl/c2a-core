/**
 * @file
 * @brief CCP 汎用 utility テスト用 app
 */
#ifndef TEST_CCP_UTIL_H_
#define TEST_CCP_UTIL_H_

#include <src_core/TlmCmd/common_cmd_packet.h>

CCP_EXEC_STS Cmd_TEST_CCP_REGISTER_TLC_ASAP(const CommonCmdPacket* packet);

#endif
