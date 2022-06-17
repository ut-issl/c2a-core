#pragma section REPRO
/**
 * @file
 * @brief CCSDS で規定される TC Segment の実装
 */
#include "tc_segment.h"

TCS_SEQ_FLAG TCS_get_seq_flag(const TcSegment* tcs)
{
  uint8_t pos = 0;
  uint8_t mask = 0xc0; // 1100 0000b

  return (TCS_SEQ_FLAG)((tcs->packet[pos] & mask) >> 6);
}

TCS_MAP_ID TCS_get_map_id(const TcSegment* tcs)
{
  uint8_t pos = 0;
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

const CmdSpacePacket* TCS_get_command_space_packet(const TcSegment* tcs)
{
  return (const CmdSpacePacket*)(&tcs->packet[TCS_HEADER_SIZE]);
}

#pragma section
