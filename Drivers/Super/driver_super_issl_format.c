#pragma section REPRO
/**
 * @file
 * @brief  Driver のための ISSL フォーマット通信のための公開定義，関数など
 */

#include "driver_super_issl_format.h"
#include "../../Library/crc.h"
#include <string.h>


uint32_t DS_ISSLFMT_get_tlm_version(const DS_StreamConfig* p_stream_config)
{
  // [TODO] 現状は Version 番号が 8bit のもののみしか対応していない
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
  // TODO: 2bit のもののみしか対応していない
  uint16_t length = DSSC_get_rx_frame(p_stream_config)[2];
  length <<= 8;
  length  |= DSSC_get_rx_frame(p_stream_config)[3];
  return (uint32_t)length;
}

uint16_t DS_ISSLFMT_calc_crc(const unsigned char* c, size_t n)
{
  return crc_16_ibm_right(0x0000, c, n, 0);
}

const uint8_t* DS_C2AFMT_get_user_data_head(const DS_StreamConfig* p_stream_config)
{
  return &(DSSC_get_rx_frame(p_stream_config)[DS_ISSLFMT_COMMON_HEADER_SIZE]);
}

DS_ERR_CODE DS_C2AFMT_get_ctp(const DS_StreamConfig* p_stream_config, CommonTlmPacket* ctp)
{
  uint32_t issl_fmt_user_data_len = DS_ISSLFMT_get_tlm_length(p_stream_config);
  if (issl_fmt_user_data_len > CTP_MAX_LEN) return DS_ERR_CODE_ERR;

  // まず， CTP 最大長だけコピーしてから，アサーションする（効率のため）
  memcpy(&ctp->packet, DS_C2AFMT_get_user_data_head(p_stream_config), (size_t)issl_fmt_user_data_len);

  if (CTP_get_packet_len(ctp) != issl_fmt_user_data_len) return DS_ERR_CODE_ERR;
  if (!CTP_is_valid_packet(ctp)) return DS_ERR_CODE_ERR;
  return DS_ERR_CODE_OK;
}

#pragma section
