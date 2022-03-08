/**
 * @file
 * @brief CCSDS で規定される TC Frame の実装
 * @note  packet 構造
 *        |---------+-------+-------+-----------------------|
 *        | Pos     | Pos   | size  | name                  |
 *        | [octet] | [bit] | [bit] |                       |
 *        |---------+-------+-------+-----------------------|
 *        | === Primary Header =============================|
 *        |---------+-------+-------+-----------------------|
 *        |       0 |     0 |     2 | Version               |
 *        |       0 |     2 |     2 | Type                  |
 *        |       0 |     4 |     2 | N/A                   |
 *        |       0 |     6 |    10 | Spacecraft ID         |
 *        |       2 |     0 |     6 | Virtual Channel ID    |
 *        |       2 |     6 |    10 | Frame Length          |
 *        |       4 |     0 |     8 | Frame Sequence Number |
 *        |---------+-------+-------+-----------------------|
 *        | === User Data Field ============================|
 *        |---------+-------+-------+-----------------------|
 *        |       5 |     0 |     * | Command Space Packet  |
 *        |---------+-------+-------+-----------------------|
 *        | === Frame Error Control Field ==================|
 *        |---------+-------+-------+-----------------------|
 *        |       * |     0 |    16 |                       |
 *        |---------+-------+-------+-----------------------|
 */
#ifndef TCFRAME_H_
#define TCFRAME_H_

#include <stddef.h>

#include "../../Library/stdint.h"
#include "TCSegment.h"

#define TCF_MAX_LEN (1024u)
#define TCF_HEADER_SIZE (5u)
#define TCF_FECF_SIZE (2u)

#define TCF_BC_CMD_CODE_UNLOCK (0x00)
#define TCF_BC_CMD_CODE_SET_VR_0 (0x82)
#define TCF_BC_CMD_CODE_SET_VR_1 (0x00)

#define TCF_PACKET_MAX_LENGTH (TCF_HEADER_SIZE + TCS_PACKET_MAX_LENGTH + TCF_FECF_SIZE)

typedef struct
{
  uint8_t packet[TCF_PACKET_MAX_LENGTH];
} TCFrame;

typedef enum
{
  TCF_VER_1 = 0, // 00b: Version 1
  TCF_VER_UNKNOWN
} TCF_VER;

typedef enum
{
  TCF_TYPE_AD = 0, // 00b: TC data with FARM
  TCF_TYPE_BD = 2, // 10b: TC data without FARM
  TCF_TYPE_BC = 3, // 11b: FARM Control Command
  TCF_TYPE_UNKNOWN
} TCF_TYPE;

typedef enum
{
  TCF_SCID_SAMPLE_SATELLITE = 0x00, // SCID for command of sample satellite
  TCF_SCID_UNKNOWN
} TCF_SCID;

typedef enum
{
  TCF_VCID_REALTIME = 0,
  TCF_VCID_UNKNOWN
} TCF_VCID;

TCF_VER TCF_get_ver(const TCFrame* tcf);

TCF_TYPE TCF_get_type(const TCFrame* tcf);

TCF_SCID TCF_get_scid(const TCFrame* tcf);

TCF_VCID TCF_get_vcid(const TCFrame* tcf);

size_t TCF_get_frame_len(const TCFrame* tcf);

uint8_t TCF_get_frame_seq_num(const TCFrame* tcf);

uint16_t TCF_get_fecw(const TCFrame* tcf);

const TCSegment* TCF_get_tc_segment(const TCFrame* tcf);

uint16_t TCF_calc_fecw(const TCFrame* tcf);

const TCFrame* TCF_convert_raw_byte(const uint8_t* byte);

#endif
