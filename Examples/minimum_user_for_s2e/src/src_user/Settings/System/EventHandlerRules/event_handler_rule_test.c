#pragma section REPRO
/**
 * @file   event_handler_rule_test.c
 * @brief  EH の テスト用 Rule
 */
#include "event_handler_rules.h"
#include <src_core/System/EventManager/event_handler.h>
#include "../../../CmdTlm/block_command_definitions.h"

void EH_load_rule_test(void)
{
  EH_RuleSettings settings;

  settings.event.group = EL_GROUP_TEST_AH;
  settings.event.local = 0;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_SINGLE;
  settings.condition.count_threshold = 0;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 1;

  EH_register_rule(EH_RULE_TEST0, &settings);


  settings.event.group = EL_GROUP_TEST_AH;
  settings.event.local = 1;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 0;
  settings.condition.type = EH_RESPONSE_CONDITION_SINGLE;
  settings.condition.count_threshold = 0;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 1;

  EH_register_rule(EH_RULE_TEST1, &settings);


  settings.event.group = EL_GROUP_TEST_AH;
  settings.event.local = 2;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 3;
  settings.condition.time_threshold_ms = 30 * 1000;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 1;

  EH_register_rule(EH_RULE_TEST2, &settings);


  settings.event.group = EL_GROUP_TEST_AH;
  settings.event.local = 3;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 3;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 1;

  EH_register_rule(EH_RULE_TEST3, &settings);
}

#pragma section
