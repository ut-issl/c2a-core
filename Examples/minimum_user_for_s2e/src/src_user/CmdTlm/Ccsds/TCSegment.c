#pragma section REPRO
#include "TCSegment.h"

#include <string.h> // for memcpy

TCS_SEQ_FLAG TCS_get_seq_flag(const TCS* tcs)
{
  unsigned int pos = 0;
  uint8_t mask = 0xc0; // 1100 0000b

  return (TCS_SEQ_FLAG)((tcs->header[pos] & mask) >> 6);
}

TCS_MAP_ID TCS_get_map_id(const TCS* tcs)
{
  unsigned int pos = 0;
  uint8_t mask = 0x3f;

  TCS_MAP_ID map_id = (TCS_MAP_ID)(tcs->header[pos] & mask);

  switch (map_id)
  {
  case TCS_MAP_ID_DHU_HDC: // FALL THROUGH
  case TCS_MAP_ID_NORMAL:  // FALL THROUGH
  case TCS_MAP_ID_LONG:
    return map_id;

  default:
    return TCS_MAP_ID_UNKNOWN;
  }
}
#pragma section
