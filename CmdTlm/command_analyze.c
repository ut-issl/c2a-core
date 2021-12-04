#pragma section REPRO
/**
 * @file command_analyze.c
 * @brief  コマンドの実行・登録，コマンド関連処理
 */
#include "command_analyze.h"
#include <src_user/CmdTlm/command_definitions.h>
#include "../Library/print.h"
#include "../Library/endian_memcpy.h"
#include <string.h>

/**
 * @brief  コマンドパラメタのサイズ情報を取得
 * @param cmd_code: コマンドID
 * @param n: N番目の引数 （0起算）
 * @return CA_PARAM_SIZE_TYPE
 * @note   不正な引数の場合は CA_PARAM_SIZE_TYPE_NONE を返す
 */
static CA_PARAM_SIZE_TYPE CA_get_param_size_type_(CMD_CODE cmd_code, uint8_t n);

static CommandAnalyze command_analyze_;
const CommandAnalyze* const command_analyze = &command_analyze_;


void CA_initialize(void)
{
  int i;

  if (CA_MAX_CMDS <= Cmd_CODE_MAX)
  {
    Printf("CA: init error!!!\n");
    return;
  }

  memset(&command_analyze_, 0x00, sizeof(CommandAnalyze));
  for (i = 0; i < CA_MAX_CMDS; ++i)
  {
    command_analyze_.cmd_table[i].cmd_func = NULL;
  }

  CA_load_cmd_table(command_analyze_.cmd_table);
}

CCP_EXEC_STS CA_execute_cmd(const CTCP* packet)
{
  CMD_CODE cmd_code;
  CCP_EXEC_STS (*cmd_func)(const CTCP*) = NULL;

  cmd_code = CCP_get_id(packet);
  cmd_func = command_analyze->cmd_table[cmd_code].cmd_func;

  if (cmd_func != NULL)
  {
    // ここで最低限のパラメタ長チェックをするが， bct_id_t など，内部定義を使っているものは各コマンド内部でもアサーションすること
    uint16_t param_len = CCP_get_param_len(packet);
    if (CA_ckeck_cmd_param_len(cmd_code, param_len) != CA_ACK_OK) return CCP_EXEC_ILLEGAL_LENGTH;

    return cmd_func(packet);
  }
  else
  {
    return CCP_EXEC_CMD_NOT_DEFINED;
  }
}

uint8_t CA_get_cmd_param_num(CMD_CODE cmd_code)
{
  uint8_t num;
  if (cmd_code >= CA_MAX_CMDS) return 0;

  for (num = 0; num < CA_MAX_CMD_PARAM_NUM; ++num)
  {
    if (CA_get_param_size_type_(cmd_code, num) == CA_PARAM_SIZE_TYPE_NONE)
    {
      return num;
    }
  }
  return CA_MAX_CMD_PARAM_NUM;
}

uint8_t CA_get_cmd_param_size(CMD_CODE cmd_code, uint8_t n)
{
  switch (CA_get_param_size_type_(cmd_code, n))
  {
    case CA_PARAM_SIZE_TYPE_NONE:
    case CA_PARAM_SIZE_TYPE_RAW:
      return 0;
    case CA_PARAM_SIZE_TYPE_1BYTE:
      return 1;
      break;
    case CA_PARAM_SIZE_TYPE_2BYTE:
      return 2;
      break;
    case CA_PARAM_SIZE_TYPE_4BYTE:
      return 4;
      break;
    case CA_PARAM_SIZE_TYPE_8BYTE:
      return 8;
      break;
    default:
      return 0;     // 不正
  }
}

uint16_t CA_get_cmd_param_min_len(CMD_CODE cmd_code)
{
  uint8_t i;
  uint16_t min_len = 0;

  for (i = 0; i < CA_MAX_CMD_PARAM_NUM; ++i)
  {
    uint8_t param_size = CA_get_cmd_param_size(cmd_code, i);
    if (param_size == 0) return min_len;
    min_len += param_size;
  }
  return min_len;
}

CA_ACK CA_ckeck_cmd_param_len(CMD_CODE cmd_code, uint16_t param_len)
{
  if (cmd_code >= CA_MAX_CMDS) return CA_ACK_ERR;

  if (CA_has_raw_param(cmd_code))
  {
    if (param_len < CA_get_cmd_param_min_len(cmd_code) || param_len > CCP_get_max_param_len())
    {
      return CA_ACK_ERR;
    }
    return CA_ACK_OK;
  }
  else
  {
    return (CA_get_cmd_param_min_len(cmd_code) == param_len) ? CA_ACK_OK : CA_ACK_ERR;
  }
}

int CA_has_raw_param(CMD_CODE cmd_code)
{
  uint8_t param_num = CA_get_cmd_param_num(cmd_code);
  if (param_num == 0) return 0;

  return (CA_get_param_size_type_(cmd_code, param_num - 1) == CA_PARAM_SIZE_TYPE_RAW) ? 1 : 0;
}

static CA_PARAM_SIZE_TYPE CA_get_param_size_type_(CMD_CODE cmd_code, uint8_t n)
{
  if (n >= CA_MAX_CMD_PARAM_NUM) return CA_PARAM_SIZE_TYPE_NONE;
  if (cmd_code >= CA_MAX_CMDS) return CA_PARAM_SIZE_TYPE_NONE;
  if (n % 2)
  {
    return (CA_PARAM_SIZE_TYPE)command_analyze->cmd_table[cmd_code].param_size_infos[n / 2].bit.second;
  }
  else
  {
    return (CA_PARAM_SIZE_TYPE)command_analyze->cmd_table[cmd_code].param_size_infos[n / 2].bit.first;
  }
}

CCP_EXEC_STS Cmd_CA_REGISTER_CMD(const CTCP* packet)
{
  // FIXME pytest, tlm更新
  (void)packet;
  // const uint8_t* param = CCP_get_param_head(packet);
  // uint16_t index;
  // uint32_t cmd_func;
  // CA_CMD_PARAM_LEN_TYPE param_len_type;
  // uint16_t param_len;

  // endian_memcpy(&index, param, 2);
  // endian_memcpy(&cmd_func, param + 2, 4);
  // param_len_type = (CA_CMD_PARAM_LEN_TYPE)param[6];
  // endian_memcpy(&param_len, &param[7], 2);

  // if (index >= CA_MAX_CMDS)
  // {
  //   // 登録指定位置がコマンド数上限を超えている場合は異常判定
  //   return CCP_EXEC_ILLEGAL_PARAMETER;
  // }

  // command_analyze_.cmd_table[index].cmd_func = (CCP_EXEC_STS (*)(const CTCP*))cmd_func;
  // command_analyze_.cmd_table[index].param_len_type = param_len_type;
  // command_analyze_.cmd_table[index].param_len = param_len;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_CA_SET_PAGE_FOR_TLM(const CTCP* packet)
{
  uint8_t page;

  page = CCP_get_param_head(packet)[0];
  if (page >= CA_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  command_analyze_.tlm_page_no = page;
  return CCP_EXEC_SUCCESS;
}

#pragma section
