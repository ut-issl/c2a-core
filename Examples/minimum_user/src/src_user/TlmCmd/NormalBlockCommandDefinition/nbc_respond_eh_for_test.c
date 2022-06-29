#pragma section REPRO
/**
 * @file
 * @brief test用． EH のテストで EH が発火したことを検知する
 */
#include "nbc_header.h"

#include <src_core/TlmCmd/block_command_loader.h>

#include "../command_definitions.h"
#include "../block_command_definitions.h"
#include <src_core/System/EventManager/event_logger.h>

typedef enum
{
  TEST_EH_EL_LOCAL_ID_RESPOND = 0
} TEST_EH_EL_LOCAL_ID;

void BCL_load_respond_eh_for_test(void)
{
  BCL_tool_prepare_param_uint32(EL_GROUP_TEST_EH_RESPOND);
  BCL_tool_prepare_param_uint32(TEST_EH_EL_LOCAL_ID_RESPOND);
  BCL_tool_prepare_param_uint8(EL_ERROR_LEVEL_LOW);
  BCL_tool_prepare_param_uint32(0);
  BCL_tool_register_cmd(1, Cmd_CODE_EL_RECORD_EVENT);
}

#pragma section
