/**
 * @file
 * @brief CCSDS の APID を定義する
 * @note  common_tlm_cmd_packet.h などから include されることを前提
 */
#ifndef APID_DEFINE_H_
#define APID_DEFINE_H_

/**
 * @enum   APID
 * @brief  Application Process ID
 * @note   11bit
 */
typedef enum
{
  APID_MOBC_CMD = 0x210,    //!< 01000010000b:
  APID_AOBC_CMD = 0x211,    //!< 01000010001b:
  APID_TOBC_CMD = 0x212,    //!< 01000010010b:
  APID_TCAL_TLM = 0x410,    //!< 10000010000b: APID for TIME CARIBLATION TLM
  APID_MIS_TLM  = 0x510,    //!< 10100010000b: APID for MIS TLM
  APID_DUMP_TLM = 0x710,    //!< 11100010000b: APID for DUMP TLM
  APID_FILL_PKT = 0x7ff,    //!< 11111111111b: APID for FILL PACKET
  APID_UNKNOWN
} APID;

#endif
