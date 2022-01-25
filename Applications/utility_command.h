#ifndef UTILITY_COMMAND_H_
#define UTILITY_COMMAND_H_
// FIXME: このAppは現在管理されていない！
//        使用する前に確認すること

#include "../TlmCmd/common_tlm_cmd_packet.h"
#include "../IfWrapper/uart.h"
#include "../System/ApplicationManager/app_info.h"

#define UTIL_CMD_SIZE_MAX (256) // 256Bytes以上コマンドとして送ることはないはず．．．ICOUPS_PARAM_BLより長いし

// 汎用コマンド用構造体
typedef struct
{
  UART_Config uart_config_dummy;
  unsigned char util_cmd_buffer[UTIL_CMD_SIZE_MAX];
  unsigned int cmd_size; // 送信コマンドサイズ
  unsigned int pointer;
  int uart_err_code; // UTIL_CMD_send_でのエラー情報保持用
} UtilityCommand;

extern const UtilityCommand* const utility_command;

AppInfo UTIL_CMD_create_app(void);

// コマンド
CCP_EXEC_STS Cmd_UTIL_CMD_ADD(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_UTIL_CMD_SEND(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_UTIL_CMD_RESET(const CommonCmdPacket* packet);

#endif
