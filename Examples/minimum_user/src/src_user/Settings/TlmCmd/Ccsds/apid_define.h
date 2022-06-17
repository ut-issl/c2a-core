/**
 * @file
 * @brief CCSDS の APID を定義する
 * @note  common_tlm_cmd_packet.h などから include されることを前提
 */
#ifndef APID_DEFINE_H_
#define APID_DEFINE_H_

#include "../../../Library/stdint.h"

/**
 * @enum   APID
 * @brief  Application Process ID
 * @note   11bit
 */
typedef enum
{
  APID_MOBC_CMD = 0x210,    //!< 01000010000b: APID for MOBC 宛の CMD
  APID_AOBC_CMD = 0x211,    //!< 01000010001b: APID for AOBC 宛の CMD
  APID_TOBC_CMD = 0x212,    //!< 01000010010b: APID for TOBC 宛の CMD
  APID_TCAL_TLM = 0x410,    //!< 10000010000b: APID for TIME CARIBLATION TLM (FIXME: 現在まともに使ってない)
  APID_MOBC_TLM = 0x510,    //!< 10100010000b: APID for MOBC で生成される TLM
  APID_AOBC_TLM = 0x511,    //!< 10100010001b: APID for AOBC で生成される TLM
  APID_TOBC_TLM = 0x512,    //!< 10100010002b: APID for TOBC で生成される TLM
  APID_DUMP_TLM = 0x710,    //!< 11100010000b: APID for DUMP TLM (FIXME: 現在まともに使ってない)
  APID_FILL_PKT = 0x7ff,    //!< 11111111111b: APID for FILL PACKET
  APID_UNKNOWN
} APID;


/**
 * @brief  バイト列から APID を取得
 * @param  apid: APID 候補の uint16_t
 * @note   不正な入力のときは APID_UNKNOWN を返す
 * @return APID
 */
APID APID_get_apid_from_uint16(uint16_t apid);

/**
 * @brief  入力した APID が他の OBC で生成された TLM の APID かどうか
 * @param  apid: APID
 * @note   不正な APID は 0 を返す
 * @retval 1: 他 OBC で生成された TLM の APID
 * @retval 0: それ以外の APID
 */
int APID_is_other_obc_tlm_apid(APID apid);

#endif
