#pragma section REPRO
/**
 * @file
 * @brief  EH の テスト用 Rule
 */
#include "event_handler_rules.h"
#include <src_core/System/EventManager/event_handler.h>
#include "../../../TlmCmd/block_command_definitions.h"

void EH_load_rule_test(void)
{
  EH_RuleSettings settings;

  // single
  settings.event.group = EL_GROUP_TEST_EH;
  settings.event.local = 0;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_SINGLE;
  settings.condition.count_threshold = 0;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 1;
  EH_register_rule(EH_RULE_TEST0, &settings);

  // single, err_level マッチなし
  settings.event.group = EL_GROUP_TEST_EH;
  settings.event.local = 1;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 0;
  settings.condition.type = EH_RESPONSE_CONDITION_SINGLE;
  settings.condition.count_threshold = 0;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 1;
  EH_register_rule(EH_RULE_TEST1, &settings);

  // continuous
  settings.event.group = EL_GROUP_TEST_EH;
  settings.event.local = 2;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 3;
  settings.condition.time_threshold_ms = 30 * 1000;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 1;
  EH_register_rule(EH_RULE_TEST2, &settings);

  // culmulative
  settings.event.group = EL_GROUP_TEST_EH;
  settings.event.local = 3;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 3;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 1;
  EH_register_rule(EH_RULE_TEST3, &settings);

  // single, EH_RULE_TEST0 と 同時 (デフォルト inactive)
  settings.event.group = EL_GROUP_TEST_EH;
  settings.event.local = 0;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_SINGLE;
  settings.condition.count_threshold = 1;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TEST4, &settings);

  // 多段 Lv.2, Lv.1 は EH_RULE_TEST0 (デフォルト inactive)
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_TEST0;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 2;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TEST5, &settings);

  // 多段 Lv.3 (デフォルト inactive)
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_TEST5;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 2;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_TEST_EH_RESPOND;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TEST6, &settings);
}

#pragma section
