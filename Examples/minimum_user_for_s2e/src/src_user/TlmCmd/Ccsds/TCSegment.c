#pragma section REPRO
#include "TCSegment.h"

TCS_SEQ_FLAG TCS_get_seq_flag(const TCSegment* tcs)
{
  unsigned int pos = 0;
  uint8_t mask = 0xc0; // 1100 0000b

  return (TCS_SEQ_FLAG)((tcs->packet[pos] & mask) >> 6);
}

TCS_MAP_ID TCS_get_map_id(const TCSegment* tcs)
{
  unsigned int pos = 0;
  uint8_t mask = 0x3f; // 0011 1111b

  TCS_MAP_ID map_id = (TCS_MAP_ID)(tcs->packet[pos] & mask);

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
