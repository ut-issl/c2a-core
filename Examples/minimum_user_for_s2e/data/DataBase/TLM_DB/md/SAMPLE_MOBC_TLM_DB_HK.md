# SAMPLE_MOBC_TLM_DB_HK

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
OBC_MM_STS|uint8_t||39|0|1||2.7|||||||||
OBC_MM_OPSMODE_PREV|||39|1|7||17.1|||||||||
OBC_TDSP_CURRENT_ID|uint16_t|TDSP_info->task_list_id|40|0|16|||||||||||
OBC_TCTF_LAST_RECV_ACK|uint8_t|(uint8_t)gs_driver->latest_info->rx.rec_status|42|0|8||2.13|||||||TC Transer Frame 受信処理結果||
OBC_TCTF_LAST_RECV_TIME|uint32_t|(uint32_t)gs_driver->latest_info->rx.last_rec_time|43|0|32|||||||||最新TC Transer Frame受信時刻||
OBC_TCP_LAST_RECV_ACK|uint8_t|(uint8_t)(gs_driver->latest_info->rx.cmd_ack)|47|0|8||13.1|||||||最新TCPacket受信処理結果||
OBC_GS_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_gs_cmd_list)|48|0|32|||||||||地上局コマンドカウンタ||
OBC_GS_CMD_LAST_EXEC_TIME|uint32_t|gs_command_dispatcher->prev.time.total_cycle|52|0|32|||||||||最新地上局コマンド実行時刻||
OBC_GS_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(gs_command_dispatcher->prev.code)|56|0|16|||||||||最新地上局コマンドID||
OBC_GS_CMD_LAST_EXEC_STS|int8_t|(int8_t)(gs_command_dispatcher->prev.sts)|58|0|8||7.1|||||||最新地上局コマンド実行結果||
OBC_GS_CMD_LAST_ERR_TIME|uint32_t|gs_command_dispatcher->prev_err.time.total_cycle|59|0|32|||||||||地上局異常コマンド実行時刻||
OBC_GS_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(gs_command_dispatcher->prev_err.code)|63|0|16|||||||||地上局異常コマンドID||
OBC_GS_CMD_LAST_ERR_STS|int8_t|(int8_t)(gs_command_dispatcher->prev_err.sts)|65|0|8||7.1|||||||地上局異常コマンド実行結果||
OBC_TLC_GS.COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS])|66|0|32|||||||||バス用TLのコマンドカウンタ||
OBC_TLC_GS.QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS]))|70|0|8|||||||||バス用TLの登録コマンド数||
OBC_TLC_GS.LAST_EXEC_TIME|uint32_t|timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle|71|0|32|||||||||バス用TLの最新コマンド実行時刻||
OBC_TLC_GS.LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code)|75|0|16|||||||||バス用TLの最新コマンドID||
OBC_TLC_GS.LAST_EXEC_STS|int8_t|(int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.sts)|77|0|8||7.1|||||||バス用TLの最新コマンド実行結果||
OBC_TLC_GS.LAST_ERR_TIME|uint32_t|timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle|78|0|32|||||||||バス用TLの異常コマンド実行時刻||
OBC_TLC_GS.LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code)|82|0|16|||||||||バス用TLの異常コマンドID||
OBC_TLC_GS.LAST_ERR_STS|int8_t|(int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.sts)|84|0|8||7.1|||||||バス用TLの異常コマンド実行結果||
OBC_TLC_GS.NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet)))|85|0|32|||||||||バス用TLの次コマンド実行時刻|変数指定が大変なことに…|
OBC_TLC_GS.NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet)))|89|0|16|||||||||バス用TLの次コマンドID|変数指定が大変なことに…|
OBC_TLC_BC.COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])|91|0|32|||||||||BC用TLのコマンドカウンタ||
OBC_TLC_BC.QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))|95|0|8|||||||||BC用TLの登録コマンド数||
OBC_TLC_TLM.COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])|96|0|32|||||||||テレメ用TLのコマンドカウンタ||
OBC_TLC_TLM.QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))|100|0|8|||||||||テレメ用TLの登録コマンド数||
OBC_TLC_MIS.COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])|101|0|32|||||||||ミッション用TLのコマンドカウンタ||
OBC_TLC_MIS.QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))|105|0|8|||||||||ミッション用TLの登録コマンド数||
OBC_TLC_MIS.LAST_EXEC_TIME|uint32_t|timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.time.total_cycle|106|0|32|||||||||ミッション用TLの最新コマンド実行時刻||
OBC_TLC_MIS.LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.code)|110|0|16|||||||||ミッション用TLの最新コマンドID||
OBC_TLC_MIS.LAST_EXEC_STS|int8_t|(int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.sts)|112|0|8||7.1|||||||ミッション用TLの最新コマンド実行結果||
OBC_TLC_MIS.LAST_ERR_TIME|uint32_t|timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.time.total_cycle|113|0|32|||||||||ミッション用TLの異常コマンド実行時刻||
OBC_TLC_MIS.LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.code)|117|0|16|||||||||ミッション用TLの異常コマンドID||
OBC_TLC_MIS.LAST_ERR_STS|int8_t|(int8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.sts)|119|0|8||7.1|||||||ミッション用TLの異常コマンド実行結果||
OBC_TLC_MIS.NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))->packet)))|120|0|32|||||||||ミッション用TLの次コマンド実行時刻|変数指定が大変なことに…|
OBC_TLC_MIS.NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))->packet)))|124|0|16|||||||||ミッション用TLの次コマンドID|変数指定が大変なことに…|
OBC_TLC_GS.SOE_FLAG|uint8_t||126|0|1||2.3|||||||||
OBC_TLC_GS.LOCKOUT_FLAG|||126|1|1||2.10|||||||||
OBC_TLC_BC.SOE_FLAG|||126|2|1||2.3|||||||||
OBC_TLC_BC.LOCKOUT_FLAG|||126|3|1||2.10|||||||||
OBC_TLC_TLM.SOE_FLAG|||126|4|1||2.3|||||||||
OBC_TLC_TLM.LOCKOUT_FLAG|||126|5|1||2.10|||||||||
OBC_TLC_MIS.SOE_FLAG|||126|6|1||2.3|||||||||
OBC_TLC_MIS.LOCKOUT_FLAG|||126|7|1||2.10|||||||||
WDT.IS_ENABLE|uint8_t||127|0|1||2.3|||||||||
WDT.IS_CLEAR_ENABLE|||127|1|1||2.3|||||||||
DUMMY_DATA0|||127|2|6|||||||||||
OBC_BCT_BLK_PTR|uint16_t|block_command_table->pos.block|128|0|16|||||||||BCT ブロックポインタ||
OBC_BCT_CMD_PTR|uint8_t|(uint8_t)(block_command_table->pos.cmd)|130|0|8|||||||||BCT コマンドポインタ||
OBC_BCT_REGD_TIME|uint32_t|((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1)))|131|0|32|||||||||BCT 登録コマンド時刻|変数指定が大変なことに…|
OBC_BCT_REGD_ID|uint16_t|(uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1)))|135|0|16|||||||||BCT 登録コマンドID|変数指定が大変なことに…|
EL.STATISTICS.RECORD_COUNTER_TOTAL|uint32_t|event_logger->statistics.record_counter_total|137|0|32|||||||||||
EL.STATISTICS.RECORD_COUNTERS_HIGH|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_HIGH]|141|0|16||||||||||データサイズ減らすためにu16|
EL.STATISTICS.RECORD_COUNTERS_MIDDLE|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_MIDDLE]|143|0|16||||||||||データサイズ減らすためにu16|
EL.STATISTICS.RECORD_COUNTERS_LOW|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_LOW]|145|0|16||||||||||データサイズ減らすためにu16|
EL.STATISTICS.RECORD_COUNTERS_EL|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EL]|147|0|16||||||||||データサイズ減らすためにu16|
EL.STATISTICS.RECORD_COUNTERS_EH|uint16_t|(uint16_t)event_logger->statistics.record_counters[EL_ERROR_LEVEL_EH]|149|0|16||||||||||データサイズ減らすためにu16|
EL.LATEST_EVENT.GROUP|uint8_t|(uint8_t)event_logger->latest_event.group|151|0|8|||||||||||
EL.LATEST_EVENT.LOCAL|uint32_t|event_logger->latest_event.local|152|0|32|||||||||||
EL.LATEST_EVENT.ERR_LEVEL|uint8_t|(uint8_t)event_logger->latest_event.err_level|156|0|8||5.1|||||||||
EL.LATEST_EVENT.TIME.TOTAL_CYCLE|uint32_t|event_logger->latest_event.time.total_cycle|157|0|32|||||||||||
EL.LATEST_EVENT.TIME.STEP|uint8_t|(uint8_t)event_logger->latest_event.time.step|161|0|8|||||||||||
EL.LATEST_EVENT.NOTE|uint32_t|event_logger->latest_event.note|162|0|32||||||||||u8にしてもよい|
EL.LATEST_HIGH_EVENT.GROUP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->group|166|0|8|||||||||||
EL.LATEST_HIGH_EVENT.LOCAL|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->local|167|0|32|||||||||||
EL.LATEST_HIGH_EVENT.TIME.TOTAL_CYCLE|uint32_t|EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.total_cycle|171|0|32|||||||||||
EL.LATEST_HIGH_EVENT.TIME.STEP|uint8_t|(uint8_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->time.step|175|0|8|||||||||||
EL.LATEST_HIGH_EVENT.NOTE|uint16_t|(uint16_t)EL_get_the_nth_tlog_from_the_latest(EL_ERROR_LEVEL_HIGH, 0)->note|176|0|16||||||||||u8にしてもよい|
EH.LOG_TABLE.RESPOND_COUNTER|uint32_t|event_handler->log_table.respond_counter|178|0|32|||||||||||
ANOMALY_MOBC_AL_COUNTER|uint32_t|(uint32_t)anomaly_logger->counter|182|0|32|||||||||||
ANOMALY_MOBC_AL_LATEST_TIME_MASTER|uint32_t|(uint32_t)anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].time.total_cycle|186|0|32|||||||||||
ANOMALY_MOBC_AL_LATEST_TIME_STEP|uint8_t|(uint8_t)(anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].time.step & 0x000000ff)|190|0|8|||||||||||
ANOMALY_MOBC_AL_LATEST_CODE_GROUP|uint8_t|(uint8_t)(anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].code.group & 0x000000ff)|191|0|8|||||||||||
ANOMALY_MOBC_AL_LATEST_CODE_LOCAL|uint32_t|anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].code.local|192|0|32|||||||||||
ANOMALY_MOBC_AL_LATEST_RUN_LENGTH|uint8_t|(uint8_t)(anomaly_logger->records[(anomaly_logger->header == 0) ? 0 : anomaly_logger->header - 1].run_length & 0x000000ff)|196|0|8|||||||||||
ANOMALY_MOBC_AL_HEADER|uint8_t|(uint8_t)(anomaly_logger->header)|197|0|8|||||||||||
OBC_AH_ACTION_COUNTER|uint8_t|(uint8_t)(anomaly_handler->action_counter)|198|0|8|||||||||アノマリ対応発生回数||
OBC_AH_LATEST_ID|uint8_t|(uint8_t)(anomaly_handler->latest_id)|199|0|8|||||||||アノマリ対応ID||
OBC_AH_RESPOND_AT|uint32_t|(uint32_t)(anomaly_handler->respond_at.total_cycle)|200|0|32|||||||||アノマリ対応実施時刻||
OBC_GS_CMD_ERR_COUNTER|uint8_t|(uint8_t)(gs_command_dispatcher->error_counter)|204|0|8|||||||||アノマリ対応発生回数||
DCU.EXEC_LOGS0.CMD_CODE|uint16_t|(uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].cmd_code)|205|0|16|o||||||||||
DCU.EXEC_LOGS0.STATUS|uint8_t|(uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].status)|207|0|8||4.1|||||||||
DCU.EXEC_LOGS0.EXEC_COUNTER|uint16_t|(uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].exec_counter)|208|0|16|||||||||||
DCU.EXEC_LOGS0.LAST_EXEC_STS|int8_t|(int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].last_exec_sts)|210|0|8||7.1|||||||||
DCU.EXEC_LOGS0.LAST_EXEC_TIME.TOTAL_CYCLE|uint32_t|divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[0]].last_exec_time.total_cycle|211|0|32|||||||||||
DCU.EXEC_LOGS1.CMD_CODE|uint16_t|(uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].cmd_code)|215|0|16|o||||||||||
DCU.EXEC_LOGS1.STATUS|uint8_t|(uint8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].status)|217|0|8||4.1|||||||||
DCU.EXEC_LOGS1.EXEC_COUNTER|uint16_t|(uint16_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].exec_counter)|218|0|16|||||||||||
DCU.EXEC_LOGS1.LAST_EXEC_STS|int8_t|(int8_t)(divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].last_exec_sts)|220|0|8||7.1|||||||||
DCU.EXEC_LOGS1.LAST_EXEC_TIME.TOTAL_CYCLE|uint32_t|divided_cmd_utility->exec_logs[divided_cmd_utility->exec_log_order[1]].last_exec_time.total_cycle|221|0|32|||||||||||
GIT_REV_CORE|uint32_t|GIT_REV_CORE_SHORT|225|0|32|o||||||||||
GIT_REV_USER|uint32_t|GIT_REV_USER_SHORT|229|0|32|o||||||||||
DUMMY_DATA1|uint32_t|0|233|0|32|||||||||||
DUMMY_DATA2|uint32_t|0|237|0|32|||||||||||
DUMMY_DATA3|uint32_t|0|241|0|32|||||||||||
DUMMY_DATA4|uint32_t|0|245|0|32|||||||||||
DUMMY_DATA5|uint32_t|0|249|0|32|||||||||||
DUMMY_DATA6|uint32_t|0|253|0|32|||||||||||
DUMMY_DATA7|uint32_t|0|257|0|32|||||||||||
DUMMY_DATA8|uint32_t|0|261|0|32|||||||||||
DUMMY_DATA9|uint32_t|0|265|0|32|||||||||||
DUMMY_DATA10|uint32_t|0|269|0|32|||||||||||
DUMMY_DATA11|uint32_t|0|273|0|32|||||||||||
DUMMY_DATA12|uint32_t|0|277|0|32|||||||||||
DUMMY_DATA13|uint32_t|0|281|0|32|||||||||||
DUMMY_DATA14|uint32_t|0|285|0|32|||||||||||
DUMMY_DATA15|uint32_t|0|289|0|32|||||||||||
DUMMY_DATA16|uint32_t|0|293|0|32|||||||||||
DUMMY_DATA17|uint32_t|0|297|0|32|||||||||||
DUMMY_DATA18|uint32_t|0|301|0|32|||||||||||
DUMMY_DATA19|uint32_t|0|305|0|32|||||||||||
DUMMY_DATA20|uint32_t|0|309|0|32|||||||||||
DUMMY_DATA21|uint32_t|0|313|0|32|||||||||||
DUMMY_DATA22|uint32_t|0|317|0|32|||||||||||
DUMMY_DATA23|uint32_t|0|321|0|32|||||||||||
DUMMY_DATA24|uint32_t|0|325|0|32|||||||||||
DUMMY_DATA25|uint32_t|0|329|0|32|||||||||||
DUMMY_DATA26|uint32_t|0|333|0|32|||||||||||
DUMMY_DATA27|uint32_t|0|337|0|32|||||||||||
DUMMY_DATA28|uint32_t|0|341|0|32|||||||||||
DUMMY_DATA29|uint32_t|0|345|0|32|||||||||||
DUMMY_DATA30|uint32_t|0|349|0|32|||||||||||
DUMMY_DATA31|uint32_t|0|353|0|32|||||||||||
DUMMY_DATA32|uint32_t|0|357|0|32|||||||||||
DUMMY_DATA33|uint32_t|0|361|0|32|||||||||||
DUMMY_DATA34|uint32_t|0|365|0|32|||||||||||
DUMMY_DATA35|uint32_t|0|369|0|32|||||||||||
DUMMY_DATA36|uint32_t|0|373|0|32|||||||||||
DUMMY_DATA37|uint32_t|0|377|0|32|||||||||||
DUMMY_DATA38|uint32_t|0|381|0|32|||||||||||
DUMMY_DATA39|uint32_t|0|385|0|32|||||||||||
DUMMY_DATA40|uint32_t|0|389|0|32|||||||||||
DUMMY_DATA41|uint32_t|0|393|0|32|||||||||||
DUMMY_DATA42|uint32_t|0|397|0|32|||||||||||
DUMMY_DATA43|uint32_t|0|401|0|32|||||||||||
DUMMY_DATA44|uint32_t|0|405|0|32|||||||||||
DUMMY_DATA45|uint32_t|0|409|0|32|||||||||||
