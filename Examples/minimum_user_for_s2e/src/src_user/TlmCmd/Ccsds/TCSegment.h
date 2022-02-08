#ifndef TCSEGMENT_H_
#define TCSEGMENT_H_

#include <stddef.h> // for size_t

#include "../../Library/stdint.h"
#include <src_core/TlmCmd/Ccsds/cmd_space_packet.h>

// FIXME: TCP から CmdSpacePacket にしたことによる整理をする！！！

#define TCS_HEADER_SIZE (1u)

typedef struct
{
  uint8_t header[TCS_HEADER_SIZE];
  CmdSpacePacket tcp;      // FIXME: TCP から CSP に諸々直す！
} TCS;

typedef enum
{
  TCS_SEQ_FIRST  = 1, // 01b: First Segment
  TCS_SEQ_CONT   = 0, // 00b: Continuing Segment
  TCS_SEQ_LAST   = 2, // 10b: Last Segment
  TCS_SEQ_SINGLE = 3  // 11b: No Segmentation
} TCS_SEQ_FLAG;

typedef enum
{
  TCS_MAP_ID_DHU_HDC = 1, // 000001b: DHU Hard Decode Command Packet
  TCS_MAP_ID_NORMAL  = 2, // 000010b: Normal Command Packet
  TCS_MAP_ID_LONG    = 4, // 000100b: Long Command Packet
  TCS_MAP_ID_UNKNOWN
} TCS_MAP_ID;

TCS_SEQ_FLAG TCS_get_seq_flag(const TCS* tcs);

TCS_MAP_ID TCS_get_map_id(const TCS* tcs);

#endif
