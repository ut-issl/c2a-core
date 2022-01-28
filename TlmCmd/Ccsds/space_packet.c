#pragma section REPRO
/**
 * @file
 * @brief CCSDS で規定される Space Packet の実装
 * @note  詳細は space_packet.h を参照
 */
#include "space_packet.h"
#include "../../Library/endian_memcpy.h"
#include <string.h>


static const SP_ParamExtractionInfo SP_pei_ver_           = { 0, 0xe0, 5, 1};  // 11100000b
static const SP_ParamExtractionInfo SP_pei_type_          = { 0, 0x10, 4, 1};  // 00010000b
static const SP_ParamExtractionInfo SP_pei_2nd_hdr_flag_  = { 0, 0x08, 3, 1};  // 00001000b
static const SP_ParamExtractionInfo SP_pei_apid_          = { 0, 0x07, 0, 2};  // 00000111b
static const SP_ParamExtractionInfo SP_pei_seq_flag_      = { 2, 0xc0, 6, 1};  // 11000000b
static const SP_ParamExtractionInfo SP_pei_seq_count_     = { 2, 0x3f, 0, 2};  // 00111111b
static const SP_ParamExtractionInfo SP_pei_pckt_data_len_ = { 4, 0xff, 0, 2};  // 11111111b


// ******************************
//  Primary Header getter/setter
// ******************************

SP_VER SP_get_ver(const SpacePacket* sp)
{
  uint8_t tmp;
  SP_VER ver;

  SP_extract_param_from_packet(sp, &SP_pei_ver_, &tmp);
  ver = (SP_VER)tmp;

  switch (ver)
  {
  case SP_VER_1:
    return ver;
  default:
    return SP_VER_UNKNOWN;
  }
}


void SP_set_ver(SpacePacket* sp, SP_VER ver)
{
  uint8_t tmp = (uint8_t)ver;
  SP_insert_param_to_packet(sp, &SP_pei_ver_, &tmp);
}


SP_TYPE SP_get_type(const SpacePacket* sp)
{
  uint8_t type;

  SP_extract_param_from_packet(sp, &SP_pei_type_, &type);
  return (SP_TYPE)type;
}


void SP_set_type(SpacePacket* sp, SP_TYPE type)
{
  uint8_t tmp = (uint8_t)type;
  SP_insert_param_to_packet(sp, &SP_pei_type_, &tmp);
}


SP_2ND_HDR_FLAG SP_get_2nd_hdr_flag(const SpacePacket* sp)
{
  uint8_t flag;

  SP_extract_param_from_packet(sp, &SP_pei_2nd_hdr_flag_, &flag);
  return (SP_2ND_HDR_FLAG)flag;
}


void SP_set_2nd_hdr_flag(SpacePacket* sp, SP_2ND_HDR_FLAG flag)
{
  uint8_t tmp = (uint8_t)flag;
  SP_insert_param_to_packet(sp, &SP_pei_2nd_hdr_flag_, &tmp);
}


APID SP_get_apid(const SpacePacket* sp)
{
  uint16_t tmp;
  APID apid;

  SP_extract_param_from_packet(sp, &SP_pei_apid_, &tmp);
  apid = (APID)tmp;

  switch (apid)
  {
  case APID_MOBC_CMD:         // FALLTHROUGH
  case APID_AOBC_CMD:         // FALLTHROUGH
  case APID_TOBC_CMD:         // FALLTHROUGH
  case APID_MIS_TLM:          // FALLTHROUGH
  case APID_DUMP_TLM:
    return apid;

  default:
    return APID_UNKNOWN;
  }
}


void SP_set_apid(SpacePacket* sp, APID apid)
{
  uint16_t tmp = (uint16_t)apid;
  SP_insert_param_to_packet(sp, &SP_pei_apid_, &tmp);
}


SP_SEQ_FLAG SP_get_seq_flag(const SpacePacket* sp)
{
  uint8_t flag;

  SP_extract_param_from_packet(sp, &SP_pei_seq_flag_, &flag);
  return (SP_SEQ_FLAG)flag;
}


