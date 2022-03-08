#pragma section REPRO
#include "TCFrame.h"

#include <string.h> // for memcpy

TCF_VER TCF_get_ver(const TCFrame* tcf)
{
  uint32_t pos = 0;
  uint8_t mask = 0xc0; // 1100 0000b

  TCF_VER ver = (TCF_VER)((tcf->packet[pos] & mask) >> 6);

  switch (ver)
  {
  case TCF_VER_1:
    return ver;

  default:
    return TCF_VER_UNKNOWN;
  }
}

TCF_TYPE TCF_get_type(const TCFrame* tcf)
{
  uint32_t pos = 0;
  uint8_t mask = 0x30; // 0011 0000b

  TCF_TYPE type = (TCF_TYPE)((tcf->packet[pos] & mask) >> 4);

  switch (type)
  {
  case TCF_TYPE_AD: // FALL THROUGH
  case TCF_TYPE_BD: // FALL THROUGH
  case TCF_TYPE_BC:
    return type;

  default:
    return TCF_TYPE_UNKNOWN;
  }
}

TCF_SCID TCF_get_scid(const TCFrame* tcf)
{
  uint32_t pos = 0;
  uint8_t mask = 0x03; // 0000 0011b

  // pos = 0の下位2bitsとpos = 1の8bitsを合わせた10bits
  TCF_SCID scid = (TCF_SCID)(tcf->packet[pos] & mask);
  scid = (TCF_SCID)(scid << 8);
  scid = (TCF_SCID)(scid + tcf->packet[pos + 1]);

  switch (scid)
  {
  case TCF_SCID_SAMPLE_SATELLITE:
    return scid;

  default:
    return TCF_SCID_UNKNOWN;
  }
}

TCF_VCID TCF_get_vcid(const TCFrame* tcf)
{
  uint32_t pos = 2;
  uint8_t mask = 0xfc; // 1111 1100b

  TCF_VCID vcid = (TCF_VCID)((tcf->packet[pos] & mask) >> 2);

  switch (vcid)
  {
  case TCF_VCID_REALTIME:
    return vcid;

  default:
    return TCF_VCID_UNKNOWN;
  }
}

size_t TCF_get_frame_len(const TCFrame* tcf)
{
  uint32_t pos = 2;
  uint8_t mask = 0x03; // 0000 0011b

  // pos = 0の下位2bitsとpos = 1の8bitsを合わせた10bits
  size_t len = (tcf->packet[pos] & mask);
  len <<= 8;
  len += tcf->packet[pos + 1];

  // TC Frameの長さ表記は0起算なので1起算に変換した値を返す
  return len + 1;
}

uint8_t TCF_get_frame_seq_num(const TCFrame* tcf)
{
  size_t pos = 4;
  return tcf->packet[pos];
}

const TCSegment* TCF_get_tc_segment(const TCFrame* tcf)
{
  size_t pos = TCF_HEADER_SIZE;
  return (const TCSegment*)&tcf->packet[pos];
}

uint16_t TCF_get_fecw(const TCFrame* tcf)
{
  size_t length = TCF_get_frame_len(tcf);

  uint16_t fecw = tcf->packet[length - 2];
  fecw <<= 8;
  fecw += tcf->packet[length - 1];

  return fecw;
}

const TCFrame* TCF_convert_raw_byte(const uint8_t* byte)
{
  return (const TCFrame*)byte;
}

#pragma section
