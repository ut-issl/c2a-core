#pragma section REPRO
/**
 * @file
 * @brief  CCP の汎用 Utility
 */

#include "common_cmd_packet_util.h"
#include "command_analyze.h"
#include "../Library/endian_memcpy.h"
#include <stddef.h>     // for NULL
#include <string.h>

static CommonCmdPacket* CCP_util_packet_;

/**
 * @brief NOP cmd の RTC CCP を作る
 * @param[in,out] packet: CCP
 * @return void
 */
void CCP_form_nop_rtc_(CommonCmdPacket* packet);

/**
 * @brief CCP パラメタ開始位置に対する n 番目の引数の offset を計算する
 * @param[in]  cmd_id: CMD_CODE
 * @param[in]  n: N番目の引数 （0起算）
 * @param[out] offset: offset
 * @return CCP_UTIL_ACK
 */
CCP_UTIL_ACK CCP_calc_param_offset_(CMD_CODE cmd_id, uint8_t n, uint16_t* offset);


void CCP_form_nop_rtc_(CommonCmdPacket* packet)
{
  CCP_form_rtc(packet, Cmd_CODE_NOP, NULL, 0);
}

void CCP_form_app_cmd(CommonCmdPacket* packet, cycle_t ti, AR_APP_ID id)
{
  // FIXME: この4は環境依存なので，依存しないように直す
  //        適切に直すことで， CCP_form_tlc の返り値をみなくて良くなるはず．
  //        Cmd_AM_EXECUTE_APP の引数取得部分と同時に直すべきだが，パラメタサイズは CmdDB から取得可能なはず．
  uint8_t param[4];
  size_t  id_temp = id;
  endian_memcpy(param, &id_temp, 4);

  CCP_form_tlc(packet, ti, CCP_EXEC_TYPE_TL_FROM_GS, Cmd_CODE_AM_EXECUTE_APP, param, 4);
}

