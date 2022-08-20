#pragma section REPRO
/**
 * @file
 * @brief コンポ間通信などで標準的に使う ネットワーク層 の Common Tlm Cmd Packet (概ね Space Packet)
 * @note  ここでは，データリンク層は CCSDS ではなく EB90 Frame を使うことを想定 (TODO: 今後拡張予定)
 * @note  データリンク層は DS_StreamConfig.data_link_layer_ で規定する
 * @note  packet 構造 などは TlmCmd/common_tlm_cmd_packet.h を参照のこと
 */
#include "common_tlm_cmd_packet_for_driver_super.h"
#include "eb90_frame_for_driver_super.h"
#include <string.h>

// FIXME: DS_StreamConfig.data_link_layer_ をちゃんと見る！

DS_ERR_CODE CTCP_get_ctcp_from_dssc(const DS_StreamConfig* p_stream_config, CommonTlmCmdPacket* ctcp)
{
  const uint16_t packet_len = EB90_FRAME_get_packet_length_from_dssc(p_stream_config);

  if (packet_len > CTCP_MAX_LEN) return DS_ERR_CODE_ERR;

  // まず， 受信データ長だけコピーしてしまってから，アサーションする（効率のため）
  memcpy(&ctcp->packet, EB90_FRAME_get_packet_head_from_dssc(p_stream_config), (size_t)packet_len);
  if (CTCP_get_packet_len(ctcp) != packet_len) return DS_ERR_CODE_ERR;
  if (!CTCP_is_valid_packet(ctcp)) return DS_ERR_CODE_ERR;
  return DS_ERR_CODE_OK;
}


DS_ERR_CODE CTP_get_ctp_from_dssc(const DS_StreamConfig* p_stream_config, CommonTlmPacket* ctp)
{
  const uint16_t packet_len = EB90_FRAME_get_packet_length_from_dssc(p_stream_config);

  if (packet_len > CTP_MAX_LEN) return DS_ERR_CODE_ERR;

  // まず， 受信データ長だけコピーしてしまってから，アサーションする（効率のため）
  memcpy(&ctp->packet, EB90_FRAME_get_packet_head_from_dssc(p_stream_config), (size_t)packet_len);
  if (CTP_get_packet_len(ctp) != packet_len) return DS_ERR_CODE_ERR;
  if (!CTP_is_valid_packet(ctp)) return DS_ERR_CODE_ERR;
  return DS_ERR_CODE_OK;
}


DS_ERR_CODE CCP_get_ccp_from_dssc(const DS_StreamConfig* p_stream_config, CommonCmdPacket* ccp)
{
  const uint16_t packet_len = EB90_FRAME_get_packet_length_from_dssc(p_stream_config);

  if (packet_len > CCP_MAX_LEN) return DS_ERR_CODE_ERR;

  // まず， 受信データ長だけコピーしてしまってから，アサーションする（効率のため）
  memcpy(&ccp->packet, EB90_FRAME_get_packet_head_from_dssc(p_stream_config), (size_t)packet_len);
  if (CCP_get_packet_len(ccp) != packet_len) return DS_ERR_CODE_ERR;
  if (!CCP_is_valid_packet(ccp)) return DS_ERR_CODE_ERR;
  return DS_ERR_CODE_OK;
}

#pragma section
