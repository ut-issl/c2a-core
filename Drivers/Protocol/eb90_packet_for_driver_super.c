#pragma section REPRO
/**
 * @file
 * @brief コンポ間通信などで標準的に使う ネットワーク層 の EB90 Packet
 * @note  一般的には，データリンク層は EB90 Frame を使うことを想定
 * @note  Tlm か Cmd かはコンテキストで読み替える
 * @note  データリンク層は DS_StreamConfig.data_link_layer_ で規定する
 */
#include "eb90_packet_for_driver_super.h"
#include "eb90_frame_for_driver_super.h"
#include "../../Library/endian.h"

// FIXME: DS_StreamConfig.data_link_layer_ をちゃんと見る！

uint32_t EB90_PACKET_get_version_from_dssc(const DS_StreamConfig* p_stream_config)
{
  // TODO: 現状は Version 番号が 8bit のもののみしか対応していない
  return (uint32_t)(EB90_FRAME_get_packet_head_from_dssc(p_stream_config)[0]);
}


uint32_t EB90_PACKET_get_count_from_dssc(const DS_StreamConfig* p_stream_config)
{
  const uint32_t version = EB90_PACKET_get_version_from_dssc(p_stream_config);

  if (version == 1)
  {
    return (uint32_t)(EB90_FRAME_get_packet_head_from_dssc(p_stream_config)[1]);
  }
  else
  {
    return 0;
  }
}


uint32_t EB90_PACKET_get_id_from_dssc(const DS_StreamConfig* p_stream_config)
{
  const uint32_t version = EB90_PACKET_get_version_from_dssc(p_stream_config);

  if (version == 1)
  {
    uint16_t id;
    ENDIAN_memcpy(&id, &(EB90_FRAME_get_packet_head_from_dssc(p_stream_config)[2]), sizeof(uint16_t));
    return (uint32_t)id;
  }
  else
  {
    return 0;
  }
}


const uint8_t* EB90_PACKET_get_user_data_head_from_dssc(const DS_StreamConfig* p_stream_config)
{
  const uint32_t version = EB90_PACKET_get_version_from_dssc(p_stream_config);

  if (version == 1)
  {
    return &(EB90_FRAME_get_packet_head_from_dssc(p_stream_config)[4]);
  }
  else
  {
    return NULL;
  }
}

#pragma section
