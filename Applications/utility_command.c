#pragma section REPRO
#include "utility_command.h"
#include "../System/ApplicationManager/app_info.h"
#include "../IfWrapper/uart.h"
#include <src_user/Settings/port_config.h>
#include <string.h>                     // for memcpy


static UtilityCommand utility_command_;
const UtilityCommand* const utility_command = &utility_command_;

static void UTIL_CMD_init_(void);
static void UTIL_CMD_reset_(void);
static void UTIL_CMD_add_(unsigned char add_size, const unsigned char* cmd);
static int UTIL_CMD_send_(unsigned char ch);

// �ėp�R�}���h��������
AppInfo UTIL_CMD_create_app(void)
{
  return create_app_info("util_cmd", UTIL_CMD_init_, NULL);
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

  // UART���M�p�̃_�~�[�\���́D���ۂɎg�p�����̂�ch�̂�
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
  // �T�C�Y���K�����Ă���΁C�o�b�t�@�̌�[�ɉ�����
  if (utility_command_.pointer + add_size < UTIL_CMD_SIZE_MAX)
  {
    memcpy(&utility_command_.util_cmd_buffer[utility_command_.pointer], cmd, (size_t)add_size);
    utility_command_.pointer += add_size;
    utility_command_.cmd_size = utility_command_.pointer;
  }
}

// �����̑Ή��֌W�����܂���������
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
    // UART_ERR_CODE��ێ�
    utility_command_.uart_err_code = ret;
  }
  return ret;
}

CCP_EXEC_STS Cmd_UTIL_CMD_ADD(const CTCP* packet)
{
  unsigned char size = CCP_get_param_head(packet)[0];
  if (CCP_get_param_len(packet) != 21)
  {
    // �p�����[�^���m�F(21Bytes)
    return CCP_EXEC_ILLEGAL_LENGTH;
  }
  if (size <= 20)
  {
    // �͈͓��Ȃ�Y���̂��̂�ǉ�
    if (utility_command_.pointer + size < UTIL_CMD_SIZE_MAX)
    {
      UTIL_CMD_add_(size, &CCP_get_param_head(packet)[1]);
    }
     else
      {
        return CCP_EXEC_ILLEGAL_PARAMETER;
      }
  }
  else
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_UTIL_CMD_SEND(const CTCP* packet)
{
  unsigned char uart_ch = CCP_get_param_head(packet)[0];
  int ret;

  if (uart_ch < PORT_CH_MAX_UART_PORT)
  {
    // �͈͓��Ȃ�Y���̂��̂𑗐M
    ret = UTIL_CMD_send_(uart_ch);
    if (ret != 0)
    {
      return CCP_EXEC_ILLEGAL_CONTEXT;
    }
  }
  else
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_UTIL_CMD_RESET(const CTCP* packet)
{
  (void)packet;
  UTIL_CMD_reset_();
  return CCP_EXEC_SUCCESS;
}

#pragma section