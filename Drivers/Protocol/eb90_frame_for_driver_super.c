#pragma section REPRO
/**
 * @file
 * @brief コンポ間通信などで標準的に使う データリンク層 の EB90 Frame
 */
#include "eb90_frame_for_driver_super.h"
#include "../../Library/endian.h"
#include "../../Library/crc.h"

const uint8_t EB90_FRAME_kStx[EB90_FRAME_STX_SIZE] = {EB90_FRAME_STX_1ST_BYTE, EB90_FRAME_STX_2ND_BYTE};
const uint8_t EB90_FRAME_kEtx[EB90_FRAME_ETX_SIZE] = {EB90_FRAME_ETX_1ST_BYTE, EB90_FRAME_ETX_2ND_BYTE};


uint16_t EB90_FRAME_get_packet_length_from_dssc(const DS_StreamConfig* p_stream_config)
{
  uint16_t length;
  ENDIAN_memcpy(&length, &(DSSC_get_rx_frame(p_stream_config)[2]), EB90_FRAME_LEN_SIZE);
  return length;
}


const uint8_t* EB90_FRAME_get_packet_head_from_dssc(const DS_StreamConfig* p_stream_config)
{
  return &(DSSC_get_rx_frame(p_stream_config)[EB90_FRAME_HEADER_SIZE]);
}


uint16_t EB90_FRAME_get_crc_from_dssc(const DS_StreamConfig* p_stream_config)
{
  uint16_t crc;
  uint16_t offset = EB90_FRAME_HEADER_SIZE + EB90_FRAME_get_packet_length_from_dssc(p_stream_config);
  ENDIAN_memcpy(&crc, &(DSSC_get_rx_frame(p_stream_config)[offset]), EB90_FRAME_CRC_SIZE);
  return crc;
}


uint8_t EB90_FRAME_is_valid_crc_of_dssc(const DS_StreamConfig* p_stream_config)
{
  uint16_t len = EB90_FRAME_get_packet_length_from_dssc(p_stream_config);
  const uint8_t* head = EB90_FRAME_get_packet_head_from_dssc(p_stream_config);
  return (EB90_FRAME_calc_crc(head, len + EB90_FRAME_CRC_SIZE) == 0) ? 1 : 0;
}


uint16_t EB90_FRAME_calc_crc(const uint8_t* data, size_t len)
{
  // CRC-16/CCITT-FALSE (CRC-16/AUTOSAR, CRC-16/IBM-3740 とも)
  // https://reveng.sourceforge.io/crc-catalogue/16.htm
  return CRC_calc_crc_16_ccitt_left(0xffff, data, len, 0);
}

#pragma section
