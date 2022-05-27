# SAMPLE_MOBC_TLM_DB_EH_RULE

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
TLM_INFO.RULE.PAGE_NO|uint8_t|event_handler->tlm_info.rule.page_no|26|0|8|||||||||||
RULE_TABLE.REGISTERED_RULE_NUM|uint16_t|event_handler->rule_table.registered_rule_num|27|0|16|||||||||||
RULE0.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[0 + offset].settings.event.group|29|0|8|||||||||||
RULE0.SETTINGS.EVENT.LOCAL|uint32_t|rules[0 + offset].settings.event.local|30|0|32|||||||||||
RULE0.SETTINGS.EVENT.ERR_LEVEL|uint8_t||34|0|3||5.1|||||||||
RULE0.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||34|3|1||2.11|||||||||
RULE0.SETTINGS.IS_ACTIVE|||34|4|1||2.8|||||||||
RULE0.SETTINGS.CONDITION.TYPE|||34|5|3||3.3|||||||||
RULE0.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[0 + offset].settings.condition.count_threshold|35|0|16|||||||||||
RULE0.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[0 + offset].settings.condition.time_threshold_ms|37|0|32|||||||||||
RULE0.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[0 + offset].settings.deploy_bct_id|41|0|16|||||||||||
RULE0.COUNTER|uint16_t|rules[0 + offset].counter|43|0|16|||||||||||
RULE0.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[0 + offset].last_event_time.total_cycle|45|0|32|||||||||||
RULE1.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[1 + offset].settings.event.group|49|0|8|||||||||||
RULE1.SETTINGS.EVENT.LOCAL|uint32_t|rules[1 + offset].settings.event.local|50|0|32|||||||||||
RULE1.SETTINGS.EVENT.ERR_LEVEL|uint8_t||54|0|3||5.1|||||||||
RULE1.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||54|3|1||2.11|||||||||
RULE1.SETTINGS.IS_ACTIVE|||54|4|1||2.8|||||||||
RULE1.SETTINGS.CONDITION.TYPE|||54|5|3||3.3|||||||||
RULE1.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[1 + offset].settings.condition.count_threshold|55|0|16|||||||||||
RULE1.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[1 + offset].settings.condition.time_threshold_ms|57|0|32|||||||||||
RULE1.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[1 + offset].settings.deploy_bct_id|61|0|16|||||||||||
RULE1.COUNTER|uint16_t|rules[1 + offset].counter|63|0|16|||||||||||
RULE1.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[1 + offset].last_event_time.total_cycle|65|0|32|||||||||||
RULE2.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[2 + offset].settings.event.group|69|0|8|||||||||||
RULE2.SETTINGS.EVENT.LOCAL|uint32_t|rules[2 + offset].settings.event.local|70|0|32|||||||||||
RULE2.SETTINGS.EVENT.ERR_LEVEL|uint8_t||74|0|3||5.1|||||||||
RULE2.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||74|3|1||2.11|||||||||
RULE2.SETTINGS.IS_ACTIVE|||74|4|1||2.8|||||||||
RULE2.SETTINGS.CONDITION.TYPE|||74|5|3||3.3|||||||||
RULE2.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[2 + offset].settings.condition.count_threshold|75|0|16|||||||||||
RULE2.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[2 + offset].settings.condition.time_threshold_ms|77|0|32|||||||||||
RULE2.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[2 + offset].settings.deploy_bct_id|81|0|16|||||||||||
RULE2.COUNTER|uint16_t|rules[2 + offset].counter|83|0|16|||||||||||
RULE2.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[2 + offset].last_event_time.total_cycle|85|0|32|||||||||||
RULE3.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[3 + offset].settings.event.group|89|0|8|||||||||||
RULE3.SETTINGS.EVENT.LOCAL|uint32_t|rules[3 + offset].settings.event.local|90|0|32|||||||||||
RULE3.SETTINGS.EVENT.ERR_LEVEL|uint8_t||94|0|3||5.1|||||||||
RULE3.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||94|3|1||2.11|||||||||
RULE3.SETTINGS.IS_ACTIVE|||94|4|1||2.8|||||||||
RULE3.SETTINGS.CONDITION.TYPE|||94|5|3||3.3|||||||||
RULE3.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[3 + offset].settings.condition.count_threshold|95|0|16|||||||||||
RULE3.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[3 + offset].settings.condition.time_threshold_ms|97|0|32|||||||||||
RULE3.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[3 + offset].settings.deploy_bct_id|101|0|16|||||||||||
RULE3.COUNTER|uint16_t|rules[3 + offset].counter|103|0|16|||||||||||
RULE3.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[3 + offset].last_event_time.total_cycle|105|0|32|||||||||||
RULE4.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[4 + offset].settings.event.group|109|0|8|||||||||||
RULE4.SETTINGS.EVENT.LOCAL|uint32_t|rules[4 + offset].settings.event.local|110|0|32|||||||||||
RULE4.SETTINGS.EVENT.ERR_LEVEL|uint8_t||114|0|3||5.1|||||||||
RULE4.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||114|3|1||2.11|||||||||
RULE4.SETTINGS.IS_ACTIVE|||114|4|1||2.8|||||||||
RULE4.SETTINGS.CONDITION.TYPE|||114|5|3||3.3|||||||||
RULE4.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[4 + offset].settings.condition.count_threshold|115|0|16|||||||||||
RULE4.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[4 + offset].settings.condition.time_threshold_ms|117|0|32|||||||||||
RULE4.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[4 + offset].settings.deploy_bct_id|121|0|16|||||||||||
RULE4.COUNTER|uint16_t|rules[4 + offset].counter|123|0|16|||||||||||
RULE4.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[4 + offset].last_event_time.total_cycle|125|0|32|||||||||||
RULE5.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[5 + offset].settings.event.group|129|0|8|||||||||||
RULE5.SETTINGS.EVENT.LOCAL|uint32_t|rules[5 + offset].settings.event.local|130|0|32|||||||||||
RULE5.SETTINGS.EVENT.ERR_LEVEL|uint8_t||134|0|3||5.1|||||||||
RULE5.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||134|3|1||2.11|||||||||
RULE5.SETTINGS.IS_ACTIVE|||134|4|1||2.8|||||||||
RULE5.SETTINGS.CONDITION.TYPE|||134|5|3||3.3|||||||||
RULE5.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[5 + offset].settings.condition.count_threshold|135|0|16|||||||||||
RULE5.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[5 + offset].settings.condition.time_threshold_ms|137|0|32|||||||||||
RULE5.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[5 + offset].settings.deploy_bct_id|141|0|16|||||||||||
RULE5.COUNTER|uint16_t|rules[5 + offset].counter|143|0|16|||||||||||
RULE5.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[5 + offset].last_event_time.total_cycle|145|0|32|||||||||||
RULE6.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[6 + offset].settings.event.group|149|0|8|||||||||||
RULE6.SETTINGS.EVENT.LOCAL|uint32_t|rules[6 + offset].settings.event.local|150|0|32|||||||||||
RULE6.SETTINGS.EVENT.ERR_LEVEL|uint8_t||154|0|3||5.1|||||||||
RULE6.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||154|3|1||2.11|||||||||
RULE6.SETTINGS.IS_ACTIVE|||154|4|1||2.8|||||||||
RULE6.SETTINGS.CONDITION.TYPE|||154|5|3||3.3|||||||||
RULE6.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[6 + offset].settings.condition.count_threshold|155|0|16|||||||||||
RULE6.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[6 + offset].settings.condition.time_threshold_ms|157|0|32|||||||||||
RULE6.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[6 + offset].settings.deploy_bct_id|161|0|16|||||||||||
RULE6.COUNTER|uint16_t|rules[6 + offset].counter|163|0|16|||||||||||
RULE6.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[6 + offset].last_event_time.total_cycle|165|0|32|||||||||||
RULE7.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[7 + offset].settings.event.group|169|0|8|||||||||||
RULE7.SETTINGS.EVENT.LOCAL|uint32_t|rules[7 + offset].settings.event.local|170|0|32|||||||||||
RULE7.SETTINGS.EVENT.ERR_LEVEL|uint8_t||174|0|3||5.1|||||||||
RULE7.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||174|3|1||2.11|||||||||
RULE7.SETTINGS.IS_ACTIVE|||174|4|1||2.8|||||||||
RULE7.SETTINGS.CONDITION.TYPE|||174|5|3||3.3|||||||||
RULE7.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[7 + offset].settings.condition.count_threshold|175|0|16|||||||||||
RULE7.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[7 + offset].settings.condition.time_threshold_ms|177|0|32|||||||||||
RULE7.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[7 + offset].settings.deploy_bct_id|181|0|16|||||||||||
RULE7.COUNTER|uint16_t|rules[7 + offset].counter|183|0|16|||||||||||
RULE7.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[7 + offset].last_event_time.total_cycle|185|0|32|||||||||||
RULE8.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[8 + offset].settings.event.group|189|0|8|||||||||||
RULE8.SETTINGS.EVENT.LOCAL|uint32_t|rules[8 + offset].settings.event.local|190|0|32|||||||||||
RULE8.SETTINGS.EVENT.ERR_LEVEL|uint8_t||194|0|3||5.1|||||||||
RULE8.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||194|3|1||2.11|||||||||
RULE8.SETTINGS.IS_ACTIVE|||194|4|1||2.8|||||||||
RULE8.SETTINGS.CONDITION.TYPE|||194|5|3||3.3|||||||||
RULE8.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[8 + offset].settings.condition.count_threshold|195|0|16|||||||||||
RULE8.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[8 + offset].settings.condition.time_threshold_ms|197|0|32|||||||||||
RULE8.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[8 + offset].settings.deploy_bct_id|201|0|16|||||||||||
RULE8.COUNTER|uint16_t|rules[8 + offset].counter|203|0|16|||||||||||
RULE8.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[8 + offset].last_event_time.total_cycle|205|0|32|||||||||||
RULE9.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[9 + offset].settings.event.group|209|0|8|||||||||||
RULE9.SETTINGS.EVENT.LOCAL|uint32_t|rules[9 + offset].settings.event.local|210|0|32|||||||||||
RULE9.SETTINGS.EVENT.ERR_LEVEL|uint8_t||214|0|3||5.1|||||||||
RULE9.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||214|3|1||2.11|||||||||
RULE9.SETTINGS.IS_ACTIVE|||214|4|1||2.8|||||||||
RULE9.SETTINGS.CONDITION.TYPE|||214|5|3||3.3|||||||||
RULE9.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[9 + offset].settings.condition.count_threshold|215|0|16|||||||||||
RULE9.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[9 + offset].settings.condition.time_threshold_ms|217|0|32|||||||||||
RULE9.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[9 + offset].settings.deploy_bct_id|221|0|16|||||||||||
RULE9.COUNTER|uint16_t|rules[9 + offset].counter|223|0|16|||||||||||
RULE9.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[9 + offset].last_event_time.total_cycle|225|0|32|||||||||||
RULE10.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[10 + offset].settings.event.group|229|0|8|||||||||||
RULE10.SETTINGS.EVENT.LOCAL|uint32_t|rules[10 + offset].settings.event.local|230|0|32|||||||||||
RULE10.SETTINGS.EVENT.ERR_LEVEL|uint8_t||234|0|3||5.1|||||||||
RULE10.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||234|3|1||2.11|||||||||
RULE10.SETTINGS.IS_ACTIVE|||234|4|1||2.8|||||||||
RULE10.SETTINGS.CONDITION.TYPE|||234|5|3||3.3|||||||||
RULE10.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[10 + offset].settings.condition.count_threshold|235|0|16|||||||||||
RULE10.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[10 + offset].settings.condition.time_threshold_ms|237|0|32|||||||||||
RULE10.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[10 + offset].settings.deploy_bct_id|241|0|16|||||||||||
RULE10.COUNTER|uint16_t|rules[10 + offset].counter|243|0|16|||||||||||
RULE10.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[10 + offset].last_event_time.total_cycle|245|0|32|||||||||||
RULE11.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[11 + offset].settings.event.group|249|0|8|||||||||||
RULE11.SETTINGS.EVENT.LOCAL|uint32_t|rules[11 + offset].settings.event.local|250|0|32|||||||||||
RULE11.SETTINGS.EVENT.ERR_LEVEL|uint8_t||254|0|3||5.1|||||||||
RULE11.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||254|3|1||2.11|||||||||
RULE11.SETTINGS.IS_ACTIVE|||254|4|1||2.8|||||||||
RULE11.SETTINGS.CONDITION.TYPE|||254|5|3||3.3|||||||||
RULE11.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[11 + offset].settings.condition.count_threshold|255|0|16|||||||||||
RULE11.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[11 + offset].settings.condition.time_threshold_ms|257|0|32|||||||||||
RULE11.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[11 + offset].settings.deploy_bct_id|261|0|16|||||||||||
RULE11.COUNTER|uint16_t|rules[11 + offset].counter|263|0|16|||||||||||
RULE11.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[11 + offset].last_event_time.total_cycle|265|0|32|||||||||||
RULE12.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[12 + offset].settings.event.group|269|0|8|||||||||||
RULE12.SETTINGS.EVENT.LOCAL|uint32_t|rules[12 + offset].settings.event.local|270|0|32|||||||||||
RULE12.SETTINGS.EVENT.ERR_LEVEL|uint8_t||274|0|3||5.1|||||||||
RULE12.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||274|3|1||2.11|||||||||
RULE12.SETTINGS.IS_ACTIVE|||274|4|1||2.8|||||||||
RULE12.SETTINGS.CONDITION.TYPE|||274|5|3||3.3|||||||||
RULE12.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[12 + offset].settings.condition.count_threshold|275|0|16|||||||||||
RULE12.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[12 + offset].settings.condition.time_threshold_ms|277|0|32|||||||||||
RULE12.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[12 + offset].settings.deploy_bct_id|281|0|16|||||||||||
RULE12.COUNTER|uint16_t|rules[12 + offset].counter|283|0|16|||||||||||
RULE12.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[12 + offset].last_event_time.total_cycle|285|0|32|||||||||||
RULE13.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[13 + offset].settings.event.group|289|0|8|||||||||||
RULE13.SETTINGS.EVENT.LOCAL|uint32_t|rules[13 + offset].settings.event.local|290|0|32|||||||||||
RULE13.SETTINGS.EVENT.ERR_LEVEL|uint8_t||294|0|3||5.1|||||||||
RULE13.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||294|3|1||2.11|||||||||
RULE13.SETTINGS.IS_ACTIVE|||294|4|1||2.8|||||||||
RULE13.SETTINGS.CONDITION.TYPE|||294|5|3||3.3|||||||||
RULE13.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[13 + offset].settings.condition.count_threshold|295|0|16|||||||||||
RULE13.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[13 + offset].settings.condition.time_threshold_ms|297|0|32|||||||||||
RULE13.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[13 + offset].settings.deploy_bct_id|301|0|16|||||||||||
RULE13.COUNTER|uint16_t|rules[13 + offset].counter|303|0|16|||||||||||
RULE13.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[13 + offset].last_event_time.total_cycle|305|0|32|||||||||||
RULE14.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[14 + offset].settings.event.group|309|0|8|||||||||||
RULE14.SETTINGS.EVENT.LOCAL|uint32_t|rules[14 + offset].settings.event.local|310|0|32|||||||||||
RULE14.SETTINGS.EVENT.ERR_LEVEL|uint8_t||314|0|3||5.1|||||||||
RULE14.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||314|3|1||2.11|||||||||
RULE14.SETTINGS.IS_ACTIVE|||314|4|1||2.8|||||||||
RULE14.SETTINGS.CONDITION.TYPE|||314|5|3||3.3|||||||||
RULE14.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[14 + offset].settings.condition.count_threshold|315|0|16|||||||||||
RULE14.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[14 + offset].settings.condition.time_threshold_ms|317|0|32|||||||||||
RULE14.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[14 + offset].settings.deploy_bct_id|321|0|16|||||||||||
RULE14.COUNTER|uint16_t|rules[14 + offset].counter|323|0|16|||||||||||
RULE14.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[14 + offset].last_event_time.total_cycle|325|0|32|||||||||||
RULE15.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[15 + offset].settings.event.group|329|0|8|||||||||||
RULE15.SETTINGS.EVENT.LOCAL|uint32_t|rules[15 + offset].settings.event.local|330|0|32|||||||||||
RULE15.SETTINGS.EVENT.ERR_LEVEL|uint8_t||334|0|3||5.1|||||||||
RULE15.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||334|3|1||2.11|||||||||
RULE15.SETTINGS.IS_ACTIVE|||334|4|1||2.8|||||||||
RULE15.SETTINGS.CONDITION.TYPE|||334|5|3||3.3|||||||||
RULE15.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[15 + offset].settings.condition.count_threshold|335|0|16|||||||||||
RULE15.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[15 + offset].settings.condition.time_threshold_ms|337|0|32|||||||||||
RULE15.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[15 + offset].settings.deploy_bct_id|341|0|16|||||||||||
RULE15.COUNTER|uint16_t|rules[15 + offset].counter|343|0|16|||||||||||
RULE15.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[15 + offset].last_event_time.total_cycle|345|0|32|||||||||||
RULE16.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[16 + offset].settings.event.group|349|0|8|||||||||||
RULE16.SETTINGS.EVENT.LOCAL|uint32_t|rules[16 + offset].settings.event.local|350|0|32|||||||||||
RULE16.SETTINGS.EVENT.ERR_LEVEL|uint8_t||354|0|3||5.1|||||||||
RULE16.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||354|3|1||2.11|||||||||
RULE16.SETTINGS.IS_ACTIVE|||354|4|1||2.8|||||||||
RULE16.SETTINGS.CONDITION.TYPE|||354|5|3||3.3|||||||||
RULE16.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[16 + offset].settings.condition.count_threshold|355|0|16|||||||||||
RULE16.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[16 + offset].settings.condition.time_threshold_ms|357|0|32|||||||||||
RULE16.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[16 + offset].settings.deploy_bct_id|361|0|16|||||||||||
RULE16.COUNTER|uint16_t|rules[16 + offset].counter|363|0|16|||||||||||
RULE16.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[16 + offset].last_event_time.total_cycle|365|0|32|||||||||||
RULE17.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[17 + offset].settings.event.group|369|0|8|||||||||||
RULE17.SETTINGS.EVENT.LOCAL|uint32_t|rules[17 + offset].settings.event.local|370|0|32|||||||||||
RULE17.SETTINGS.EVENT.ERR_LEVEL|uint8_t||374|0|3||5.1|||||||||
RULE17.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||374|3|1||2.11|||||||||
RULE17.SETTINGS.IS_ACTIVE|||374|4|1||2.8|||||||||
RULE17.SETTINGS.CONDITION.TYPE|||374|5|3||3.3|||||||||
RULE17.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[17 + offset].settings.condition.count_threshold|375|0|16|||||||||||
RULE17.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[17 + offset].settings.condition.time_threshold_ms|377|0|32|||||||||||
RULE17.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[17 + offset].settings.deploy_bct_id|381|0|16|||||||||||
RULE17.COUNTER|uint16_t|rules[17 + offset].counter|383|0|16|||||||||||
RULE17.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[17 + offset].last_event_time.total_cycle|385|0|32|||||||||||
RULE18.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[18 + offset].settings.event.group|389|0|8|||||||||||
RULE18.SETTINGS.EVENT.LOCAL|uint32_t|rules[18 + offset].settings.event.local|390|0|32|||||||||||
RULE18.SETTINGS.EVENT.ERR_LEVEL|uint8_t||394|0|3||5.1|||||||||
RULE18.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||394|3|1||2.11|||||||||
RULE18.SETTINGS.IS_ACTIVE|||394|4|1||2.8|||||||||
RULE18.SETTINGS.CONDITION.TYPE|||394|5|3||3.3|||||||||
RULE18.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[18 + offset].settings.condition.count_threshold|395|0|16|||||||||||
RULE18.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[18 + offset].settings.condition.time_threshold_ms|397|0|32|||||||||||
RULE18.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[18 + offset].settings.deploy_bct_id|401|0|16|||||||||||
RULE18.COUNTER|uint16_t|rules[18 + offset].counter|403|0|16|||||||||||
RULE18.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[18 + offset].last_event_time.total_cycle|405|0|32|||||||||||
RULE19.SETTINGS.EVENT.GROUP|uint8_t|(uint8_t)rules[19 + offset].settings.event.group|409|0|8|||||||||||
RULE19.SETTINGS.EVENT.LOCAL|uint32_t|rules[19 + offset].settings.event.local|410|0|32|||||||||||
RULE19.SETTINGS.EVENT.ERR_LEVEL|uint8_t||414|0|3||5.1|||||||||
RULE19.SETTINGS.SHOULD_MATCH_ERR_LEVEL|||414|3|1||2.11|||||||||
RULE19.SETTINGS.IS_ACTIVE|||414|4|1||2.8|||||||||
RULE19.SETTINGS.CONDITION.TYPE|||414|5|3||3.3|||||||||
RULE19.SETTINGS.CONDITION.COUNT_THRESHOLD|uint16_t|rules[19 + offset].settings.condition.count_threshold|415|0|16|||||||||||
RULE19.SETTINGS.CONDITION.TIME_THRESHOLD_MS|uint32_t|rules[19 + offset].settings.condition.time_threshold_ms|417|0|32|||||||||||
RULE19.SETTINGS.DEPLOY_BCT_ID|uint16_t|(uint16_t)rules[19 + offset].settings.deploy_bct_id|421|0|16|||||||||||
RULE19.COUNTER|uint16_t|rules[19 + offset].counter|423|0|16|||||||||||
RULE19.LAST_EVENT_TIME.TOTAL_CYCLE|uint32_t|(uint32_t)rules[19 + offset].last_event_time.total_cycle|425|0|32|||||||||||
