/**
 * @file
 * @brief SpacePacket 類 と CommonTlmCmdPacket 類が相互参照するのを回避するためのヘッダ
 * @note  C2A Core のヘッダが知って良い情報はこれだけ
 */
#ifndef SPACE_PACKET_TYPEDEF_H_
#define SPACE_PACKET_TYPEDEF_H_

#include <stdint.h>

#define TSP_MAX_LEN   (432)   //!< TlmSpacePacket の最大パケット長．VCDU 分割しないならこれが最大値
#define CSP_MAX_LEN   (128)   //!< CmdSpacePacket の最大パケット長．値は適当においている

// TSP_MAX_LEN, CSP_MAX_LEN を再定義
#include <src_user/Settings/TlmCmd/Ccsds/space_packet_typedef_params.h>

#if TSP_MAX_LEN > CSP_MAX_LEN
#define SP_MAX_LEN    TSP_MAX_LEN
#else
#define SP_MAX_LEN    CSP_MAX_LEN
#endif

typedef struct SpacePacket SpacePacket;
typedef struct TlmSpacePacket TlmSpacePacket;
typedef struct CmdSpacePacket CmdSpacePacket;

/**
 * @struct SpacePacket
 * @brief  Space Packet
 */
struct SpacePacket
{
  uint8_t packet[SP_MAX_LEN];
};

/**
 * @struct TlmSpacePacket
 * @brief  Space Packet (テレメ用)
 * @note   C ではテンプレートが使えないため，別で定義する
 *         https://github.com/ut-issl/c2a-core/issues/204
 */
struct TlmSpacePacket
{
  uint8_t packet[TSP_MAX_LEN];
};

/**
 * @struct CmdSpacePacket
 * @brief  Space Packet (コマンド用)
 * @note   C ではテンプレートが使えないため，別で定義する
 *         https://github.com/ut-issl/c2a-core/issues/204
 */
struct CmdSpacePacket
{
  uint8_t packet[CSP_MAX_LEN];
};

#endif
