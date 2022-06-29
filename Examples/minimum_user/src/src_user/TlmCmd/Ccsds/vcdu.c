#pragma section REPRO
#include "vcdu.h"

#include <string.h> // for memcpy

static void VCDU_set_common_hdr_(VCDU* vcdu);
static void VCDU_clear_spare_(VCDU* vcdu);

void VCDU_generate_byte_stream(const VCDU* vcdu,
                               uint8_t byte_stream[VCDU_LEN])
{
  memcpy(byte_stream, vcdu->header, VCDU_HEADER_SIZE);
  byte_stream += VCDU_HEADER_SIZE;

  M_PDU_generate_byte_stream(&(vcdu->m_pdu), byte_stream);
  byte_stream += M_PDU_LEN;

  memcpy(byte_stream, vcdu->trailer, VCDU_TRAILER_SIZE);
}

void VCDU_setup_fill_vcdu(VCDU* vcdu)
{
  VCDU_set_common_hdr_(vcdu);
  VCDU_set_vcid(vcdu, VCDU_VCID_FILL);
  M_PDU_setup_idle_m_pdu(&(vcdu->m_pdu));
}

void VCDU_setup_realtime_vcdu_hdr(VCDU* vcdu,
                                  uint32_t counter)
{
  VCDU_set_common_hdr_(vcdu);
  VCDU_set_vcid(vcdu, VCDU_VCID_REALTIME);
  VCDU_set_vcdu_counter(vcdu, counter);
}

void VCDU_setup_replay_vcdu_hdr(VCDU* vcdu,
                                uint32_t counter)
{
  VCDU_set_common_hdr_(vcdu);
  VCDU_set_vcid(vcdu, VCDU_VCID_REPLAY);
  VCDU_set_vcdu_counter(vcdu, counter);
}

static void VCDU_set_common_hdr_(VCDU* vcdu)
{
  VCDU_set_ver(vcdu, VCDU_VER_2);
  VCDU_set_scid(vcdu, VCDU_SCID_SAMPLE_SATELLITE); // FIXME: 仮
  VCDU_set_replay_flag(vcdu, VCDU_REPLAY_FALSE);
  VCDU_clear_spare_(vcdu);
}

static void VCDU_clear_spare_(VCDU* vcdu)
{
  unsigned int pos = 5;
  uint8_t mask = 0x7f; // 01111111b

  vcdu->header[pos] &= (uint8_t)(~mask);
}

VCDU_VER VCDU_get_ver(const VCDU* vcdu)
{
  unsigned int pos = 0;
  uint8_t mask = 0xc0; // 11000000b

  VCDU_VER ver = (VCDU_VER)((vcdu->header[pos] & mask) >> 6);

  switch (ver)
  {
  case VCDU_VER_2:
    return ver;

  default:
    return VCDU_VER_UNKNOWN;
  }
}

void VCDU_set_ver(VCDU* vcdu,
                  VCDU_VER ver)
{
  unsigned int pos = 0;
  uint8_t mask = 0xc0; // 11000000b
  uint8_t val = (uint8_t)((ver << 6) & mask);

  vcdu->header[pos] &= (uint8_t)(~mask);
  vcdu->header[pos] |= val;
}

VCDU_SCID VCDU_get_scdi(const VCDU* vcdu)
{
  unsigned int pos = 0;
  uint8_t mask1 = 0x3f; // 00111111b
  uint8_t mask2 = 0xc0; // 11000000b

  int scid = (vcdu->header[pos] & mask1);
  scid <<= 2;
  scid |= ((vcdu->header[pos + 1] & mask2) >> 6);

  switch (scid)
  {
  case VCDU_SCID_SAMPLE_SATELLITE:
    return (VCDU_SCID)scid;

  default:
    return VCDU_SCID_UNKNOWN;
  }
}

