/**
 * @file
 * @brief CCSDS で規定される TC Transfer Frame の実装
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
 *        |       5 |     0 |     * | TC Segment            |
 *        |---------+-------+-------+-----------------------|
 *        | === Frame Error Control Field ==================|
 *        |---------+-------+-------+-----------------------|
 *        |       * |     0 |    16 |                       |
 *        |---------+-------+-------+-----------------------|
 */
#ifndef TC_TRANSFER_FRAME_H_
#define TC_TRANSFER_FRAME_H_

#include <stddef.h>

#include "../../Library/stdint.h"
#include "TCSegment.h"

#define TCTF_MAX_LEN (1024u)
#define TCTF_HEADER_SIZE (5u)
#define TCTF_FECF_SIZE (2u)

#define TCTF_BC_CMD_CODE_UNLOCK (0x00)
#define TCTF_BC_CMD_CODE_SET_VR_0 (0x82)
#define TCTF_BC_CMD_CODE_SET_VR_1 (0x00)

#define TCTF_PACKET_MAX_LENGTH (TCTF_HEADER_SIZE + TCS_PACKET_MAX_LENGTH + TCTF_FECF_SIZE)

typedef struct
{
  uint8_t packet[TCTF_PACKET_MAX_LENGTH];
} TcTransferFrame;

typedef enum
{
  TCTF_VER_1 = 0, // 00b: Version 1
  TCTF_VER_UNKNOWN
} TCTF_VER;

typedef enum
{
  TCTF_TYPE_AD = 0, // 00b: TC data with FARM
  TCTF_TYPE_BD = 2, // 10b: TC data without FARM
  TCTF_TYPE_BC = 3, // 11b: FARM Control Command
  TCTF_TYPE_UNKNOWN
} TCTF_TYPE;

typedef enum
{
  TCTF_SCID_SAMPLE_SATELLITE = 0x00, // SCID for command of sample satellite
  TCTF_SCID_UNKNOWN
} TCTF_SCID;

typedef enum
{
  TCTF_VCID_REALTIME = 0,
  TCTF_VCID_UNKNOWN
} TCTF_VCID;

TCTF_VER TCTF_get_ver(const TcTransferFrame* tctf);

TCTF_TYPE TCTF_get_type(const TcTransferFrame* tctf);

TCTF_SCID TCTF_get_scid(const TcTransferFrame* tctf);

TCTF_VCID TCTF_get_vcid(const TcTransferFrame* tctf);

size_t TCTF_get_frame_len(const TcTransferFrame* tctf);

uint8_t TCTF_get_frame_seq_num(const TcTransferFrame* tctf);

uint16_t TCTF_get_fecw(const TcTransferFrame* tctf);

const TCSegment* TCTF_get_tc_segment(const TcTransferFrame* tctf);

uint16_t TCTF_calc_fecw(const TcTransferFrame* tctf);

const TcTransferFrame* TCTF_convert_from_bytes_to_tctf(const uint8_t* byte);

#endif
