#ifndef TCFRAME_H_
#define TCFRAME_H_

#include <stddef.h> // for size_t

#include "../../Library/stdint.h"
#include "TCSegment.h"

#define TCF_MAX_LEN (1024u)
#define TCF_HEADER_SIZE (5u)
#define TCF_FECF_SIZE (2u)

#define TCF_BC_CMD_CODE_UNLOCK (0x00)
#define TCF_BC_CMD_CODE_SET_VR_0 (0x82)
#define TCF_BC_CMD_CODE_SET_VR_1 (0x00)

typedef struct
{
  uint8_t header[TCF_HEADER_SIZE];
  TCS tcs;
  uint8_t fecf[TCF_FECF_SIZE];
} TCF;

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

TCF_VER TCF_get_ver(const TCF* tcf);

TCF_TYPE TCF_get_type(const TCF* tcf);

TCF_SCID TCF_get_scid(const TCF* tcf);

TCF_VCID TCF_get_vcid(const TCF* tcf);

size_t TCF_get_frame_len(const TCF* tcf);

uint8_t TCF_get_frame_seq_num(const TCF* tcf);

uint16_t TCF_get_fecw(const TCF* tcf);

uint16_t TCF_calc_fecw(const TCF* tcf);

#endif
