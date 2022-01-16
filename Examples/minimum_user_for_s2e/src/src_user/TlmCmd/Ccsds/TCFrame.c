#pragma section REPRO
#include "TCFrame.h"

#include <string.h> // for memcpy

TCF_VER TCF_get_ver(const TCF* tcf)
{
  unsigned int pos = 0;
  uint8_t mask = 0xc0; // 1100 0000b

  TCF_VER ver = (TCF_VER)((tcf->header[pos] & mask) >> 6);

  switch (ver)
  {
  case TCF_VER_1:
    return ver;

  default:
    return TCF_VER_UNKNOWN;
  }
}

TCF_TYPE TCF_get_type(const TCF* tcf)
{
  unsigned int pos = 0;
  uint8_t mask = 0x30; // 0011 0000b

  TCF_TYPE type = (TCF_TYPE)((tcf->header[pos] & mask) >> 4);

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

TCF_SCID TCF_get_scid(const TCF* tcf)
{
  unsigned int pos = 0;
  uint8_t mask = 0x03; // 0000 0011b

  // pos = 0の下位2bitsとpos = 1の8bitsを合わせた10bits
  TCF_SCID scid = (TCF_SCID)(tcf->header[pos] & mask);
  scid = (TCF_SCID)(scid << 8);
  scid = (TCF_SCID)(scid + tcf->header[pos + 1]);

  switch (scid)
  {
  case TCF_SCID_SAMPLE_SATELLITE:
    return scid;

  default:
    return TCF_SCID_UNKNOWN;
  }
}

TCF_VCID TCF_get_vcid(const TCF* tcf)
{
  unsigned int pos = 2;
  uint8_t mask = 0xfc; // 1111 1100b

  TCF_VCID vcid = (TCF_VCID)((tcf->header[pos] & mask) >> 2);

  switch (vcid)
  {
  case TCF_VCID_REALTIME:
    return vcid;

  default:
    return TCF_VCID_UNKNOWN;
  }
}

size_t TCF_get_frame_len(const TCF* tcf)
{
  unsigned int pos = 2;
  uint8_t mask = 0x03; // 0000 0011b

  // pos = 0の下位2bitsとpos = 1の8bitsを合わせた10bits
  size_t len = (tcf->header[pos] & mask);
  len <<= 8;
  len += tcf->header[pos + 1];

  // TC Frameの長さ表記は0起算なので1起算に変換した値を返す
  return len + 1;
}

uint8_t TCF_get_frame_seq_num(const TCF* tcf)
{
  size_t pos = 4;
  return (uint8_t)(tcf->header[pos]);
}

uint16_t TCF_get_fecw(const TCF* tcf)
{
  unsigned int pos = 0;

  uint16_t fecw = tcf->fecf[pos];
  fecw <<= 8;
  fecw += tcf->fecf[pos + 1];

  return fecw;
}

#pragma section
