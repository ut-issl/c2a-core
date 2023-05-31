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
 * @brief HP テレメを生成
 * @note  Cmd_TG_GENERATE_TLM の Destination Flags が High Priority Realtime Telemetry のエイリアス
 */
CCP_CmdRet Cmd_TG_GENERATE_HP_TLM(const CommonCmdPacket* packet);

/**
 * @brief RT テレメを生成
 * @note  Cmd_TG_GENERATE_TLM の Destination Flags が Realtime Telemetry のエイリアス
 */
CCP_CmdRet Cmd_TG_GENERATE_RT_TLM(const CommonCmdPacket* packet);

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
 * @brief HP テレメとして 2nd OBC のテレメを転送
 * @note  Cmd_TG_FORWARD_TLM の Destination Flags が High Priority Realtime Telemetry のエイリアス
 */
CCP_CmdRet Cmd_TG_FORWARD_AS_HP_TLM(const CommonCmdPacket* packet);

/**
 * @brief RT テレメとして 2nd OBC のテレメを転送
 * @note  Cmd_TG_FORWARD_TLM の Destination Flags が Realtime Telemetry のエイリアス
 */
CCP_CmdRet Cmd_TG_FORWARD_AS_RT_TLM(const CommonCmdPacket* packet);

/**
 * @brief ST テレメとして 2nd OBC のテレメを転送
 * @note  Cmd_TG_FORWARD_TLM の Destination Flags が Stored Telemetry のエイリアス
 * @note  Data Recorder のどのパーティションに配送されるかも引数で受ける
 */
CCP_CmdRet Cmd_TG_FORWARD_AS_ST_TLM(const CommonCmdPacket* packet);

#endif
