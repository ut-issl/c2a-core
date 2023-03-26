#pragma section REPRO
/**
 * @file
 * @brief CCSDS で規定される Space Packet の コマンド版の実装
 * @brief C ではテンプレートが使えないため，別で定義する
 * @note  詳細は space_packet.h を参照
 */
#include "cmd_space_packet.h"
#include <stddef.h>
#include <string.h>

#define CSP_CAST_TO_SP(csp)           ((const SpacePacket*)(csp))   //!< SPへのキャスト． SPのほうが広いので，これは可能
#define CSP_CAST_TO_NON_CONST_SP(csp) ((SpacePacket*)(csp))         //!< SPへのキャスト． SPのほうが広いので，これは可能


static const SP_ParamExtractionInfo CSP_pei_2nd_hdr_ver_ = { 6, 0xff, 0, 1};  // 11111111b
static const SP_ParamExtractionInfo CSP_pei_cmd_type_    = { 7, 0xff, 0, 1};  // 11111111b
static const SP_ParamExtractionInfo CSP_pei_cmd_id_      = { 8, 0xff, 0, 2};  // 11111111b
static const SP_ParamExtractionInfo CSP_pei_dest_type_   = {10, 0xf0, 4, 1};  // 11110000b
static const SP_ParamExtractionInfo CSP_pei_exec_type_   = {10, 0x0f, 0, 1};  // 00001111b
static const SP_ParamExtractionInfo CSP_pei_ti_          = {11, 0xff, 0, 4};  // 11111111b


// ******************************
//  Primary Header getter/setter
// ******************************

SP_VER CSP_get_ver(const CmdSpacePacket* csp)
{
  return SP_get_ver(CSP_CAST_TO_SP(csp));
}


void CSP_set_ver(CmdSpacePacket* csp, SP_VER ver)
{
  SP_set_ver(CSP_CAST_TO_NON_CONST_SP(csp), ver);
}


SP_TYPE CSP_get_type(const CmdSpacePacket* csp)
{
  return SP_get_type(CSP_CAST_TO_SP(csp));
}


void CSP_set_type(CmdSpacePacket* csp, SP_TYPE type)
{
  SP_set_type(CSP_CAST_TO_NON_CONST_SP(csp), type);
}


SP_2ND_HDR_FLAG CSP_get_2nd_hdr_flag(const CmdSpacePacket* csp)
{
  return SP_get_2nd_hdr_flag(CSP_CAST_TO_SP(csp));
}


void CSP_set_2nd_hdr_flag(CmdSpacePacket* csp, SP_2ND_HDR_FLAG flag)
{
  SP_set_2nd_hdr_flag(CSP_CAST_TO_NON_CONST_SP(csp), flag);
}


APID CSP_get_apid(const CmdSpacePacket* csp)
{
  return SP_get_apid(CSP_CAST_TO_SP(csp));
}


void CSP_set_apid(CmdSpacePacket* csp, APID apid)
{
  SP_set_apid(CSP_CAST_TO_NON_CONST_SP(csp), apid);
}


SP_SEQ_FLAG CSP_get_seq_flag(const CmdSpacePacket* csp)
{
  return SP_get_seq_flag(CSP_CAST_TO_SP(csp));
}


void CSP_set_seq_flag(CmdSpacePacket* csp, SP_SEQ_FLAG flag)
{
  SP_set_seq_flag(CSP_CAST_TO_NON_CONST_SP(csp), flag);
}


uint16_t CSP_get_seq_count(const CmdSpacePacket* csp)
{
  return SP_get_seq_count(CSP_CAST_TO_SP(csp));
}


void CSP_set_seq_count(CmdSpacePacket* csp, uint16_t count)
{
  SP_set_seq_count(CSP_CAST_TO_NON_CONST_SP(csp), count);
}


uint16_t CSP_get_packet_data_len(const CmdSpacePacket* csp)
{
  return SP_get_packet_data_len(CSP_CAST_TO_SP(csp));
}


