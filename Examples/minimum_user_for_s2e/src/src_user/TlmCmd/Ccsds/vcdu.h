#ifndef VCDU_H_
#define VCDU_H_

#include "m_pdu.h"

#define VCDU_HEADER_SIZE (6u)
#define VCDU_TRAILER_SIZE (4u)
#define VCDU_LEN (VCDU_HEADER_SIZE + M_PDU_LEN + VCDU_TRAILER_SIZE)
#define VCDU_COUNTER_MAX (0x01000000) // 24bit長

typedef struct
{
  uint8_t header[VCDU_HEADER_SIZE];
  M_PDU m_pdu;
  uint8_t trailer[VCDU_TRAILER_SIZE];
} VCDU;

typedef enum
{
  VCDU_VER_2 = 1, // 01b: Version2
  VCDU_VER_UNKNOWN
} VCDU_VER;

typedef enum
{
  VCDU_SCID_SAMPLE_SATELLITE = 0x00,
  VCDU_SCID_UNKNOWN
} VCDU_SCID;

typedef enum
{
  VCDU_VCID_REALTIME = 0x01, // 000001b: Realtime Transfer Frame
  VCDU_VCID_REPLAY = 0x02, // 000010b: Stored Transfer Frame
  VCDU_VCID_FILL = 0x3f, // 111111b: Fill Transfer Frame
  VCDU_VCID_UNKNOWN
} VCDU_VCID;

typedef enum
{
  VCDU_REPLAY_FALSE = 0, // 0b: Realtime Transfer Frame
  VCDU_REPLAY_TRUE = 1 // 1b: Replay Transfer Frame
} VCDU_REPLAY_FLAG;

void VCDU_generate_byte_stream(const VCDU* vcdu,
                               uint8_t byte_stream[VCDU_LEN]);

void VCDU_setup_fill_vcdu(VCDU* vcdu);

void VCDU_setup_realtime_vcdu_hdr(VCDU* vcdu,
                                  uint32_t counter);

void VCDU_setup_replay_vcdu_hdr(VCDU* vcdu,
                                uint32_t counter);

VCDU_VER VCDU_get_ver(const VCDU* vcdu);

void VCDU_set_ver(VCDU* vcdu,
                  VCDU_VER ver);

VCDU_SCID VCDU_get_scdi(const VCDU* vcdu);

void VCDU_set_scid(VCDU* vcdu,
                   VCDU_SCID scid);

VCDU_VCID VCDU_get_vcid(const VCDU* vcdu);

void VCDU_set_vcid(VCDU* vcdu,
                   VCDU_VCID vcid);

uint32_t VCDU_get_vcdu_counter(const VCDU* vcdu);

void VCDU_set_vcdu_counter(VCDU* vcdu,
                           uint32_t counter);

VCDU_REPLAY_FLAG VCDU_get_replay_flag(const VCDU* vcdu);

void VCDU_set_replay_flag(VCDU* vcdu,
                          VCDU_REPLAY_FLAG flag);

uint32_t VCDU_get_clcw(const VCDU* vcdu);

void VCDU_set_clcw(VCDU* vcdu,
                   uint32_t clcw);

uint32_t VCDU_calc_next_counter(uint32_t prev);

#endif // VCDU_H_
