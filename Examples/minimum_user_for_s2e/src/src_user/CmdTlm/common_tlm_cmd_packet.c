#pragma section REPRO
/**
 * @file
 * @brief  CTCP の定義
 * @note   CTCP: common_tlm_cmd_packet
 * @note   CCP:  CommonCmdPacket
 * @note   CTP:  CommonTlmPacket
 */

#include <string.h> // for memcpy
#include "./Ccsds/TCPacket.h"
#include <src_core/CmdTlm/common_tlm_cmd_packet.h>
#include <src_core/Library/endian_memcpy.h>

// ********************
//  CTCP getter/setter
// ********************

CTCP_TC_DSC CTCP_get_tc_dsc(const CTCP* packet)
{
  switch (TCP_get_type(packet))
  {
  case TCP_TYPE_TLM:
    return CTCP_TC_DSC_TLM;

  case TCP_TYPE_CMD:
    return CTCP_TC_DSC_CMD;

  default:
    return CTCP_TC_DSC_UNKNOWN;
  }
}

void CTCP_copy_packet(CTCP* dest, const CTCP* src)
{
  TCP_copy_packet(dest, src);
}


// *******************
//  CCP getter/setter
// *******************

cycle_t CCP_get_ti(const CTCP* packet)
{
  return (cycle_t)TCP_CMD_get_ti(packet);
}

void CCP_set_ti(CTCP* packet, cycle_t ti)
{
  TCP_CMD_set_ti(packet, (uint32_t)ti);
}

CTCP_DEST_ID CCP_get_dest_id(const CTCP* packet)
{
  return (CTCP_DEST_ID)TCP_get_apid(packet);
}

void CCP_set_dest_id(CTCP* packet, CTCP_DEST_ID id)
{
  TCP_set_apid(packet, (TCP_APID)id);
}

CCP_EXEC_TYPE CCP_get_exec_type(const CTCP* packet)
{
  // TCP_CMD_EXEC_TYPEとCCP_EXEC_TYPEの定義は合わせている．(正確には、前者は後者に含まれている．)
  TCP_CMD_EXEC_TYPE exec_type = TCP_CMD_get_exec_type(packet);
  if (exec_type == TCP_CMD_EXEC_TYPE_UNKNOWN)
  {
    return CCP_EXEC_TYPE_UNKNOWN;
  }
  else
  {
    return (CCP_EXEC_TYPE)exec_type;
  }
}

void CCP_set_exec_type(CTCP* packet, CCP_EXEC_TYPE type)
{
  // CCP_EXEC_TYPEのほうがとりうる数値の幅が広いので，アサーション
  if ((int)type >= (int)TCP_CMD_EXEC_TYPE_UNKNOWN)
  {
    TCP_CMD_set_exec_type(packet, TCP_CMD_EXEC_TYPE_UNKNOWN);
  }
  else
  {
    TCP_CMD_set_exec_type(packet, (TCP_CMD_EXEC_TYPE)type);
  }
}

// 2021-01-22 追加
CCP_DEST_TYPE CCP_get_dest_type(const CTCP* packet)
{
  return (CCP_DEST_TYPE)TCP_CMD_get_dest_type(packet);
}

// 2021-01-22 追加
void CCP_set_dest_type(CTCP* packet, CCP_DEST_TYPE type)
{
  TCP_CMD_set_dest_type(packet, (TCP_CMD_DEST_TYPE)type);
}

CMD_CODE CCP_get_id(const CTCP* packet)
{
  return (CMD_CODE)TCP_CMD_get_channel_id(packet);
}

void CCP_set_id(CTCP* packet, CMD_CODE id)
{
  TCP_CMD_set_channel_id(packet, (uint16_t)id);
}

uint16_t CCP_get_packet_len(const CTCP* packet)
{
  return (uint16_t)(TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN);
}

uint16_t CCP_get_param_len(const CTCP* packet)
{
  // ユーザーデータ長からヘッダ長を除いたものがパラメータ長
  return (uint16_t)(TCP_get_packet_len(packet) - TCP_CMD_2ND_HDR_LEN - TCP_CMD_USER_HDR_LEN);
}

uint16_t CCP_get_max_param_len(void)
{
  return (uint16_t)(TCP_MAX_LEN - TCP_PRM_HDR_LEN - TCP_CMD_2ND_HDR_LEN - TCP_CMD_USER_HDR_LEN);
}

const uint8_t* CCP_get_param_head(const CTCP* packet)
{
  return TCP_CMD_get_param_head(packet);
}

void CCP_set_param(CTCP* packet, const uint8_t* param, uint16_t len)
{
  TCP_CMD_set_param(packet, param, len);
}

void CCP_set_common_hdr(CTCP* packet)
{
  TCP_CMD_set_common_hdr(packet);
}

// *******************
//  CTP getter/setter
// *******************

CTP_DEST_FLAG CTP_get_dest_flag(const CTCP* packet)
{
  uint8_t category = TCP_TLM_get_category(packet);
  uint8_t mask = 0xe0; // 11100000b

  category &= mask;
  category >>= 5;

  if (category == 0)
  {
    return CTP_DEST_FLAG_RP;
  }
  else
  {
    return (CTP_DEST_FLAG)category;
  }
}

void CTP_set_dest_flag(CTCP* packet, CTP_DEST_FLAG flag)
{
  uint8_t category = TCP_TLM_get_category(packet);
  uint8_t mask = 0xe0; // 11100000b

  category &= (uint8_t)(~mask);
  category |= (uint8_t)(flag << 5);

  TCP_TLM_set_category(packet, category);
}

uint8_t* CTP_get_user_data_head(CTCP* packet)
{
  return TCP_TLM_get_user_data_head(packet);
}
#pragma section
