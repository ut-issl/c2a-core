#ifndef TELEMETRY_GENERATOR_H_
#define TELEMETRY_GENERATOR_H_

#include "common_cmd_packet.h"

// deprecated
CCP_CmdRet Cmd_GENERATE_TLM(const CommonCmdPacket* packet);

/**
 * @brief 自身の OBC のテレメを生成
 */
CCP_CmdRet Cmd_TG_GENERATE_TLM(const CommonCmdPacket* packet);

/**
 * @brief HK テレメを生成
 * @note  Cmd_TG_GENERATE_TLM の Destination Flags が Housekeeping Telemetry のエイリアス
 */
CCP_CmdRet Cmd_TG_GENERATE_HK_TLM(const CommonCmdPacket* packet);

/**
 * @brief MS テレメを生成
 * @note  Cmd_TG_GENERATE_TLM の Destination Flags が Mission Telemetry のエイリアス
 */
CCP_CmdRet Cmd_TG_GENERATE_MS_TLM(const CommonCmdPacket* packet);

/**
 * @brief ST テレメを生成
 * @note  Cmd_TG_GENERATE_TLM の Destination Flags が Stored Telemetry のエイリアス
 * @note  Data Recorder のどのパーティションに配送されるかも引数で受ける
 */
CCP_CmdRet Cmd_TG_GENERATE_ST_TLM(const CommonCmdPacket* packet);

/**
 * @brief 2nd OBC のテレメを転送
 */
CCP_CmdRet Cmd_TG_FORWARD_TLM(const CommonCmdPacket* packet);

/**
 * @brief HK テレメとして 2nd OBC のテレメを転送
 * @note  Cmd_TG_FORWARD_TLM の Destination Flags が Housekeeping Telemetry のエイリアス
 */
CCP_CmdRet Cmd_TG_FORWARD_AS_HK_TLM(const CommonCmdPacket* packet);

/**
 * @brief MS テレメとして 2nd OBC のテレメを転送
 * @note  Cmd_TG_FORWARD_TLM の Destination Flags が Mission Telemetry のエイリアス
 */
CCP_CmdRet Cmd_TG_FORWARD_AS_MS_TLM(const CommonCmdPacket* packet);

/**
 * @brief ST テレメとして 2nd OBC のテレメを転送
 * @note  Cmd_TG_FORWARD_TLM の Destination Flags が Stored Telemetry のエイリアス
 * @note  Data Recorder のどのパーティションに配送されるかも引数で受ける
 */
CCP_CmdRet Cmd_TG_FORWARD_AS_ST_TLM(const CommonCmdPacket* packet);

/**
 * @brief 2nd OBC のテレメを生成したのち MS テレメとして転送する
 */
CCP_CmdRet Cmd_TG_GENERATE_2ND_OBC_MS_TLM(const CommonCmdPacket* packet);

#endif
