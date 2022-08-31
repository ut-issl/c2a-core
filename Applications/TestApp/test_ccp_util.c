#pragma section REPRO
/**
 * @file
 * @brief CCP 汎用 utility テスト用 app
 */

#include "test_ccp_util.h"
#include "../../TlmCmd/common_cmd_packet_util.h"
#include <src_user/TlmCmd/command_definitions.h>

CCP_CmdRet Cmd_TEST_CCP_REGISTER_TLC_ASAP(const CommonCmdPacket* packet)
{
  cycle_t ti = CCP_get_param_from_packet(packet, 0, cycle_t);
  PH_ACK ack = CCP_register_tlc_asap(ti, TLCD_ID_FROM_GS, Cmd_CODE_NOP, NULL, 0);
  if (ack != PH_ACK_TLC_SUCCESS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_TEST_CCP_GET_RAW_PARAM_INFO(const CommonCmdPacket* packet)
{
  uint16_t len = CCP_get_param_from_packet(packet, 0, uint16_t);
  uint8_t first_data = CCP_get_param_from_packet(packet, 1, uint8_t);
  const uint8_t* data;

  // CCP_get_raw_param_head の test
  if (CCP_get_raw_param_head(packet) != CCP_get_param_head(packet) + sizeof(uint16_t) + sizeof(uint8_t))
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  // len チェック
  if (CCP_get_raw_param_len(packet) != len)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  if (len == 0)
  {
    // raw param なし
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  }

  data = CCP_get_raw_param_head(packet);
  if (data[0] != first_data)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