void SP_set_seq_flag(SpacePacket* sp, SP_SEQ_FLAG flag)
{
  uint8_t tmp = (uint8_t)flag;
  SP_insert_param_to_packet(sp, &SP_pei_seq_flag_, &tmp);
}


uint16_t SP_get_seq_count(const SpacePacket* sp)
{
  uint16_t count;

  SP_extract_param_from_packet(sp, &SP_pei_seq_count_, &count);
  return count;
}


void SP_set_seq_count(SpacePacket* sp, uint16_t count)
{
  SP_insert_param_to_packet(sp, &SP_pei_seq_count_, &count);
}


uint16_t SP_get_packet_data_len(const SpacePacket* sp)
{
  uint16_t len;

  SP_extract_param_from_packet(sp, &SP_pei_pckt_data_len_, &len);
  // Data Length は 0 起算表記なので 1 起算に変換して返す
  return (uint16_t)(len + 1);
}


void SP_set_packet_data_len(SpacePacket* sp, uint16_t len)
{
  if (len == 0) return;

  // Data Length は 0 起算表記なので 1 起算を変換
  len--;
  SP_insert_param_to_packet(sp, &SP_pei_pckt_data_len_, &len);
}


// ******************
//  Utility Function
// ******************

uint16_t SP_get_packet_len(const SpacePacket* sp)
{
  return (uint16_t)(SP_get_packet_data_len(sp) + SP_PRM_HDR_LEN);
}


void SP_set_packet_len(SpacePacket* sp, uint16_t len)
{
  if (len <= SP_PRM_HDR_LEN) return;
  SP_set_packet_data_len(sp, (uint16_t)(len - SP_PRM_HDR_LEN));
}


void SP_copy_packet(SpacePacket* dest, const SpacePacket* src)
{
  memcpy(dest->packet, src->packet, (size_t)SP_get_packet_len(src));
}


void SP_extract_param_from_packet(const SpacePacket* sp,
                                  const SP_ParamExtractionInfo* pei,
                                  void* dst)
{
  uint8_t buffer[8];
  uint8_t i;

  if (pei->byte_len == 0) return;
  if (pei->byte_len > 8) return;
  if (pei->offset > 7) return;

  // offset あり
  if (pei->offset)
  {
    // offset されるのは， 1byte の中で格納される時
    if (pei->byte_len != 1) return;

    buffer[0] = (sp->packet[pei->pos] & pei->mask) >> pei->offset;
  }
  else
  {
    // offset なし
    // 最初の 1byte
    buffer[0] = sp->packet[pei->pos] & pei->mask;

    // 2byte 目以降
    for (i = 1; i < pei->byte_len; ++i)
    {
      buffer[i] = sp->packet[pei->pos + i];
    }
  }

  endian_memcpy(dst, buffer, pei->byte_len);
}


void SP_insert_param_to_packet(SpacePacket* sp,
                               const SP_ParamExtractionInfo* pei,
                               const void* src)
{
  uint8_t buffer[8];
  uint8_t i;

  if (pei->byte_len == 0) return;
  if (pei->byte_len > 8) return;
  if (pei->offset > 7) return;

  endian_memcpy(buffer, src, pei->byte_len);

  // offset あり
  if (pei->offset)
  {
    // offset されるのは， 1byte の中で格納される時
    if (pei->byte_len != 1) return;

    buffer[0] <<= pei->offset;

    sp->packet[pei->pos] &= (uint8_t)(~pei->mask);
    sp->packet[pei->pos] |= (uint8_t)(buffer[0] & pei->mask);
  }
  else
  {
    // offset なし
    // 最初の 1byte
    sp->packet[pei->pos] &= (uint8_t)(~pei->mask);
    sp->packet[pei->pos] |= (uint8_t)(buffer[0] & pei->mask);

    // 2byte 目以降
    for (i = 1; i < pei->byte_len; ++i)
    {
      sp->packet[pei->pos + i] = buffer[i];
    }
  }
}

#pragma section
