# SAMPLE_AOBC_TLM_DB_AOBC_HK

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
OBC_TM_MODE_TIME|uint32_t|TMGR_get_master_clock().mode_cycle|26|0|32|||||||||||
OBC_TM_UNIXTIME_AT_TI0|double|TMGR_get_unixtime_at_ti0()|30|0|64|||||||||||
OBC_MM_OPSMODE|uint8_t|(uint8_t)(mode_manager->current_id)|38|0|8||17.1|||||||||
OBC_MM_STS|uint8_t||39|0|1||2.5|||||||||
OBC_MM_OPSMODE_PREV|||39|1|7||17.1|||||||||
OBC_TDSP_CURRENT_ID|uint16_t|TDSP_info->task_list_id|40|0|16|||||||||||
OBC_TCTF_LAST_RECV_ACK|uint8_t|(uint8_t)gs_driver->latest_info->rec_status|42|0|8||2.7|||||||TC Transer Frame 受信処理結果||
OBC_TCTF_LAST_RECV_TIME|uint32_t|(uint32_t)gs_driver->latest_info->last_rec_time|43|0|32|||||||||最新TC Transer Frame受信時刻||
OBC_TCP_LAST_RECV_ACK|uint8_t|(uint8_t)(gs_driver->latest_info->cmd_ack)|47|0|8||13.1|||||||最新TCPacket受信処理結果||
OBC_GS_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_gs_cmd_list)|48|0|32|||||||||地上局コマンドカウンタ||
OBC_GS_CMD_LAST_EXEC_TIME|uint32_t|gs_command_dispatcher->prev.time.total_cycle|52|0|32|||||||||最新地上局コマンド実行時刻||
OBC_GS_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(gs_command_dispatcher->prev.code)|56|0|16|||||||||最新地上局コマンドID||
OBC_GS_CMD_LAST_EXEC_STS|int8_t|(int8_t)(gs_command_dispatcher->prev.sts)|58|0|8||7.1|||||||最新地上局コマンド実行結果||
OBC_GS_CMD_LAST_ERR_TIME|uint32_t|gs_command_dispatcher->prev_err.time.total_cycle|59|0|32|||||||||地上局異常コマンド実行時刻||
OBC_GS_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(gs_command_dispatcher->prev_err.code)|63|0|16|||||||||地上局異常コマンドID||
OBC_GS_CMD_LAST_ERR_STS|int8_t|(int8_t)(gs_command_dispatcher->prev_err.sts)|65|0|8||7.1|||||||地上局異常コマンド実行結果||
OBC_TL0_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[0])|66|0|32|||||||||TL0コマンドカウンタ||
OBC_TL0_CMD_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[0]))|70|0|8|||||||||TL0登録コマンド数||
OBC_TL0_CMD_LAST_EXEC_TIME|uint32_t|timeline_command_dispatcher[0].prev.time.total_cycle|71|0|32|||||||||最新TL0コマンド実行時刻||
OBC_TL0_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher[0].prev.code)|75|0|16|||||||||最新TL0コマンドID||
OBC_TL0_CMD_LAST_EXEC_STS|int8_t|(int8_t)(timeline_command_dispatcher[0].prev.sts)|77|0|8||7.1|||||||最新TL0コマンド実行結果||
OBC_TL0_CMD_LAST_ERR_TIME|uint32_t|timeline_command_dispatcher[0].prev_err.time.total_cycle|78|0|32|||||||||TL0異常コマンド実行時刻||
OBC_TL0_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher[0].prev_err.code)|82|0|16|||||||||TL0異常コマンドID||
OBC_TL0_CMD_LAST_ERR_STS|int8_t|(int8_t)(timeline_command_dispatcher[0].prev_err.sts)|84|0|8||7.1|||||||TL0異常コマンド実行結果||
OBC_TL0_NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[0])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[0]))->packet)))|85|0|32|||||||||次TL0コマンド実行時刻|変数指定が大変なことに…|
OBC_TL0_NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[0])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[0]))->packet)))|89|0|16|||||||||次TL0コマンドID|変数指定が大変なことに…|
OBC_TL0_CMD_SOE_FLAG|uint8_t||91|0|1||2.2|||||||||
OBC_TL0_CMD_LOCKOUT_FLAG|||91|1|1||2.6|||||||||
OBC_TL1_CMD_SOE_FLAG|||91|2|1||2.2|||||||||
OBC_TL1_CMD_LOCKOUT_FLAG|||91|3|1||2.6|||||||||
OBC_TL2_CMD_SOE_FLAG|||91|4|1||2.2|||||||||
OBC_TL2_CMD_LOCKOUT_FLAG|||91|5|1||2.6|||||||||
WDT.IS_ENABLE|||91|6|1||2.2|||||||||
WDT.IS_CLEAR_ENABLE|||91|7|1||2.2|||||||||
OBC_BCT_BLK_PTR|uint16_t|block_command_table->pos.block|92|0|16|||||||||BCT ブロックポインタ||
OBC_BCT_CMD_PTR|uint8_t|(uint8_t)(block_command_table->pos.cmd)|94|0|8|||||||||BCT コマンドポインタ||
OBC_BCT_REGD_TIME|uint32_t|((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1)))|95|0|32|||||||||BCT 登録コマンド時刻|変数指定が大変なことに…|
OBC_BCT_REGD_ID|uint16_t|(uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1)))|99|0|16|||||||||BCT 登録コマンドID|変数指定が大変なことに…|
EL.STATISTICS.RECORD_COUNTER_TOTAL|uint32_t|event_logger->statistics.record_counter_total|101|0|32|||||||||||
EL.STATISTICS.RECORD_COUNTERS_HIGH|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH]|105|0|16||||||||||データサイズ減らすためにu16|
EL.STATISTICS.RECORD_COUNTERS_MIDDLE|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_MIDDLE]|107|0|16||||||||||データサイズ減らすためにu16|
EL.STATISTICS.RECORD_COUNTERS_LOW|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW]|109|0|16||||||||||データサイズ減らすためにu16|
EL.STATISTICS.RECORD_COUNTERS_EL|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL]|111|0|16||||||||||データサイズ減らすためにu16|
EL.STATISTICS.RECORD_COUNTERS_EH|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH]|113|0|16||||||||||データサイズ減らすためにu16|
EL.LATEST_EVENT.GROUP|uint8_t|(uint8_t)event_logger->latest_event.group|115|0|8|||||||||||
EL.LATEST_EVENT.LOCAL|uint32_t|event_logger->latest_event.local|116|0|32|||||||||||
EL.LATEST_EVENT.ERR_LEVEL|uint8_t|(uint8_t)event_logger->latest_event.err_level|120|0|8||5.1|||||||||
EL.LATEST_EVENT.TIME.TOTAL_CYCLE|uint32_t|event_logger->latest_event.time.total_cycle|121|0|32|||||||||||
EL.LATEST_EVENT.TIME.STEP|uint8_t|(uint8_t)event_logger->latest_event.time.step|125|0|8|||||||||||
EL.LATEST_EVENT.NOTE|uint16_t|(uint16_t)event_logger->latest_event.note|126|0|16||||||||||u8にしてもよい|
EL.LATEST_HIGH_EVENT.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->group|128|0|8|||||||||||
EL.LATEST_HIGH_EVENT.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->local|129|0|32|||||||||||
EL.LATEST_HIGH_EVENT.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.total_cycle|133|0|32|||||||||||
EL.LATEST_HIGH_EVENT.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.step|137|0|8|||||||||||
EL.LATEST_HIGH_EVENT.NOTE|uint16_t|(uint16_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->note|138|0|16||||||||||u8にしてもよい|
EH.LOG_TABLE.RESPOND_COUNTER|uint32_t|event_handler->log_table.respond_counter|140|0|32|||||||||||
ANOMALY_MOBC_AL_COUNTER|uint32_t|(uint32_t)anomaly_logger->counter|144|0|32|||||||||||
ANOMALY_MOBC_AL_LATEST_TIME_MASTER|uint32_t|(uint32_t)anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].time.total_cycle|148|0|32|||||||||||
ANOMALY_MOBC_AL_LATEST_TIME_STEP|uint8_t|(uint8_t)(anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].time.step & 0x000000ff)|152|0|8|||||||||||
ANOMALY_MOBC_AL_LATEST_CODE_GROUP|uint8_t|(uint8_t)(anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].code.group & 0x000000ff)|153|0|8|||||||||||
ANOMALY_MOBC_AL_LATEST_CODE_LOCAL|uint32_t|anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].code.local|154|0|32|||||||||||
ANOMALY_MOBC_AL_LATEST_RUN_LENGTH|uint8_t|(uint8_t)(anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].run_length & 0x000000ff)|158|0|8|||||||||||
ANOMALY_MOBC_AL_HEADER|uint8_t|(uint8_t)(anomaly_logger->header)|159|0|8|||||||||||
OBC_TL1_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[1])|160|0|32|||||||||TL1コマンドカウンタ||
OBC_TL2_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[2])|164|0|32|||||||||TL1コマンドカウンタ||
OBC_TL1_CMD_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[1]))|168|0|8|||||||||||
OBC_TL2_CMD_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[2]))|169|0|8|||||||||||
OBC_AH_ACTION_COUNTER|uint8_t|(uint8_t)(anomaly_handler->action_counter)|170|0|8|||||||||アノマリ対応発生回数||
OBC_AH_LATEST_ID|uint8_t|(uint8_t)(anomaly_handler->latest_id)|171|0|8|||||||||アノマリ対応ID||
OBC_AH_RESPOND_AT|uint32_t|(uint32_t)(anomaly_handler->respond_at.total_cycle)|172|0|32|||||||||アノマリ対応実施時刻||
OBC_GS_CMD_ERR_COUNTER|uint8_t|(uint8_t)(gs_command_dispatcher->error_counter)|176|0|8|||||||||アノマリ対応発生回数||
DCU.EXEC_LOGS0.CMD_CODE|uint16_t|(uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].cmd_code)|177|0|16|o||||||||||
DCU.EXEC_LOGS0.STATUS|uint8_t|(uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].status)|179|0|8||4.1|||||||||
DCU.EXEC_LOGS0.EXEC_COUNTER|uint16_t|(uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].exec_counter)|180|0|16|||||||||||
DCU.EXEC_LOGS0.LAST_EXEC_STS|int8_t|(int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].last_exec_sts)|182|0|8||7.1|||||||||
DCU.EXEC_LOGS0.LAST_EXEC_TIME.TOTAL_CYCLE|uint32_t|divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].last_exec_time.total_cycle|183|0|32|||||||||||
DCU.EXEC_LOGS1.CMD_CODE|uint16_t|(uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].cmd_code)|187|0|16|o||||||||||
DCU.EXEC_LOGS1.STATUS|uint8_t|(uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].status)|189|0|8||4.1|||||||||
DCU.EXEC_LOGS1.EXEC_COUNTER|uint16_t|(uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].exec_counter)|190|0|16|||||||||||
DCU.EXEC_LOGS1.LAST_EXEC_STS|int8_t|(int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].last_exec_sts)|192|0|8||7.1|||||||||
DCU.EXEC_LOGS1.LAST_EXEC_TIME.TOTAL_CYCLE|uint32_t|divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].last_exec_time.total_cycle|193|0|32|||||||||||
GIT_REV_CORE|uint32_t|GIT_REV_CORE_SHORT|197|0|32|o||||||||||
GIT_REV_USER|uint32_t|GIT_REV_USER_SHORT|201|0|32|o||||||||||
