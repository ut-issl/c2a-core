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

DS_ERR_CODE CTCP_get_ctcp_from_dssc(const DS_StreamConfig* p_stream_config, CommonTlmCmdPacket* received_packet)
{
  const uint16_t packet_len = EB90_FRAME_get_packet_length_from_dssc(p_stream_config);

  if (packet_len > CTCP_MAX_LEN) return DS_ERR_CODE_ERR;

  // まず， 受信データ長だけコピーしてしまってから，アサーションする（効率のため）
  memcpy(&received_packet->packet, EB90_FRAME_get_packet_head_from_dssc(p_stream_config), (size_t)packet_len);
  if (CTCP_get_packet_len(received_packet) != packet_len) return DS_ERR_CODE_ERR;
  if (!CTCP_is_valid_packet(received_packet)) return DS_ERR_CODE_ERR;
  return DS_ERR_CODE_OK;
}


DS_ERR_CODE CTP_get_ctp_from_dssc(const DS_StreamConfig* p_stream_config, CommonTlmPacket* received_packet)
{
  const uint16_t packet_len = EB90_FRAME_get_packet_length_from_dssc(p_stream_config);

  if (packet_len > CTP_MAX_LEN) return DS_ERR_CODE_ERR;

  // まず， 受信データ長だけコピーしてしまってから，アサーションする（効率のため）
  memcpy(&received_packet->packet, EB90_FRAME_get_packet_head_from_dssc(p_stream_config), (size_t)packet_len);
  if (CTP_get_packet_len(received_packet) != packet_len) return DS_ERR_CODE_ERR;
  if (!CTP_is_valid_packet(received_packet)) return DS_ERR_CODE_ERR;
  return DS_ERR_CODE_OK;
}


DS_ERR_CODE CCP_get_ccp_from_dssc(const DS_StreamConfig* p_stream_config, CommonCmdPacket* received_packet)
{
  const uint16_t packet_len = EB90_FRAME_get_packet_length_from_dssc(p_stream_config);

  if (packet_len > CCP_MAX_LEN) return DS_ERR_CODE_ERR;

  // まず， 受信データ長だけコピーしてしまってから，アサーションする（効率のため）
  memcpy(&received_packet->packet, EB90_FRAME_get_packet_head_from_dssc(p_stream_config), (size_t)packet_len);
  if (CCP_get_packet_len(received_packet) != packet_len) return DS_ERR_CODE_ERR;
  if (!CCP_is_valid_packet(received_packet)) return DS_ERR_CODE_ERR;
  return DS_ERR_CODE_OK;
}


DS_ERR_CODE CTCP_init_dssc(DS_StreamConfig* p_stream_config,
                           uint8_t* tx_frame_buffer,
                           int16_t tx_frame_buffer_size,
                           DS_ERR_CODE (*data_analyzer)(DS_StreamConfig* p_stream_config, void* p_driver))
{
  if (tx_frame_buffer_size < EB90_FRAME_HEADER_SIZE + CTCP_MAX_LEN + EB90_FRAME_FOOTER_SIZE)
  {
    return DS_ERR_CODE_ERR;
  }

  // 送信はする
  DSSC_set_tx_frame(p_stream_config, tx_frame_buffer);  // 送る直前に中身を memcpy する
  DSSC_set_tx_frame_buffer_size(p_stream_config, tx_frame_buffer_size);
  DSSC_set_tx_frame_size(p_stream_config, 0);           // 送る直前に値をセットする

  // 定期的な受信はする
  DSSC_set_rx_header(p_stream_config, EB90_FRAME_kStx, EB90_FRAME_STX_SIZE);
  DSSC_set_rx_footer(p_stream_config, EB90_FRAME_kEtx, EB90_FRAME_ETX_SIZE);
  DSSC_set_rx_frame_size(p_stream_config, -1);          // 可変
  DSSC_set_rx_framelength_pos(p_stream_config, EB90_FRAME_STX_SIZE);
  DSSC_set_rx_framelength_type_size(p_stream_config, 2);
  DSSC_set_rx_framelength_offset(p_stream_config,
                                 EB90_FRAME_HEADER_SIZE + EB90_FRAME_FOOTER_SIZE);
  DSSC_set_data_analyzer(p_stream_config, data_analyzer);

  // 定期 TLM の監視機能の有効化はここではしないので， Driver 側でやる
  // enable もここではしない

  return DS_ERR_CODE_OK;
}


DS_ERR_CODE CTCP_set_tx_frame_to_dssc(DS_StreamConfig* p_stream_config,
                                      const CommonTlmCmdPacket* send_packet)
{
  size_t pos;
  size_t size;
  uint16_t crc;
  uint16_t packet_len = CTCP_get_packet_len(send_packet);
  uint16_t frame_len = (uint16_t)(packet_len + EB90_FRAME_HEADER_SIZE + EB90_FRAME_FOOTER_SIZE);
  uint8_t* tx_frame = DSSC_get_tx_frame_as_non_const_pointer(p_stream_config);

  if (frame_len > DSSC_get_tx_frame_buffer_size(p_stream_config)) return DS_ERR_CODE_ERR;

  DSSC_set_tx_frame_size(p_stream_config, frame_len);

  pos  = 0;
  size = EB90_FRAME_STX_SIZE;
  memcpy(&(tx_frame[pos]), EB90_FRAME_kStx, size);
  pos += size;
  size = EB90_FRAME_LEN_SIZE;
  ENDIAN_memcpy(&(tx_frame[pos]), &packet_len, size);       // ここはエンディアンを気にする！
  pos += size;

  size = (size_t)packet_len;
  memcpy(&(tx_frame[pos]), send_packet->packet, size);
  pos += size;

  crc = EB90_FRAME_calc_crc(tx_frame + EB90_FRAME_HEADER_SIZE, pos - EB90_FRAME_HEADER_SIZE);
  size = EB90_FRAME_CRC_SIZE;
  ENDIAN_memcpy(&(tx_frame[pos]), &crc, size);       // ここはエンディアンを気にする！
  pos += size;
  size = EB90_FRAME_ETX_SIZE;
  memcpy(&(tx_frame[pos]), EB90_FRAME_kEtx, size);

  return DS_ERR_CODE_OK;
}


DS_ERR_CODE CTP_set_tx_frame_to_dssc(DS_StreamConfig* p_stream_config,
                                     const CommonTlmPacket* send_packet)
{
  const CommonTlmCmdPacket* ctcp = CTCP_convert_from_ctp(send_packet);
  if (ctcp == NULL) return DS_ERR_CODE_ERR;
  return CTCP_set_tx_frame_to_dssc(p_stream_config, ctcp);
}


DS_ERR_CODE CCP_set_tx_frame_to_dssc(DS_StreamConfig* p_stream_config,
                                     const CommonCmdPacket* send_packet)
{
  const CommonTlmCmdPacket* ctcp = CTCP_convert_from_ccp(send_packet);
  if (ctcp == NULL) return DS_ERR_CODE_ERR;
  return CTCP_set_tx_frame_to_dssc(p_stream_config, ctcp);
}

#pragma section
