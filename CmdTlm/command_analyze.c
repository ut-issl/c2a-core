#pragma section REPRO
/**
 * @file command_analyze.c
 * @brief  コマンドの実行・登録，コマンド関連処理
 */
#include "command_analyze.h"
#include <src_user/CmdTlm/command_definitions.h>
#include "../Library/print.h"
#include "../Library/endian_memcpy.h"

static CommandAnalyze command_analyze_;
const CommandAnalyze* const command_analyze = &command_analyze_;


static void CA_initialize_cmd_table_(void);


void CA_initialize(void)
{
  // cmd_table_を初期化
  CA_initialize_cmd_table_();

  if (CA_MAX_CMDS <= Cmd_CODE_MAX)
  {
    Printf("CA: init error!!!\n");
    return;
  }

  CA_load_cmd_table(command_analyze_.cmd_table);
}

static void CA_initialize_cmd_table_(void)
{
  int i;
  for (i = 0; i < CA_MAX_CMDS; ++i)
  {
    command_analyze_.cmd_table[i].cmd_func = NULL;
    command_analyze_.cmd_table[i].param_len_type = CA_CMD_PARAM_LEN_TYPE_UNKNOWN;
    command_analyze_.cmd_table[i].param_len = 0;
  }


  // ページ番号の初期値は0に設定
  command_analyze_.tlm_page_no = 0;
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

CA_ACK CA_ckeck_cmd_param_len(CMD_CODE cmd_code, uint16_t param_len)
{
  switch (command_analyze->cmd_table[cmd_code].param_len_type)
  {
  case CA_CMD_PARAM_LEN_TYPE_FIXED:
    if (param_len != command_analyze->cmd_table[cmd_code].param_len) return CA_ACK_ERR;
    break;
  case CA_CMD_PARAM_LEN_TYPE_LOWER_LIMIT:
    if (param_len < command_analyze->cmd_table[cmd_code].param_len || param_len > CCP_get_max_param_len())
    {
      return CA_ACK_ERR;
    }
    break;
  default:
    // CA_CMD_PARAM_LEN_TYPE_UNKNOWN
    // なにもチェックしない
    break;
  }

  return CA_ACK_OK;
}

CCP_EXEC_STS Cmd_CA_REGISTER_CMD(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint16_t index;
  uint32_t cmd_func;
  CA_CMD_PARAM_LEN_TYPE param_len_type;
  uint16_t param_len;

  endian_memcpy(&index, param, 2);
  endian_memcpy(&cmd_func, param + 2, 4);
  param_len_type = (CA_CMD_PARAM_LEN_TYPE)param[6];
  endian_memcpy(&param_len, &param[7], 2);

  if (index >= CA_MAX_CMDS)
  {
    // 登録指定位置がコマンド数上限を超えている場合は異常判定
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  command_analyze_.cmd_table[index].cmd_func = (CCP_EXEC_STS (*)(const CTCP*))cmd_func;
  command_analyze_.cmd_table[index].param_len_type = param_len_type;
  command_analyze_.cmd_table[index].param_len = param_len;

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
