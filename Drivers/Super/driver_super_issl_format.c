#pragma section REPRO
/**
 * @file
 * @brief  DriverのためのISSLフォーマット通信のための公開定義，関数など
 */

#include "driver_super_issl_format.h"
#include "../../Library/crc.h"


uint32_t DS_ISSLFMT_get_tlm_version(const DS_StreamConfig* p_stream_config)
{
  // [TODO] 現状はVersion番号が8bitのもののみしか対応していない
  return (uint32_t)(DSSC_get_rx_frame(p_stream_config)[DS_ISSLFMT_COMMON_HEADER_SIZE]);
}

uint32_t DS_ISSLFMT_get_tlm_count(const uint32_t version, const DS_StreamConfig* p_stream_config)
{
  if (version == 1)
  {
    return (uint32_t)(DSSC_get_rx_frame(p_stream_config)[DS_ISSLFMT_COMMON_HEADER_SIZE + 1]);
  }
  else
  {
    return 0;
  }
}

uint32_t DS_ISSLFMT_get_tlm_id(const uint32_t version, const DS_StreamConfig* p_stream_config)
{
  if (version == 1)
  {
    uint16_t id = DSSC_get_rx_frame(p_stream_config)[DS_ISSLFMT_COMMON_HEADER_SIZE + 2];
    id <<= 8;
    id  |= DSSC_get_rx_frame(p_stream_config)[DS_ISSLFMT_COMMON_HEADER_SIZE + 3];
    return (uint32_t)id;
  }
  else
  {
    return 0;
  }
}

uint32_t DS_ISSLFMT_get_tlm_length(const DS_StreamConfig* p_stream_config)
{
  // [TODO] 2bitのもののみしか対応していない
  uint16_t length = DSSC_get_rx_frame(p_stream_config)[2];
  length <<= 8;
  length  |= DSSC_get_rx_frame(p_stream_config)[3];
  return (uint32_t)length;
}

uint16_t DS_ISSLFMT_calc_crc(const unsigned char* c, size_t n)
{
  return crc_16_ibm_right(0x0000, c, n, 0);
}

uint8_t DS_C2AFMT_get_tlm_id(const DS_StreamConfig* p_stream_config)
{
  uint16_t offset = DS_ISSLFMT_COMMON_HEADER_SIZE + DS_C2AFMT_TCP_TLM_PRIMARY_HEADER_SIZE + 5;
  uint8_t  tlm_id = DSSC_get_rx_frame(p_stream_config)[offset];
  return tlm_id;

  // [TODO] 本当は以下の関数を使いたいが，TPC型にmemcpyしないとこの関数が使えない．．．
  // return TCP_TLM_get_packet_id();
}

#pragma section
