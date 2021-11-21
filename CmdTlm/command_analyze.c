#pragma section REPRO
/**
 * @file command_analyze.c
 * @brief  �R�}���h�̎��s�E�o�^�C�R�}���h�֘A����
 */
#include "command_analyze.h"
#include <src_user/CmdTlm/command_definitions.h>
#include "../Library/print.h"
#include "../Library/endian_memcpy.h"

static CmdInfo cmd_table_[CMD_MAX_CMDS];
const CmdInfo* cmd_table;
static int CA_page_no_;
const int* CA_page_no;

static void CA_initialize_cmd_table_(void);


void CA_initialize(void)
{
  // cmd_table_��������
  CA_initialize_cmd_table_();

  if (CMD_MAX_CMDS <= Cmd_CODE_MAX)
  {
    Printf("CA: init error!!!\n");
    return;
  }

  CA_load_cmd_table(cmd_table_);
}

static void CA_initialize_cmd_table_(void)
{
  int i;
  for (i = 0; i < CMD_MAX_CMDS; ++i)
  {
    cmd_table_[i].cmd_func = NULL;
    cmd_table_[i].param_len_type = CA_CMD_PARAM_LEN_TYPE_UNKNOWN;
    cmd_table_[i].param_len = 0;
  }

  cmd_table = cmd_table_;

  // �y�[�W�ԍ��̏����l��0�ɐݒ�
  CA_page_no_ = 0;
  CA_page_no = &CA_page_no_;
}

CCP_EXEC_STS CA_execute_cmd(const CTCP* packet)
{
  CMD_CODE cmd_code;
  CCP_EXEC_STS (*cmd_func)(const CTCP*) = NULL;

  cmd_code = CCP_get_id(packet);
  cmd_func = cmd_table_[cmd_code].cmd_func;

  if (cmd_func != NULL)
  {
    // �����ōŒ���̃p�����^���`�F�b�N�����邪�C bct_id_t �ȂǁC������`���g���Ă�����̂͊e�R�}���h�����ł��A�T�[�V�������邱��
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
  switch (cmd_table_[cmd_code].param_len_type)
  {
  case CA_CMD_PARAM_LEN_TYPE_FIXED:
    if (param_len != cmd_table_[cmd_code].param_len) return CA_ACK_ERR;
    break;
  case CA_CMD_PARAM_LEN_TYPE_LOWER_LIMIT:
    if (param_len < cmd_table_[cmd_code].param_len || param_len > CCP_get_max_param_len())
    {
      return CA_ACK_ERR;
    }
    break;
  default:
    // CA_CMD_PARAM_LEN_TYPE_UNKNOWN
    // �Ȃɂ��`�F�b�N���Ȃ�
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

  if (index >= CMD_MAX_CMDS)
  {
    // �o�^�w��ʒu���R�}���h������𒴂��Ă���ꍇ�ُ͈픻��
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  cmd_table_[index].cmd_func = (CCP_EXEC_STS (*)(const CTCP*))cmd_func;
  cmd_table_[index].param_len_type = param_len_type;
  cmd_table_[index].param_len = param_len;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_CA_SET_PAGE_FOR_TLM(const CTCP* packet)
{
  uint8_t page;

  page = CCP_get_param_head(packet)[0];
  if (page >= CA_TLM_PAGE_MAX)
  {
    // �y�[�W�ԍ����R�}���h�e�[�u���͈͊O
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  CA_page_no_ = page;
  return CCP_EXEC_SUCCESS;
}

#pragma section