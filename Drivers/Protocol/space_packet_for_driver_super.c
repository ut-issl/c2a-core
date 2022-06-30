#pragma section REPRO
/**
 * @file
 * @brief コンポ間通信などで標準的に使う ネットワーク層 の Space Packet
 * @note  ここでは，データリンク層は CCSDS ではなく EB90 Frame を使うことを想定
 * @note  データリンク層は DS_StreamConfig.data_link_layer_ で規定する
 * @note  packet 構造 などは TlmCmd/Ccsds/space_packet.h を参照のこと
 */

#include "space_packet_for_driver_super.h"

// FIXME: DS_StreamConfig.data_link_layer_ をちゃんと見る！




#pragma section
