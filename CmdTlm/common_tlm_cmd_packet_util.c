#pragma section REPRO
/**
 * @file   common_tlm_cmd_packet_util.c
 * @brief  CTCP の汎用Util
 */

#include "common_tlm_cmd_packet_util.h"
#include "../Applications/timeline_command_dispatcher.h" // for TL_ID_MAX
#include "command_analyze.h"
#include "block_command_table.h" // for BCT_MAX_BLOCKS
#include <src_user/CmdTlm/command_definitions.h>  // for CMD_CODE
#include "../Library/endian_memcpy.h"
#include <stddef.h>     // for NULL
#include <string.h>

/**
 * @brief NOP cmd の RTC CTCP を作る
 * @param[in,out] packet: CTCP
 * @return void
 */
void CCP_form_nop_rtc_cmd_(CTCP* packet);

/**
 * @brief CTCP パラメタ開始位置に対する n 番目の引数の offset を計算する
 * @param[in]  cmd_id: CMD_CODE
 * @param[in]  n: N番目の引数 （0起算）
 * @param[out] offset: offset
 * @return CTCP_UTIL_ACK
 */
CTCP_UTIL_ACK CCP_calc_param_offset_(CMD_CODE cmd_id, uint8_t n, uint16_t* offset);


void CCP_form_nop_rtc_cmd_(CTCP* packet)
{
  CCP_form_rtc(packet, Cmd_CODE_NOP, NULL, 0);
}

void CCP_form_app_cmd(CTCP* packet, cycle_t ti, AR_APP_ID id)
{
  // FIXME: この4は環境依存なので，依存しないように直す
  //        適切に直すことで， CCP_form_tlc の返り値をみなくて良くなるはず．
  //        Cmd_AM_EXECUTE_APP の引数取得部分と同時に直すべきだが，パラメタサイズは CmdDBから取得可能なはず．
  uint8_t param[4];
  size_t  id_temp = id;
  endian_memcpy(param, &id_temp, 4);

  CCP_form_tlc(packet, ti, Cmd_CODE_AM_EXECUTE_APP, param, 4);
}

CTCP_UTIL_ACK CCP_form_rtc(CTCP* packet, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (param == NULL && len != 0)
  {
    CCP_form_nop_rtc_cmd_(packet);
    return CTCP_UTIL_ACK_PARAM_ERR;
  }

  if (CA_ckeck_cmd_param_len(cmd_id, len) != CA_ACK_OK)
  {
    CCP_form_nop_rtc_cmd_(packet);
    return CTCP_UTIL_ACK_PARAM_ERR;
  }

  CCP_set_common_hdr(packet);
  CCP_set_id(packet, cmd_id);
  CCP_set_exec_type(packet, CCP_EXEC_TYPE_RT);
  CCP_set_dest_type(packet, TCP_CMD_DEST_TYPE_TO_ME);
  CCP_set_ti(packet, 0); // RTの場合、TIは0固定。
  CCP_set_param(packet, param, len);

  return CTCP_UTIL_ACK_OK;
}

CTCP_UTIL_ACK CCP_form_tlc(CTCP* packet, cycle_t ti, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (param == NULL && len != 0)
  {
    CCP_form_nop_rtc_cmd_(packet);
    CCP_convert_rtc_to_tlc(packet, ti);
    return CTCP_UTIL_ACK_PARAM_ERR;
  }

  if (CA_ckeck_cmd_param_len(cmd_id, len) != CA_ACK_OK)
  {
    CCP_form_nop_rtc_cmd_(packet);
    CCP_convert_rtc_to_tlc(packet, ti);
    return CTCP_UTIL_ACK_PARAM_ERR;
  }

  CCP_set_common_hdr(packet);
  CCP_set_id(packet, cmd_id);
  CCP_set_exec_type(packet, CCP_EXEC_TYPE_TL0);
  CCP_set_dest_type(packet, TCP_CMD_DEST_TYPE_TO_ME);
  CCP_set_ti(packet, ti);
  CCP_set_param(packet, param, len);

  return CTCP_UTIL_ACK_OK;
}

CTCP_UTIL_ACK CCP_form_block_deploy_cmd(CTCP* packet, uint8_t tl_no, bct_id_t block_no)
{
  uint8_t param[1 + SIZE_OF_BCT_ID_T];

  if ((tl_no >= TL_ID_MAX) || (block_no >= BCT_MAX_BLOCKS))
  {
    // タイムラインのline番号、ブロックコマンド番号が範囲外の場合異常判定
    CCP_form_nop_rtc_cmd_(packet);
    return CTCP_UTIL_ACK_PARAM_ERR;
  }

  param[0] = tl_no;
  endian_memcpy(&param[1], &block_no, SIZE_OF_BCT_ID_T);

  return CCP_form_rtc(packet, Cmd_CODE_TLCD_DEPLOY_BLOCK, param, 1 + SIZE_OF_BCT_ID_T);
}

void CCP_convert_rtc_to_tlc(CTCP* packet, cycle_t ti)
{
  CCP_set_exec_type(packet, CCP_EXEC_TYPE_TL0);
  CCP_set_ti(packet, ti);
}

uint8_t* CCP_get_1byte_param_from_packet(const CTCP* packet, uint8_t n)
{
  static uint8_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 1;
  CTCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CTCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}

uint16_t* CCP_get_2byte_param_from_packet(const CTCP* packet, uint8_t n)
{
  static uint16_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 2;
  CTCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CTCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}

uint32_t* CCP_get_4byte_param_from_packet(const CTCP* packet, uint8_t n)
{
  static uint32_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 4;
  CTCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CTCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}

uint64_t* CCP_get_8byte_param_from_packet(const CTCP* packet, uint8_t n)
{
  static uint64_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 8;
  CTCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CTCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}

uint16_t CCP_get_raw_param_from_packet(const CTCP* packet, void* dest, uint16_t max_copy_len)
{
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  CTCP_UTIL_ACK ack;
  int32_t copy_len;

  if (!CA_has_raw_param(cmd_id)) return 0;

  ack = CCP_calc_param_offset_(cmd_id, (uint8_t)(CA_get_cmd_param_num(cmd_id) - 1), &offset);
  if (ack != CTCP_UTIL_ACK_OK) return 0;

  copy_len = CCP_get_param_len(packet) - offset;
  if (copy_len < 0) return 0;
  if (max_copy_len != 0 && copy_len > max_copy_len)
  {
    copy_len = max_copy_len;
  }

  memcpy(dest,  CCP_get_param_head(packet) + offset, (size_t)copy_len);
  return (uint16_t)copy_len;
}

CTCP_UTIL_ACK CCP_calc_param_offset_(CMD_CODE cmd_id, uint8_t n, uint16_t* offset)
{
  uint8_t i;
  if (cmd_id >= CA_MAX_CMDS) return CTCP_UTIL_ACK_PARAM_ERR;
  if (CA_get_cmd_param_num(cmd_id) <= n) return CTCP_UTIL_ACK_PARAM_ERR;

  *offset = 0;
  for (i = 0; i < n; ++i)
  {
    (*offset) += CA_get_cmd_param_size(cmd_id, i);
  }
  return CTCP_UTIL_ACK_OK;
}

#pragma section
