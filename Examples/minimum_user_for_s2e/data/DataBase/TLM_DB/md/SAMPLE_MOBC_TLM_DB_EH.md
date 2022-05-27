# SAMPLE_MOBC_TLM_DB_EH

Name|Type|Exp.|Octet Pos.|bit Pos.|bit Len.|HEX|Status|a0|a1|a2|a3|a4|a5|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
PH.VER|uint16_t||0|0|3|||||||||||
PH.TYPE|||0|3|1|||||||||||
PH.SH_FLAG|||0|4|1|||||||||||
PH.APID|||0|5|11|||||||||||
PH.SEQ_FLAG|uint16_t||2|0|2|||||||||||
PH.SEQ_COUNT|||2|2|14|||||||||||
PH.PACKET_LEN|uint16_t||4|0|16|||||||||||
SH.VER|uint8_t||6|0|8|||||||||||
SH.TI|uint32_t||7|0|32|||||||||||
SH.TLM_ID|uint8_t||11|0|8|o||||||||||
SH.GLOBAL_TIME|double||12|0|64|||||||||||
SH.ON_BOARD_SUBNET_TIME|uint32_t||20|0|32|||||||||||
SH.DEST_FLAGS|uint8_t||24|0|8|o||||||||||
SH.DR_PARTITION|uint8_t||25|0|8|||||||||||
EVENT_UTIL.IS_ENABLED_EH_EXECUTION|uint8_t|event_utility->is_enabled_eh_execution|26|0|8||2.4|||||||||
EL_EVENT_COUNTER.COUNTER_TOTAL|uint32_t|event_handler->el_event_counter.counter_total|27|0|32|||||||||||
EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_HIGH|uint32_t|event_handler->el_event_counter.counters[EL_ERROR_LEVEL_HIGH]|31|0|32|||||||||||
EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_MIDDLE|uint32_t|event_handler->el_event_counter.counters[EL_ERROR_LEVEL_MIDDLE]|35|0|32|||||||||||
EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_LOW|uint32_t|event_handler->el_event_counter.counters[EL_ERROR_LEVEL_LOW]|39|0|32|||||||||||
EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_EL|uint32_t|event_handler->el_event_counter.counters[EL_ERROR_LEVEL_EL]|43|0|32|||||||||||
EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_EH|uint32_t|event_handler->el_event_counter.counters[EL_ERROR_LEVEL_EH]|47|0|32|||||||||||
LOG_TABLE.RESPOND_COUNTER|uint32_t|event_handler->log_table.respond_counter|51|0|32|||||||||||
LOG_TABLE.LOG_WP|uint16_t|event_handler->log_table.log_wp|55|0|16|||||||||||
EXEC_SETTINGS.MAX_RESPONSE_NUM|uint8_t|event_handler->exec_settings.max_response_num|57|0|8|||||||||||
EXEC_SETTINGS.MAX_CHECK_EVENT_NUM|uint16_t|event_handler->exec_settings.max_check_event_num|58|0|16|||||||||||
EXEC_SETTINGS.MAX_MULTI_LEVEL_NUM|uint8_t|event_handler->exec_settings.max_multi_level_num|60|0|8|||||||||||
REG_FROM_CMD.RULE_ID|uint8_t|(uint8_t)event_handler->reg_from_cmd.rule_id|61|0|8|||||||||||
REG_FROM_CMD.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)event_handler->reg_from_cmd.settings.event.group|62|0|8|||||||||||
REG_FROM_CMD.SETTINGS.EVENT.LOCAL|uint32_t|event_handler->reg_from_cmd.settings.event.local|63|0|32|||||||||||
REG_FROM_CMD.SETTINGS.EVENT.ERR_LEVEL|uint8_t||67|0|3||5.1|||||||||
REG_FROM_CMD.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||67|3|1||2.11|||||||||
REG_FROM_CMD.SETTINGS.IS_ACTIVE|||67|4|1||2.8|||||||||
REG_FROM_CMD.SETTINGS.CONDITION.TYPE|||67|5|3||3.3||||||||1 bitあまりあり|
REG_FROM_CMD.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|event_handler->reg_from_cmd.settings.condition.count_threshold|68|0|16|||||||||||
REG_FROM_CMD.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|event_handler->reg_from_cmd.settings.condition.time_threshold_ms|70|0|32|||||||||||
REG_FROM_CMD.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)event_handler->reg_from_cmd.settings.deploy_bct_id|74|0|16|||||||||||
REG_FROM_CMD.REGISTER_ACK|uint8_t|(uint8_t)event_handler->reg_from_cmd.register_ack|76|0|8|||||||||||
TLM_INFO.RULE.PAGE_NO|uint8_t|event_handler->tlm_info.rule.page_no|77|0|8|||||||||||
TLM_INFO.RULE.TARGET_RULE_ID|uint8_t|(uint8_t)event_handler->tlm_info.rule.target_rule_id|78|0|8|||||||||||
TLM_INFO.RULE_SORTED_INDEX.PAGE_NO|uint8_t|event_handler->tlm_info.rule_sorted_index.page_no|79|0|8|||||||||||
TLM_INFO.LOG.PAGE_NO|uint8_t|event_handler->tlm_info.log.page_no|80|0|8|||||||||||
TARTGET_RULE.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.event.group|81|0|8|||||||||||
TARTGET_RULE.SETTINGS.EVENT.LOCAL|uint32_t|event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.event.local|82|0|32|||||||||||
TARTGET_RULE.SETTINGS.EVENT.ERR_LEVEL|uint8_t||86|0|3||5.1|||||||||
TARTGET_RULE.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||86|3|1||2.11|||||||||
TARTGET_RULE.SETTINGS.IS_ACTIVE|||86|4|1||2.8|||||||||
TARTGET_RULE.SETTINGS.CONDITION.TYPE|||86|5|3||3.3||||||||1 bitあまりあり|
TARTGET_RULE.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.condition.count_threshold|87|0|16|||||||||||
TARTGET_RULE.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.condition.time_threshold_ms|89|0|32|||||||||||
TARTGET_RULE.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].settings.deploy_bct_id|93|0|16|||||||||||
TARTGET_RULE.COUNTER|uint16_t|event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].counter|95|0|16|||||||||||
TARTGET_RULE.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)event_handler->rule_table.rules[event_handler->tlm_info.rule.target_rule_id].last_event_time.total_cycle|97|0|32|||||||||||