void CSP_set_packet_data_len(CmdSpacePacket* csp, uint16_t len)
{
  SP_set_packet_data_len(CSP_CAST_TO_NON_CONST_SP(csp), len);
}


// **************************
//  2nd Header getter/setter
// **************************

CSP_2ND_HDR_VER CSP_get_2nd_hdr_ver(const CmdSpacePacket* csp)
{
  uint8_t tmp;
  CSP_2ND_HDR_VER ver;

  SP_extract_param_from_packet(CSP_CAST_TO_SP(csp), &CSP_pei_2nd_hdr_ver_, &tmp);
  ver = (CSP_2ND_HDR_VER)tmp;

  switch (ver)
  {
  case CSP_2ND_HDR_VER_1:
    return ver;
  default:
    return CSP_2ND_HDR_VER_UNKNOWN;
  }
}


void CSP_set_2nd_hdr_ver(CmdSpacePacket* csp, CSP_2ND_HDR_VER ver)
{
  uint8_t tmp = (uint8_t)ver;
  SP_insert_param_to_packet(CSP_CAST_TO_NON_CONST_SP(csp), &CSP_pei_2nd_hdr_ver_, &tmp);
}


CSP_CMD_TYPE CSP_get_cmd_type(const CmdSpacePacket* csp)
{
  (void)csp;
  // ひとまず
  return CSP_CMD_TYPE_UNKNOWN;
}


void CSP_set_cmd_type(CmdSpacePacket* csp, CSP_CMD_TYPE cmd_type)
{
  uint8_t tmp = (uint8_t)cmd_type;
  SP_insert_param_to_packet(CSP_CAST_TO_NON_CONST_SP(csp), &CSP_pei_cmd_type_, &tmp);
}


CMD_CODE CSP_get_cmd_id(const CmdSpacePacket* csp)
{
  uint16_t id;

  SP_extract_param_from_packet(CSP_CAST_TO_SP(csp), &CSP_pei_cmd_id_, &id);
  return (CMD_CODE)id;
}


void CSP_set_cmd_id(CmdSpacePacket* csp, CMD_CODE id)
{
  uint16_t tmp = (uint16_t)id;
  SP_insert_param_to_packet(CSP_CAST_TO_NON_CONST_SP(csp), &CSP_pei_cmd_id_, &tmp);
}


CCP_DEST_TYPE CSP_get_dest_type(const CmdSpacePacket* csp)
{
  uint8_t dest_type;
  SP_extract_param_from_packet(CSP_CAST_TO_SP(csp), &CSP_pei_dest_type_, &dest_type);
  return CCP_get_dest_type_from_uint8(dest_type);
}


void CSP_set_dest_type(CmdSpacePacket* csp, CCP_DEST_TYPE dest_type)
{
  uint8_t tmp = (uint8_t)dest_type;
  SP_insert_param_to_packet(CSP_CAST_TO_NON_CONST_SP(csp), &CSP_pei_dest_type_, &tmp);
}


CCP_EXEC_TYPE CSP_get_exec_type(const CmdSpacePacket* csp)
{
  uint8_t tmp;
  CCP_EXEC_TYPE exec_type;

  SP_extract_param_from_packet(CSP_CAST_TO_SP(csp), &CSP_pei_exec_type_, &tmp);
  exec_type = (CCP_EXEC_TYPE)tmp;

  switch (exec_type)
  {
  case CCP_EXEC_TYPE_GS:
  case CCP_EXEC_TYPE_TL_FROM_GS:
  case CCP_EXEC_TYPE_BC:
  case CCP_EXEC_TYPE_RT:
  case CCP_EXEC_TYPE_UTL:
  case CCP_EXEC_TYPE_TL_DEPLOY_BC:
  case CCP_EXEC_TYPE_TL_DEPLOY_TLM:
#ifdef TLCD_ENABLE_MISSION_TL
  case CCP_EXEC_TYPE_TL_FOR_MISSION:
  case CCP_EXEC_TYPE_UTL_FOR_MISSION:
#endif
    return exec_type;  // ここまで FALL THROUGH
  default:
    return CCP_EXEC_TYPE_UNKNOWN;
  }
}


