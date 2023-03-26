/**
 * @file
 * @brief CCP 汎用 utility テスト用 app
 */
#ifndef TEST_CCP_UTIL_H_
#define TEST_CCP_UTIL_H_

#include "../../TlmCmd/common_cmd_packet.h"

CCP_CmdRet Cmd_TEST_CCP_REGISTER_TLC_ASAP(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TEST_CCP_GET_RAW_PARAM_INFO(const CommonCmdPacket* packet);

#endif
