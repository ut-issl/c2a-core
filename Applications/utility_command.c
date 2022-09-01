#pragma section REPRO
#include "utility_command.h"
#include "../System/ApplicationManager/app_info.h"
#include "../IfWrapper/uart.h"
#include <src_user/Settings/port_config.h>
#include <string.h>                     // for memcpy
#include "../TlmCmd/common_cmd_packet_util.h"


static UtilityCommand utility_command_;
const UtilityCommand* const utility_command = &utility_command_;

static void UTIL_CMD_init_(void);
static void UTIL_CMD_reset_(void);
static void UTIL_CMD_add_(unsigned char add_size, const unsigned char* cmd);
static int UTIL_CMD_send_(unsigned char ch);

// 汎用コマンドここから
AppInfo UTIL_CMD_create_app(void)
{
  return AI_create_app_info("util_cmd", UTIL_CMD_init_, NULL);
}

static void UTIL_CMD_init_(void)
{
  int i;
  for (i = 0; i < UTIL_CMD_SIZE_MAX; i++)
  {
    utility_command_.util_cmd_buffer[i] = 0;
  }
  utility_command_.cmd_size = 0;
  utility_command_.pointer = 0;

  // UART送信用のダミー構造体．実際に使用されるのはchのみ
  utility_command_.uart_config_dummy.ch = 0;
  utility_command_.uart_config_dummy.baudrate = 115200;
  utility_command_.uart_config_dummy.parity_settings = PARITY_SETTINGS_NONE;
  utility_command_.uart_config_dummy.data_length = UART_DATA_LENGTH_8BIT;
  utility_command_.uart_config_dummy.stop_bit = UART_STOP_BIT_1BIT;
}

static void UTIL_CMD_reset_(void)
{
  int i;
  for (i = 0; i < UTIL_CMD_SIZE_MAX; i++)
  {
    utility_command_.util_cmd_buffer[i] = 0;
  }
  utility_command_.cmd_size = 0;
  utility_command_.pointer = 0;
}

static void UTIL_CMD_add_(unsigned char add_size, const unsigned char* cmd)
{
  // サイズが適合していれば，バッファの後端に加える
  if (utility_command_.pointer + add_size < UTIL_CMD_SIZE_MAX)
  {
    memcpy(&utility_command_.util_cmd_buffer[utility_command_.pointer], cmd, (size_t)add_size);
    utility_command_.pointer += add_size;
    utility_command_.cmd_size = utility_command_.pointer;
  }
}

// ここの対応関係をうまく整理する
static int UTIL_CMD_send_(unsigned char ch)
{
  int ret = -100;
  if (ch < PORT_CH_MAX_UART_PORT)
  {
    utility_command_.uart_config_dummy.ch = ch;
    ret = UART_tx(&(utility_command_.uart_config_dummy), &(utility_command_.util_cmd_buffer[0]), (int)utility_command_.cmd_size);
  }
  if (ret != 0)
  {
    // UART_ERR_CODEを保持
    utility_command_.uart_err_code = ret;
  }
  return ret;
}

CCP_CmdRet Cmd_UTIL_CMD_ADD(const CommonCmdPacket* packet)
{
  unsigned char size = CCP_get_param_head(packet)[0];
  if (CCP_get_param_len(packet) != 21)
  {
    // パラメータ長確認(21Bytes)
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
  }
  if (size <= 20)
  {
    // 範囲内なら該当のものを追加
    if (utility_command_.pointer + size < UTIL_CMD_SIZE_MAX)
    {
      UTIL_CMD_add_(size, &CCP_get_param_head(packet)[1]);
    }
     else
      {
        return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
      }
  }
  else
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_UTIL_CMD_SEND(const CommonCmdPacket* packet)
{
  unsigned char uart_ch = CCP_get_param_head(packet)[0];
  int ret;

  if (uart_ch < PORT_CH_MAX_UART_PORT)
  {
    // 範囲内なら該当のものを送信
    ret = UTIL_CMD_send_(uart_ch);
    if (ret != 0)
    {
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
    }
  }
  else
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_UTIL_CMD_RESET(const CommonCmdPacket* packet)
{
  (void)packet;
  UTIL_CMD_reset_();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