void CSP_set_exec_type(CmdSpacePacket* csp, CCP_EXEC_TYPE exec_type)
{
  uint8_t tmp = (uint8_t)exec_type;
  SP_insert_param_to_packet(CSP_CAST_TO_NON_CONST_SP(csp), &CSP_pei_exec_type_, &tmp);
}


uint32_t CSP_get_ti(const CmdSpacePacket* csp)
{
  uint32_t ti;

  SP_extract_param_from_packet(CSP_CAST_TO_SP(csp), &CSP_pei_ti_, &ti);
  return ti;
}


void CSP_set_ti(CmdSpacePacket* csp, uint32_t ti)
{
  SP_insert_param_to_packet(CSP_CAST_TO_NON_CONST_SP(csp), &CSP_pei_ti_, &ti);
}


// ******************
//  Utility Function
// ******************

uint16_t CSP_get_packet_len(const CmdSpacePacket* csp)
{
  return SP_get_packet_len(CSP_CAST_TO_SP(csp));
}


void CSP_set_packet_len(CmdSpacePacket* csp, uint16_t len)
{
  SP_set_packet_len(CSP_CAST_TO_NON_CONST_SP(csp), len);
}


void CSP_copy_packet(CmdSpacePacket* dest, const CmdSpacePacket* src)
{
  SP_copy_packet(CSP_CAST_TO_NON_CONST_SP(dest), CSP_CAST_TO_SP(src));
}


const uint8_t* CSP_get_param_head(const CmdSpacePacket* csp)
{
  return &(csp->packet[SP_PRM_HDR_LEN + CSP_SND_HDR_LEN]);
}


uint16_t CSP_get_param_len(const CmdSpacePacket* csp)
{
  return (uint16_t)(CSP_get_packet_len(csp) - SP_PRM_HDR_LEN - CSP_SND_HDR_LEN);
}


void CSP_set_param(CmdSpacePacket* csp, const uint8_t* param, uint16_t len)
{
  // パラメータをパケットにコピー
  // paramは既に成形されているのでここは ENDIAN_memcpy ではなく memcpy (paramの数, 種類などがこの関数からは不明)
  // const ではダメなので， CSP_get_param_head が使えない
  memcpy(&(csp->packet[SP_PRM_HDR_LEN + CSP_SND_HDR_LEN]), param, (size_t)len);

  // パラメータ長に合わせパケット長を設定
  // パケット長はヘッダ分が加算される
  CSP_set_packet_len(csp, (uint16_t)(SP_PRM_HDR_LEN + CSP_SND_HDR_LEN + len));
}


void CSP_set_common_hdr(CmdSpacePacket* csp)
{
  // ここでは Version Number は Version-1 固定
  CSP_set_ver(csp, SP_VER_1);
  // Command Packet なので Type は Telecommand 固定
  CSP_set_type(csp, SP_TYPE_CMD);
  // ここでは Secondary Header は必須
  CSP_set_2nd_hdr_flag(csp, SP_2ND_HDR_FLAG_PRESENT);
  // APID
  CSP_set_apid(csp, CCP_APID_TO_ME);
  // ここでは Sequence Flag は Standalone Packet に固定
  CSP_set_seq_flag(csp, SP_SEQ_FLAG_SINGLE);
  // ここでは Sequence Count は 0 固定
  CSP_set_seq_count(csp, 0);
  // ここでは Sec. HDR Ver は CSP_2ND_HDR_VER_TO_USE 固定
  CSP_set_2nd_hdr_ver(csp, CSP_2ND_HDR_VER_TO_USE);
  // ここでは Command Type は CSP_CMD_TYPE_UNKNOWN 固定
  CSP_set_cmd_type(csp, CSP_CMD_TYPE_UNKNOWN);
}

#pragma section
