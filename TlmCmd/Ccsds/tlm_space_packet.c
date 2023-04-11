#pragma section REPRO
/**
 * @file
 * @brief CCSDS で規定される Space Packet の テレメ版の実装
 * @brief C ではテンプレートが使えないため，別で定義する
 * @note  詳細は space_packet.h を参照
 */
#include "tlm_space_packet.h"
#include <stddef.h>
#include <string.h>

#define TSP_CAST_TO_SP(tsp)           ((const SpacePacket*)(tsp))   //!< SPへのキャスト． SPのほうが広いので，これは可能
#define TSP_CAST_TO_NON_CONST_SP(tsp) ((SpacePacket*)(tsp))         //!< SPへのキャスト． SPのほうが広いので，これは可能


static const SP_ParamExtractionInfo TSP_pei_2nd_hdr_ver_ = { 6, 0xff, 0, 1};  // 11111111b
static const SP_ParamExtractionInfo TSP_pei_board_time_  = { 7, 0xff, 0, 4};  // 11111111b
static const SP_ParamExtractionInfo TSP_pei_tlm_id_      = {11, 0xff, 0, 1};  // 11111111b
static const SP_ParamExtractionInfo TSP_pei_global_time_ = {12, 0xff, 0, 8};  // 11111111b
static const SP_ParamExtractionInfo TSP_pei_ob_sn_time_  = {20, 0xff, 0, 4};  // 11111111b
static const SP_ParamExtractionInfo TSP_pei_dest_flags_  = {24, 0xff, 0, 1};  // 11111111b
static const SP_ParamExtractionInfo TSP_pei_dest_info_   = {25, 0x0f, 0, 1};  // 11111111b


// ******************************
//  Primary Header getter/setter
// ******************************

SP_VER TSP_get_ver(const TlmSpacePacket* tsp)
{
  return SP_get_ver(TSP_CAST_TO_SP(tsp));
}


void TSP_set_ver(TlmSpacePacket* tsp, SP_VER ver)
{
  SP_set_ver(TSP_CAST_TO_NON_CONST_SP(tsp), ver);
}


SP_TYPE TSP_get_type(const TlmSpacePacket* tsp)
{
  return SP_get_type(TSP_CAST_TO_SP(tsp));
}


void TSP_set_type(TlmSpacePacket* tsp, SP_TYPE type)
{
  SP_set_type(TSP_CAST_TO_NON_CONST_SP(tsp), type);
}


SP_2ND_HDR_FLAG TSP_get_2nd_hdr_flag(const TlmSpacePacket* tsp)
{
  return SP_get_2nd_hdr_flag(TSP_CAST_TO_SP(tsp));
}


void TSP_set_2nd_hdr_flag(TlmSpacePacket* tsp, SP_2ND_HDR_FLAG flag)
{
  SP_set_2nd_hdr_flag(TSP_CAST_TO_NON_CONST_SP(tsp), flag);
}


APID TSP_get_apid(const TlmSpacePacket* tsp)
{
  return SP_get_apid(TSP_CAST_TO_SP(tsp));
}


void TSP_set_apid(TlmSpacePacket* tsp, APID apid)
{
  SP_set_apid(TSP_CAST_TO_NON_CONST_SP(tsp), apid);
}


SP_SEQ_FLAG TSP_get_seq_flag(const TlmSpacePacket* tsp)
{
  return SP_get_seq_flag(TSP_CAST_TO_SP(tsp));
}


void TSP_set_seq_flag(TlmSpacePacket* tsp, SP_SEQ_FLAG flag)
{
  SP_set_seq_flag(TSP_CAST_TO_NON_CONST_SP(tsp), flag);
}


uint16_t TSP_get_seq_count(const TlmSpacePacket* tsp)
{
  return SP_get_seq_count(TSP_CAST_TO_SP(tsp));
}


void TSP_set_seq_count(TlmSpacePacket* tsp, uint16_t count)
{
  SP_set_seq_count(TSP_CAST_TO_NON_CONST_SP(tsp), count);
}


uint16_t TSP_get_packet_data_len(const TlmSpacePacket* tsp)
{
  return SP_get_packet_data_len(TSP_CAST_TO_SP(tsp));
}


void TSP_set_packet_data_len(TlmSpacePacket* tsp, uint16_t len)
{
  SP_set_packet_data_len(TSP_CAST_TO_NON_CONST_SP(tsp), len);
}


// **************************
//  2nd Header getter/setter
// **************************

TSP_2ND_HDR_VER TSP_get_2nd_hdr_ver(const TlmSpacePacket* tsp)
{
  uint8_t tmp;
  TSP_2ND_HDR_VER ver;

  SP_extract_param_from_packet(TSP_CAST_TO_SP(tsp), &TSP_pei_2nd_hdr_ver_, &tmp);
  ver = (TSP_2ND_HDR_VER)tmp;

  switch (ver)
  {
  case TSP_2ND_HDR_VER_1:
    return ver;
  default:
    return TSP_2ND_HDR_VER_UNKNOWN;
  }
}


void TSP_set_2nd_hdr_ver(TlmSpacePacket* tsp, TSP_2ND_HDR_VER ver)
{
  uint8_t tmp = (uint8_t)ver;
  SP_insert_param_to_packet(TSP_CAST_TO_NON_CONST_SP(tsp), &TSP_pei_2nd_hdr_ver_, &tmp);
}


uint32_t TSP_get_board_time(const TlmSpacePacket* tsp)
{
  uint32_t time;

  SP_extract_param_from_packet(TSP_CAST_TO_SP(tsp), &TSP_pei_board_time_, &time);
  return time;
}


