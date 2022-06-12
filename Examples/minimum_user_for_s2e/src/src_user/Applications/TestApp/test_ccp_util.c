#pragma section REPRO
/**
 * @file
 * @brief CCP 汎用 utility テスト用 app
 */

#include "test_ccp_util.h"
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../TlmCmd/command_definitions.h"

CCP_EXEC_STS Cmd_TEST_CCP_REGISTER_TLC_ASAP(const CommonCmdPacket* packet)
{
  cycle_t ti = CCP_get_param_from_packet(packet, 0, cycle_t);
  PH_ACK ack = CCP_register_tlc_asap(ti, TLCD_ID_FROM_GS, Cmd_CODE_NOP, NULL, 0);
  if (ack != PH_ACK_TLC_SUCCESS) return CCP_EXEC_ILLEGAL_CONTEXT;

  return CCP_EXEC_SUCCESS;
}

#pragma section
