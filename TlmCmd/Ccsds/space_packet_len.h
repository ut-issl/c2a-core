/**
 * @file
 * @brief CCSDS で規定される Space Packet のパケット長情報のヘッダ
 * @note  space_packet.h と tlm_space_packet.h, cmd_space_packet.h の相互 include 回避のため，分離した
 */
#ifndef SPACE_PACKET_LEN_H_
#define SPACE_PACKET_LEN_H_

#define TSP_MAX_LEN   (432)   //!< TlmSpacePacket の最大パケット長．VCDU 分割しないならこれが最大値
#define CSP_MAX_LEN   (128)   //!< CmdSpacePacket の最大パケット長．値は適当においている

// TSP_MAX_LEN, CSP_MAX_LEN を再定義
#include <src_user/Settings/TlmCmd/Ccsds/space_packet_len_params.h>

#if TSP_MAX_LEN > CSP_MAX_LEN
#define SP_MAX_LEN    TSP_MAX_LEN
#else
#define SP_MAX_LEN    CSP_MAX_LEN
#endif

#endif