void TSP_set_board_time(TlmSpacePacket* tsp, uint32_t time)
{
  SP_insert_param_to_packet(TSP_CAST_TO_NON_CONST_SP(tsp), &TSP_pei_board_time_, &time);
}


double TSP_get_global_time(const TlmSpacePacket* tsp)
{
  double time;

  SP_extract_param_from_packet(TSP_CAST_TO_SP(tsp), &TSP_pei_global_time_, &time);
  return time;
}


void TSP_set_global_time(TlmSpacePacket* tsp, double time)
{
  SP_insert_param_to_packet(TSP_CAST_TO_NON_CONST_SP(tsp), &TSP_pei_global_time_, &time);
}


uint32_t TSP_get_on_board_subnet_time(const TlmSpacePacket* tsp)
{
  uint32_t time;

  SP_extract_param_from_packet(TSP_CAST_TO_SP(tsp), &TSP_pei_ob_sn_time_, &time);
  return time;
}


void TSP_set_on_board_subnet_time(TlmSpacePacket* tsp, uint32_t time)
{
  SP_insert_param_to_packet(TSP_CAST_TO_NON_CONST_SP(tsp), &TSP_pei_ob_sn_time_, &time);
}


TLM_CODE TSP_get_tlm_id(const TlmSpacePacket* tsp)
{
  uint8_t id;

  SP_extract_param_from_packet(TSP_CAST_TO_SP(tsp), &TSP_pei_tlm_id_, &id);
  return (TLM_CODE)id;
}


void TSP_set_tlm_id(TlmSpacePacket* tsp, TLM_CODE id)
{
  uint8_t tmp = (uint8_t)id;
  SP_insert_param_to_packet(TSP_CAST_TO_NON_CONST_SP(tsp), &TSP_pei_tlm_id_, &tmp);
}


ctp_dest_flags_t TSP_get_dest_flags(const TlmSpacePacket* tsp)
{
  ctp_dest_flags_t flags;

  SP_extract_param_from_packet(TSP_CAST_TO_SP(tsp), &TSP_pei_dest_flags_, &flags);
  return flags;
}


void TSP_set_dest_flags(TlmSpacePacket* tsp, ctp_dest_flags_t flags)
{
  SP_insert_param_to_packet(TSP_CAST_TO_NON_CONST_SP(tsp), &TSP_pei_dest_flags_, &flags);
}


uint8_t TSP_get_dest_info(const TlmSpacePacket* tsp)
{
  uint8_t info;

  SP_extract_param_from_packet(TSP_CAST_TO_SP(tsp), &TSP_pei_dest_info_, &info);
  return info;
}


void TSP_set_dest_info(TlmSpacePacket* tsp, uint8_t info)
{
  SP_insert_param_to_packet(TSP_CAST_TO_NON_CONST_SP(tsp), &TSP_pei_dest_info_, &info);
}


// ******************
//  Utility Function
// ******************

uint16_t TSP_get_packet_len(const TlmSpacePacket* tsp)
{
  return SP_get_packet_len(TSP_CAST_TO_SP(tsp));
}


void TSP_set_packet_len(TlmSpacePacket* tsp, uint16_t len)
{
  SP_set_packet_len(TSP_CAST_TO_NON_CONST_SP(tsp), len);
}


void TSP_copy_packet(TlmSpacePacket* dest, const TlmSpacePacket* src)
{
  SP_copy_packet(TSP_CAST_TO_NON_CONST_SP(dest), TSP_CAST_TO_SP(src));
}


uint8_t* TSP_get_user_data_head(const TlmSpacePacket* tsp)
{
  return (uint8_t*)&(tsp->packet[SP_PRM_HDR_LEN + TSP_SND_HDR_LEN]);    // const_cast
}


// FIXME: TCP 時代の len と変わってることに注意！！！！！！
// これを呼ぶ関数でなおす！！！！
void TSP_setup_primary_hdr(TlmSpacePacket* tsp, APID apid, uint16_t seq_count, uint16_t packet_len)
{
  TSP_set_common_hdr(tsp);
  TSP_set_apid(tsp, apid);
  TSP_set_seq_count(tsp, seq_count);
  TSP_set_packet_len(tsp, packet_len);
}


void TSP_set_common_hdr(TlmSpacePacket* tsp)
{
  // ここでは Version Number は Version-1 固定
  TSP_set_ver(tsp, SP_VER_1);
  // Telemetry Packet なので TypeはTelemetry 固定
  TSP_set_type(tsp, SP_TYPE_TLM);
  // ここでは Secondary Header は必須
  TSP_set_2nd_hdr_flag(tsp, SP_2ND_HDR_FLAG_PRESENT);
  // ここでは Sequence Flag は Standalone Packet に固定     // FIXME: きちんとやる
  TSP_set_seq_flag(tsp, SP_SEQ_FLAG_SINGLE);
}


void TSP_setup_fill_packet(TlmSpacePacket* tsp, uint16_t fill_size)
{
  // パケットの内容を0クリア。
  memset(tsp->packet, 0x00, TSP_MAX_LEN);

  if (fill_size > SP_PRM_HDR_LEN)
  {
    // Fill領域が Packet Primary Header 長より大きい場合
    // Fill領域に ピッタリハマる Fill Packet を生成する
    TSP_setup_primary_hdr(tsp,
                          APID_FILL_PKT,
                          0,        // FIXME: これでいいのか確認
                          (uint16_t)fill_size);
  }
  else
  {
    // Fill領域が Packet Primary Header 長より小さい場合
    // 領域ピッタリの Fill Packet の生成は不可能なので，データ長 1 のFill Packetを生成する。
    TSP_setup_primary_hdr(tsp,
                          APID_FILL_PKT,
                          0,        // FIXME: これでいいのか確認
                          SP_PRM_HDR_LEN + 1);
  }
}

#pragma section
