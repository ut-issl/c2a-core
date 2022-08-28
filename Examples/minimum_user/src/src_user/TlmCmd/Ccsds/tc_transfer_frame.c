#pragma section REPRO
/**
 * @file
 * @brief CCSDS で規定される TC Transfer Frame の実装
 */
#include "tc_transfer_frame.h"

#include <string.h>
#include <src_core/Library/crc.h>

TCTF_VER TCTF_get_ver(const TcTransferFrame* tctf)
{
  uint8_t pos = 0;
  uint8_t mask = 0xc0; // 1100 0000b

  TCTF_VER ver = (TCTF_VER)((tctf->packet[pos] & mask) >> 6);

  switch (ver)
  {
  case TCTF_VER_1:
    return ver;

  default:
    return TCTF_VER_UNKNOWN;
  }
}

TCTF_TYPE TCTF_get_type(const TcTransferFrame* tctf)
{
  uint8_t pos = 0;
  uint8_t mask = 0x30; // 0011 0000b

  TCTF_TYPE type = (TCTF_TYPE)((tctf->packet[pos] & mask) >> 4);

  switch (type)
  {
  case TCTF_TYPE_AD: // FALL THROUGH
  case TCTF_TYPE_BD: // FALL THROUGH
  case TCTF_TYPE_BC:
    return type;

  default:
    return TCTF_TYPE_UNKNOWN;
  }
}

TCTF_SCID TCTF_get_scid(const TcTransferFrame* tctf)
{
  uint8_t pos = 0;
  uint8_t mask = 0x03; // 0000 0011b

  // pos = 0の下位2bitsとpos = 1の8bitsを合わせた10bits
  TCTF_SCID scid = (TCTF_SCID)(tctf->packet[pos] & mask);
  scid = (TCTF_SCID)(scid << 8);
  scid = (TCTF_SCID)(scid + tctf->packet[pos + 1]);

  switch (scid)
  {
  case TCTF_SCID_SAMPLE_SATELLITE:
    return scid;

  default:
    return TCTF_SCID_UNKNOWN;
  }
}

TCTF_VCID TCTF_get_vcid(const TcTransferFrame* tctf)
{
  uint8_t pos = 2;
  uint8_t mask = 0xfc; // 1111 1100b

  TCTF_VCID vcid = (TCTF_VCID)((tctf->packet[pos] & mask) >> 2);

  switch (vcid)
  {
  case TCTF_VCID_REALTIME:
    return vcid;

  default:
    return TCTF_VCID_UNKNOWN;
  }
}

uint16_t TCTF_get_frame_len(const TcTransferFrame* tctf)
{
  uint8_t pos = 2;
  uint8_t mask = 0x03; // 0000 0011b

  // pos = 0の下位2bitsとpos = 1の8bitsを合わせた10bits
  uint16_t len = (tctf->packet[pos] & mask);
  len <<= 8;
  len += tctf->packet[pos + 1];

  // TC Frameの長さ表記は0起算なので1起算に変換した値を返す
  return len + 1;
}

uint8_t TCTF_get_frame_seq_num(const TcTransferFrame* tctf)
{
  return tctf->packet[4];
}

const TcSegment* TCTF_get_tc_segment(const TcTransferFrame* tctf)
{
  return (const TcSegment*)(&tctf->packet[TCTF_HEADER_SIZE]);
}

uint16_t TCTF_get_fecw(const TcTransferFrame* tctf)
{
  uint16_t length = TCTF_get_frame_len(tctf);

  uint16_t fecw = tctf->packet[length - 2];
  fecw <<= 8;
  fecw += tctf->packet[length - 1];

  return fecw;
}

uint8_t TCTF_check_fecw(const TcTransferFrame* tctf)
{
  uint16_t len = TCTF_get_frame_len(tctf);
  uint16_t result = CRC_calc_crc_16_ccitt_left(0xffff, tctf->packet, len, 0);

  return !result;
}

const TcTransferFrame* TCTF_convert_from_bytes_to_tctf(const uint8_t* byte)
{
  return (const TcTransferFrame*)byte;
}

#pragma section
