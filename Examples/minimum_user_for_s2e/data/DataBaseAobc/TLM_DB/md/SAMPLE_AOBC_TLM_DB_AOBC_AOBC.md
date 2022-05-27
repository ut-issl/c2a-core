# SAMPLE_AOBC_TLM_DB_AOBC_AOBC

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
TM_MODE_TIME|uint32_t|(uint32_t)(TMGR_get_master_clock().mode_cycle)|26|0|32|||||||||モード遷移後経過時刻||
TM_UTL_UNIXTIME_EPOCH|double|TMGR_get_utl_unixtime_epoch()|30|0|64|||||||||UTL_cmdで用いるunixtimeの紀元||
TM_CYCLES_PER_SEC_FIX_RATIO|double|(double)(time_manager->unixtime_info_.cycle_correction)|38|0|64|||||||||CYCLES_PER_SECの補正倍率. 初期値は1.0||
MM_STS|uint8_t|(uint8_t)(mode_manager->stat)|46|0|8|||||||||モード遷移実行状態||
MM_OPSMODE|uint8_t|(uint8_t)(mode_manager->current_id)|47|0|8|||||||||現状モード番号||
MM_OPSMODE_PREV|uint8_t|(uint8_t)(mode_manager->previous_id)|48|0|8|||||||||直前モード番号||
TDSP_CURRENT_ID|uint8_t|(uint8_t)(TDSP_info->task_list_id)|49|0|8|||||||||現状 TaskList ID||
TDSP_CMD_LAST_ERR_TIME_MASTER|uint32_t|(uint32_t)(TDSP_info->tskd.prev_err.time.total_cycle)|50|0|32|||||||||最新内部コマンド実行時刻||
TDSP_CMD_LAST_ERR_TIME_STEP|uint8_t|(uint8_t)(TDSP_info->tskd.prev_err.time.step)|54|0|8|||||||||||
TDSP_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(TDSP_info->tskd.prev_err.code)|55|0|16|||||||||最新内部コマンドID||
TDSP_CMD_LAST_ERR_STS|int32_t|(int32_t)(TDSP_info->tskd.prev_err.sts)|57|0|32||7.1|||||||最新内部コマンド実行結果||
GS_RET_FROM_IF_RX|int32_t|(int32_t)gs_driver->latest_info->ret_from_if_rx|61|0|32|||||||||||
TCTF_LAST_RECV_ACK|uint8_t|(uint8_t)gs_driver->latest_info->rec_status|65|0|8||2.8|||||||TC Transer Frame DS処理結果||
TCTF_VALIDATE_STATUS|uint8_t|(uint8_t)gs_driver->latest_info->tctf_validate_status|66|0|8||18.1|||||||TC Transer Frame 検証結果||
TCTF_LAST_RECV_TIME|uint32_t|(uint32_t)gs_driver->latest_info->last_rec_time|67|0|32|||||||||最新TC Transer Frame受信時刻||
TCTF_FARM_PW|uint8_t|(uint8_t)gs_validate_info->positive_window_width|71|0|8|||||||||FARM-1 Positive Window Width||
TCP_LAST_RECV_ACK|uint8_t|(uint8_t)gs_driver->latest_info->cmd_ack|72|0|8||13.1|||||||最新TCPacket受信処理結果||
TLM_TX_PORT_TYPE|uint8_t|(uint8_t)gs_driver->tlm_tx_port_type|73|0|8||2.1|||||||TLM の送信情報がUARTのものかGSのものか||
GS_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_gs_cmd_list)|74|0|32|||||||||地上局コマンドカウンタ||
GS_CMD_LAST_EXEC_TIME|uint32_t|(uint32_t)(gs_command_dispatcher->prev.time.total_cycle)|78|0|32|||||||||最新地上局コマンド実行時刻||
GS_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(gs_command_dispatcher->prev.code)|82|0|16|||||||||最新地上局コマンドID||
GS_CMD_LAST_EXEC_STS|int32_t|(int32_t)(gs_command_dispatcher->prev.sts)|84|0|32||7.1|||||||最新地上局コマンド実行結果||
GS_CMD_LAST_ERR_TIME|uint32_t|(uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle)|88|0|32|||||||||最新地上局コマンド実行時刻||
GS_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(gs_command_dispatcher->prev_err.code)|92|0|16|||||||||最新地上局コマンドID||
GS_CMD_LAST_ERR_STS|int32_t|(int32_t)(gs_command_dispatcher->prev_err.sts)|94|0|32||7.1|||||||最新地上局コマンド実行結果||
GS_CMD_ERR_COUNTER|uint32_t|(uint32_t)(gs_command_dispatcher->error_counter)|98|0|32|||||||||地上局コマンド実行異常カウンタ||
RT_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_rt_cmd_list)|102|0|32|||||||||内部コマンドカウンタ||
RT_CMD_LAST_EXEC_TIME|uint32_t|(uint32_t)(realtime_command_dispatcher->prev.time.total_cycle)|106|0|32|||||||||最新内部コマンド実行時刻||
RT_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(realtime_command_dispatcher->prev.code)|110|0|16|||||||||最新内部コマンドID||
RT_CMD_LAST_EXEC_STS|int32_t|(int32_t)(realtime_command_dispatcher->prev.sts)|112|0|32||7.1|||||||最新内部コマンド実行結果||
RT_CMD_LAST_ERR_TIME|uint32_t|(uint32_t)(realtime_command_dispatcher->prev_err.time.total_cycle)|116|0|32|||||||||最新内部コマンド実行時刻||
RT_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(realtime_command_dispatcher->prev_err.code)|120|0|16|||||||||最新内部コマンドID||
RT_CMD_LAST_ERR_STS|int32_t|(int32_t)(realtime_command_dispatcher->prev_err.sts)|122|0|32||7.1|||||||最新内部コマンド実行結果||
RT_CMD_ERR_COUNTER|uint32_t|(uint32_t)(realtime_command_dispatcher->error_counter)|126|0|32|||||||||地上局コマンド実行異常カウンタ||
TL0_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[0])|130|0|32|||||||||TL0コマンドカウンタ||
TL0_CMD_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[0]))|134|0|8|||||||||TL0登録コマンド数||
TL0_CMD_LAST_EXEC_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher[0].prev.time.total_cycle)|135|0|32|||||||||最新TL0コマンド実行時刻||
TL0_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher[0].prev.code)|139|0|16|||||||||最新TL0コマンドID||
TL0_CMD_LAST_EXEC_STS|int32_t|(int32_t)(timeline_command_dispatcher[0].prev.sts)|141|0|32||7.1|||||||最新TL0コマンド実行結果||
TL0_CMD_LAST_ERR_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher[0].prev_err.time.total_cycle)|145|0|32|||||||||最新TL0コマンド実行時刻||
TL0_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher[0].prev_err.code)|149|0|16|||||||||最新TL0コマンドID||
TL0_CMD_LAST_ERR_STS|int32_t|(int32_t)(timeline_command_dispatcher[0].prev_err.sts)|151|0|32||7.1|||||||最新TL0コマンド実行結果||
TL0_CMD_ERR_COUNTER|uint32_t|(uint32_t)(timeline_command_dispatcher[0].error_counter)|155|0|32|||||||||地上局コマンド実行異常カウンタ||
TL0_CMD_SOE_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher[0].stop_on_error)|159|0|8||2.3|||||||||
TL0_CMD_LOCKOUT_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher[0].lockout)|160|0|8||2.4|||||||||
TL0_NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[0])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[0]))->packet)))|161|0|32|||||||||次TL0コマンド実行時刻|変数指定が大変なことに…|
TL0_NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[0])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[0]))->packet)))|165|0|16|||||||||次TL0コマンドID|変数指定が大変なことに…|
TL1_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[1])|167|0|32|||||||||TL1コマンドカウンタ||
TL1_CMD_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[1]))|171|0|8|||||||||TL1登録コマンド数||
TL1_CMD_LAST_EXEC_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher[1].prev.time.total_cycle)|172|0|32|||||||||最新TL1コマンド実行時刻||
TL1_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher[1].prev.code)|176|0|16|||||||||最新TL1コマンドID||
TL1_CMD_LAST_EXEC_STS|int32_t|(int32_t)(timeline_command_dispatcher[1].prev.sts)|178|0|32||7.1|||||||最新TL1コマンド実行結果||
TL1_CMD_LAST_ERR_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher[1].prev_err.time.total_cycle)|182|0|32|||||||||最新TL1コマンド実行時刻||
TL1_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher[1].prev_err.code)|186|0|16|||||||||最新TL1コマンドID||
TL1_CMD_LAST_ERR_STS|int32_t|(int32_t)(timeline_command_dispatcher[1].prev_err.sts)|188|0|32||7.1|||||||最新TL1コマンド実行結果||
TL1_CMD_ERR_COUNTER|uint32_t|(uint32_t)(timeline_command_dispatcher[1].error_counter)|192|0|32|||||||||地上局コマンド実行異常カウンタ||
TL1_CMD_SOE_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher[1].stop_on_error)|196|0|8||2.3|||||||||
TL1_CMD_LOCKOUT_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher[1].lockout)|197|0|8||2.4|||||||||
TL1_NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[1])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[1]))->packet)))|198|0|32|||||||||次TL1コマンド実行時刻|変数指定が大変なことに…|
TL1_NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[1])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[1]))->packet)))|202|0|16|||||||||次TL1コマンドID|変数指定が大変なことに…|
TL2_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[2])|204|0|32|||||||||TL1コマンドカウンタ||
TL2_CMD_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[2]))|208|0|8|||||||||TL1登録コマンド数||
TL2_CMD_LAST_EXEC_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher[2].prev.time.total_cycle)|209|0|32|||||||||最新TL1コマンド実行時刻||
TL2_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher[2].prev.code)|213|0|16|||||||||最新TL1コマンドID||
TL2_CMD_LAST_EXEC_STS|int32_t|(int32_t)(timeline_command_dispatcher[2].prev.sts)|215|0|32||7.1|||||||最新TL1コマンド実行結果||
TL2_CMD_LAST_ERR_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher[2].prev_err.time.total_cycle)|219|0|32|||||||||最新TL1コマンド実行時刻||
TL2_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher[2].prev_err.code)|223|0|16|||||||||最新TL1コマンドID||
TL2_CMD_LAST_ERR_STS|int32_t|(int32_t)(timeline_command_dispatcher[2].prev_err.sts)|225|0|32||7.1|||||||最新TL1コマンド実行結果||
TL2_CMD_ERR_COUNTER|uint32_t|(uint32_t)(timeline_command_dispatcher[2].error_counter)|229|0|32|||||||||地上局コマンド実行異常カウンタ||
TL2_CMD_SOE_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher[2].stop_on_error)|233|0|8||2.3|||||||||
TL2_CMD_LOCKOUT_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher[2].lockout)|234|0|8||2.4|||||||||
TL2_NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[2])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[2]))->packet)))|235|0|32|||||||||次TL1コマンド実行時刻|変数指定が大変なことに…|
TL2_NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[2])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[2]))->packet)))|239|0|16|||||||||次TL1コマンドID|変数指定が大変なことに…|
BCT_BLK_PTR|uint8_t|(uint8_t)(block_command_table->pos.block)|241|0|8|||||||||BCT ブロックポインタ||
BCT_CMD_PTR|uint8_t|(uint8_t)(block_command_table->pos.cmd)|242|0|8|||||||||BCT コマンドポインタ||
BCT_REGD_TIME|uint32_t|((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1)))|243|0|32|||||||||BCT 登録コマンド時刻|変数指定が大変なことに…|
BCT_REGD_ID|uint16_t|(uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1)))|247|0|16|||||||||BCT 登録コマンドID|変数指定が大変なことに…|
MS_BUFFER_AVAILABLE|uint8_t|gs_driver->ccsds_info.buffer_num|249|0|8|||||||||CCSDS MS VCDU 使用量||
MS_T2M_FLUSH_INTERVAL|uint32_t|(uint32_t)(DI_GS_ms_tlm_packet_handler->tc_packet_to_m_pdu.flush_interval)|250|0|32|||||||||TL0コマンドカウンタ||
RP_T2M_FLUSH_INTERVAL|uint32_t|(uint32_t)(DI_GS_rp_tlm_packet_handler->tc_packet_to_m_pdu.flush_interval)|254|0|32|||||||||TL0コマンドカウンタ||
MS_TLM_COUNTER|uint32_t|PL_count_executed_nodes(&PH_ms_tlm_list)|258|0|32|||||||||TL0コマンドカウンタ||
MS_TLM_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_ms_tlm_list))|262|0|8|||||||||TL0登録コマンド数||
ST_TLM_COUNTER|uint32_t|PL_count_executed_nodes(&PH_st_tlm_list)|263|0|32|||||||||TL0コマンドカウンタ||
ST_TLM_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_st_tlm_list))|267|0|8|||||||||TL0登録コマンド数||
RP_TLM_COUNTER|uint32_t|PL_count_executed_nodes(&PH_rp_tlm_list)|268|0|32|||||||||TL0コマンドカウンタ||
RP_TLM_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_rp_tlm_list))|272|0|8|||||||||TL0登録コマンド数||
TCI_TX_CYCLE|uint32_t|(uint32_t)gs_driver->latest_info->send_cycle|273|0|32|||||||||時刻校正情報::送出サイクル||
TCI_TX_BITRATE|uint32_t|gs_driver->driver_ccsds.ccsds_config.bitrate|277|0|32|||||||||時刻校正情報::送出ビットレート||
TCI_VCID|uint8_t|(uint8_t)(gs_driver->info[2].vcid)|281|0|8|||||||||時刻構成情報::送出VCID||
TCI_VCDU_COUNTER|uint32_t|gs_driver->latest_info->vcdu_counter|282|0|32|||||||||時刻校正情報::送出VCDUカウンタ||