void VCDU_set_scid(VCDU* vcdu,
                   VCDU_SCID scid)
{
  unsigned int pos = 0;
  uint8_t mask1 = 0x3f; // 00111111b
  uint8_t mask2 = 0xc0; // 11000000b

  vcdu->header[pos] &= (uint8_t)(~mask1);
  vcdu->header[pos] |= (uint8_t)((scid >> 2) & mask1);
  vcdu->header[pos + 1] &= (uint8_t)(~mask2);
  vcdu->header[pos + 1] |= (uint8_t)((scid << 6) & mask2);
}

VCDU_VCID VCDU_get_vcid(const VCDU* vcdu)
{
  unsigned int pos = 1;
  uint8_t mask = 0x3f; // 00111111b

  VCDU_VCID vcid = (VCDU_VCID)(vcdu->header[pos] & mask);

  switch (vcid)
  {
  case VCDU_VCID_REALTIME: // FALLTHROUGH
  case VCDU_VCID_REPLAY: // FALLTHROUGH
  case VCDU_VCID_FILL: // FALLTHROUGH
    return vcid;

  default:
    return VCDU_VCID_UNKNOWN;
  }
}

void VCDU_set_vcid(VCDU* vcdu,
                   VCDU_VCID vcid)
{
  unsigned int pos = 1;
  uint8_t mask = 0x3f; // 00111111b

  vcdu->header[pos] &= (uint8_t)(~mask);
  vcdu->header[pos] |= (uint8_t)(vcid & mask);
}

uint32_t VCDU_get_vcdu_counter(const VCDU* vcdu)
{
  unsigned int pos = 2;

  uint32_t counter = vcdu->header[pos];
  counter <<= 8;
  counter |=  vcdu->header[pos + 1];
  counter <<= 8;
  counter |=  vcdu->header[pos + 2];

  return counter;
}

void VCDU_set_vcdu_counter(VCDU* vcdu,
                           uint32_t counter)
{
  unsigned int pos = 2;

  vcdu->header[pos] = (uint8_t)((counter >> 16) & 0xff);
  vcdu->header[pos + 1] = (uint8_t)((counter >> 8) & 0xff);
  vcdu->header[pos + 2] = (uint8_t)(counter & 0xff);
}

VCDU_REPLAY_FLAG VCDU_get_replay_flag(const VCDU* vcdu)
{
  unsigned int pos = 5;
  uint8_t mask = 0x80; // 10000000b

  return (VCDU_REPLAY_FLAG)((vcdu->header[pos] & mask) >> 7);
}

void VCDU_set_replay_flag(VCDU* vcdu,
                          VCDU_REPLAY_FLAG flag)
{
  unsigned int pos = 5;
  uint8_t mask = 0x80; // 10000000b

  vcdu->header[pos] &= (uint8_t)(~mask);
  vcdu->header[pos] |= (uint8_t)((flag << 7) & mask);
}

uint32_t VCDU_get_clcw(const VCDU* vcdu)
{
  unsigned int pos = 0;

  uint32_t clcw = vcdu->trailer[pos];
  clcw <<= 8;
  clcw |= vcdu->trailer[pos + 1];
  clcw <<= 8;
  clcw |= vcdu->trailer[pos + 2];
  clcw <<= 8;
  clcw |= vcdu->trailer[pos + 3];

  return clcw;
}

void VCDU_set_clcw(VCDU* vcdu,
                   uint32_t clcw)
{
  unsigned int pos = 0;

  vcdu->trailer[pos] = (uint8_t)((clcw >> 24) & 0xff);
  vcdu->trailer[pos + 1] = (uint8_t)((clcw >> 16) & 0xff);
  vcdu->trailer[pos + 2] = (uint8_t)((clcw >> 8) & 0xff);
  vcdu->trailer[pos + 3] = (uint8_t)(clcw & 0xff);
}

uint32_t VCDU_calc_next_counter(uint32_t prev)
{
  return (prev + 1) % VCDU_COUNTER_MAX;
}
#pragma section