CCP_UTIL_ACK CCP_form_rtc(CommonCmdPacket* packet, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (param == NULL && len != 0)
  {
    CCP_form_nop_rtc_(packet);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if (CA_ckeck_cmd_param_len(cmd_id, len) != CA_ACK_OK)
  {
    CCP_form_nop_rtc_(packet);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  CCP_set_common_hdr(packet);
  CCP_set_id(packet, cmd_id);
  CCP_set_exec_type(packet, CCP_EXEC_TYPE_RT);
  CCP_set_dest_type(packet, CCP_DEST_TYPE_TO_ME);
  CCP_set_ti(packet, 0); // RTの場合、TIは0固定。
  CCP_set_param(packet, param, len);

  return CCP_UTIL_ACK_OK;
}

CCP_UTIL_ACK CCP_form_tlc(CommonCmdPacket* packet, cycle_t ti, CCP_EXEC_TYPE type, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (param == NULL && len != 0)
  {
    CCP_form_nop_rtc_(packet);
    CCP_convert_rtc_to_tlc(packet, ti);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if (CA_ckeck_cmd_param_len(cmd_id, len) != CA_ACK_OK)
  {
    CCP_form_nop_rtc_(packet);
    CCP_convert_rtc_to_tlc(packet, ti);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  CCP_set_common_hdr(packet);
  CCP_set_id(packet, cmd_id);
  CCP_set_exec_type(packet, type);
  CCP_set_dest_type(packet, CCP_DEST_TYPE_TO_ME);
  CCP_set_ti(packet, ti);
  CCP_set_param(packet, param, len);

  return CCP_UTIL_ACK_OK;
}

CCP_UTIL_ACK CCP_form_block_deploy_cmd(CommonCmdPacket* packet, TLCD_ID tl_no, bct_id_t block_no)
{
  uint8_t param[1 + SIZE_OF_BCT_ID_T];

  if ((tl_no >= TLCD_ID_MAX) || (block_no >= BCT_MAX_BLOCKS))
  {
    // タイムラインのline番号、ブロックコマンド番号が範囲外の場合異常判定
    CCP_form_nop_rtc_(packet);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  param[0] = (uint8_t)tl_no;
  endian_memcpy(&param[1], &block_no, SIZE_OF_BCT_ID_T);

  return CCP_form_rtc(packet, Cmd_CODE_TLCD_DEPLOY_BLOCK, param, 1 + SIZE_OF_BCT_ID_T);
}

void CCP_convert_rtc_to_tlc(CommonCmdPacket* packet, cycle_t ti)
{
  CCP_set_exec_type(packet, CCP_EXEC_TYPE_TL_FROM_GS);
  CCP_set_ti(packet, ti);
}

PH_ACK CCP_register_app_cmd(cycle_t ti, AR_APP_ID id)
{
  CCP_form_app_cmd(CCP_util_packet_, ti, id);
  return PH_analyze_cmd_packet(CCP_util_packet_);
}

PH_ACK CCP_register_rtc(CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (CCP_form_rtc(CCP_util_packet_, cmd_id, param, len) != CCP_UTIL_ACK_OK)
  {
    return PH_ACK_INVALID_PACKET;
  }

  return PH_analyze_cmd_packet(CCP_util_packet_);
}

PH_ACK CCP_register_tlc(cycle_t ti, CCP_EXEC_TYPE type, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (CCP_form_tlc(CCP_util_packet_, ti, type, cmd_id, param, len) != CCP_UTIL_ACK_OK)
  {
    return PH_ACK_INVALID_PACKET;
  }

  return PH_analyze_cmd_packet(CCP_util_packet_);
}

PH_ACK CCP_register_block_deploy_cmd(TLCD_ID tl_no, bct_id_t block_no)
{
  if (CCP_form_block_deploy_cmd(CCP_util_packet_, tl_no, block_no) != CCP_UTIL_ACK_OK)
  {
    return PH_ACK_INVALID_PACKET;
  }

  return PH_analyze_cmd_packet(CCP_util_packet_);
}

const PacketList* CCP_get_packet_list_from_exec_type(CCP_EXEC_TYPE type)
{
  switch (type)
  {
  case CCP_EXEC_TYPE_GS:
    return &PH_gs_cmd_list;

  case CCP_EXEC_TYPE_TL_FROM_GS:
    return &PH_tl_cmd_list[TLCD_ID_FROM_GS];

  case CCP_EXEC_TYPE_RT:
    return &PH_rt_cmd_list;

  case CCP_EXEC_TYPE_UTL:
    return &PH_tl_cmd_list[TLCD_ID_FROM_GS];

  case CCP_EXEC_TYPE_TL_DEPLOY_BC:
    return &PH_tl_cmd_list[TLCD_ID_DEPLOY_BC];

  case CCP_EXEC_TYPE_TL_DEPLOY_TLM:
    return &PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM];

#ifdef TLCD_ENABLE_MISSION_TL
  case CCP_EXEC_TYPE_TL_FOR_MISSION:
    return &PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION];

  case CCP_EXEC_TYPE_UTL_FOR_MISSION:
    return &PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION];
#endif

  default:
    break;
  }

  return NULL;
}

uint8_t* CCP_get_1byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n)
{
  static uint8_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 1;
  CCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}

uint16_t* CCP_get_2byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n)
{
  static uint16_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 2;
  CCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}

uint32_t* CCP_get_4byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n)
{
  static uint32_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 4;
  CCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}

uint64_t* CCP_get_8byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n)
{
  static uint64_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 8;
  CCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}

uint16_t CCP_get_raw_param_from_packet(const CommonCmdPacket* packet, void* dest, uint16_t max_copy_len)
{
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  CCP_UTIL_ACK ack;
  int32_t copy_len;

  if (!CA_has_raw_param(cmd_id)) return 0;

  ack = CCP_calc_param_offset_(cmd_id, (uint8_t)(CA_get_cmd_param_num(cmd_id) - 1), &offset);
  if (ack != CCP_UTIL_ACK_OK) return 0;

  copy_len = CCP_get_param_len(packet) - offset;
  if (copy_len < 0) return 0;
  if (max_copy_len != 0 && copy_len > max_copy_len)
  {
    copy_len = max_copy_len;
  }

  memcpy(dest,  CCP_get_param_head(packet) + offset, (size_t)copy_len);
  return (uint16_t)copy_len;
}

CCP_UTIL_ACK CCP_calc_param_offset_(CMD_CODE cmd_id, uint8_t n, uint16_t* offset)
{
  uint8_t i;
  if (cmd_id >= CA_MAX_CMDS) return CCP_UTIL_ACK_PARAM_ERR;
  if (CA_get_cmd_param_num(cmd_id) <= n) return CCP_UTIL_ACK_PARAM_ERR;

  *offset = 0;
  for (i = 0; i < n; ++i)
  {
    (*offset) += CA_get_cmd_param_size(cmd_id, i);
  }
  return CCP_UTIL_ACK_OK;
}

#pragma section
