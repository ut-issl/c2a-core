/**
 * @file
 * @brief  AOBC の DI と AOBC の cmd dispatcher
 * @note   C2A 間通信のデモ用であり， AOBC も C2A 搭載を仮定する
 */
#ifndef DI_AOBC_H_
#define DI_AOBC_H_

#include "../../Drivers/Aocs/aobc.h"
#include "../../Drivers/Aocs/aobc_command_definitions.h"
#include "../../Drivers/Aocs/aobc_telemetry_definitions.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/command_dispatcher.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

extern const AOBC_Driver* const aobc_driver;            //!< AOBC driver
extern const CommandDispatcher* const DI_AOBC_cdis;     //!< AOBC cmd dispatcher

/**
 * @brief  AOBC update アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo DI_AOBC_update(void);

/**
 * @brief  AOBC cmd dispatcher アプリ生成関数
 * @param  void
 * @return void
 */
AppInfo DI_AOBC_cmd_dispatcher(void);

/**
 * @brief  CommonCmdPacket を AOBC に送信する
 * @note   AOBC が自身のコマンドと解釈できるよう，Execution Type を上書きするため， packet を const cast する．[TODO] const cast やめたい
 * @note   この関数を呼んでも良いのは，user_packet_handler のみ！
 * @param  packet: CommonCmdPacket packet
 * @retval CCP_CmdRet{CCP_EXEC_SUCCESS, *}: 無事に転送された
 * @retval それ以外: 転送失敗（DS_CMD_ERR_CODE を CCP_EXEC_STS に変換して返す．詳細エラーは DriverSuper を参照）
 */
CCP_CmdRet DI_AOBC_dispatch_command(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_DI_AOBC_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_DI_AOBC_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet);

#endif
