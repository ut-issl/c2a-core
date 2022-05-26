# SAMPLE_MOBC_TLM_DB_MOBC

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
GS_RET_FROM_IF_RX|int32_t|(int32_t)gs_driver->latest_info->rx.ret_from_if_rx|61|0|32|||||||||||
TCTF_LAST_RECV_ACK|uint8_t|(uint8_t)gs_driver->latest_info->rx.rec_status|65|0|8||2.14|||||||TC Transer Frame DS処理結果||
TCTF_LAST_RECV_TIME|uint32_t|(uint32_t)gs_driver->latest_info->rx.last_rec_time|66|0|32|||||||||最新TC Transer Frame受信時刻||
TCTF_FARM_PW|uint8_t|(uint8_t)gs_validate_info->positive_window_width|70|0|8|||||||||FARM-1 Positive Window Width||
TCP_LAST_RECV_ACK|uint8_t|(uint8_t)gs_driver->latest_info->rx.cmd_ack|71|0|8||13.1|||||||最新TCPacket受信処理結果||
TLM_TX_PORT_TYPE|uint8_t|(uint8_t)gs_driver->tlm_tx_port_type|72|0|8||2.2|||||||TLM の送信情報がUARTのものかGSのものか||
GS_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_gs_cmd_list)|73|0|32|||||||||地上局コマンドカウンタ||
GS_CMD_LAST_EXEC_TIME|uint32_t|(uint32_t)(gs_command_dispatcher->prev.time.total_cycle)|77|0|32|||||||||最新地上局コマンド実行時刻||
GS_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(gs_command_dispatcher->prev.code)|81|0|16|||||||||最新地上局コマンドID||
GS_CMD_LAST_EXEC_STS|int32_t|(int32_t)(gs_command_dispatcher->prev.sts)|83|0|32||7.1|||||||最新地上局コマンド実行結果||
GS_CMD_LAST_ERR_TIME|uint32_t|(uint32_t)(gs_command_dispatcher->prev_err.time.total_cycle)|87|0|32|||||||||最新地上局コマンド実行時刻||
GS_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(gs_command_dispatcher->prev_err.code)|91|0|16|||||||||最新地上局コマンドID||
GS_CMD_LAST_ERR_STS|int32_t|(int32_t)(gs_command_dispatcher->prev_err.sts)|93|0|32||7.1|||||||最新地上局コマンド実行結果||
GS_CMD_ERR_COUNTER|uint32_t|(uint32_t)(gs_command_dispatcher->error_counter)|97|0|32|||||||||地上局コマンド実行異常カウンタ||
RT_CMD_COUNTER|uint32_t|PL_count_executed_nodes(&PH_rt_cmd_list)|101|0|32|||||||||内部コマンドカウンタ||
RT_CMD_LAST_EXEC_TIME|uint32_t|(uint32_t)(realtime_command_dispatcher->prev.time.total_cycle)|105|0|32|||||||||最新内部コマンド実行時刻||
RT_CMD_LAST_EXEC_ID|uint16_t|(uint16_t)(realtime_command_dispatcher->prev.code)|109|0|16|||||||||最新内部コマンドID||
RT_CMD_LAST_EXEC_STS|int32_t|(int32_t)(realtime_command_dispatcher->prev.sts)|111|0|32||7.1|||||||最新内部コマンド実行結果||
RT_CMD_LAST_ERR_TIME|uint32_t|(uint32_t)(realtime_command_dispatcher->prev_err.time.total_cycle)|115|0|32|||||||||最新内部コマンド実行時刻||
RT_CMD_LAST_ERR_ID|uint16_t|(uint16_t)(realtime_command_dispatcher->prev_err.code)|119|0|16|||||||||最新内部コマンドID||
RT_CMD_LAST_ERR_STS|int32_t|(int32_t)(realtime_command_dispatcher->prev_err.sts)|121|0|32||7.1|||||||最新内部コマンド実行結果||
RT_CMD_ERR_COUNTER|uint32_t|(uint32_t)(realtime_command_dispatcher->error_counter)|125|0|32|||||||||地上局コマンド実行異常カウンタ||
TLC_GS.COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS])|129|0|32|||||||||TL0コマンドカウンタ||
TLC_GS.QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS]))|133|0|8|||||||||TL0登録コマンド数||
TLC_GS.LAST_EXEC_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.time.total_cycle)|134|0|32|||||||||最新TL0コマンド実行時刻||
TLC_GS.LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.code)|138|0|16|||||||||最新TL0コマンドID||
TLC_GS.LAST_EXEC_STS|int32_t|(int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev.sts)|140|0|32||7.1|||||||最新TL0コマンド実行結果||
TLC_GS.LAST_ERR_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.time.total_cycle)|144|0|32|||||||||最新TL0コマンド実行時刻||
TLC_GS.LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.code)|148|0|16|||||||||最新TL0コマンドID||
TLC_GS.LAST_ERR_STS|int32_t|(int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].prev_err.sts)|150|0|32||7.1|||||||最新TL0コマンド実行結果||
TLC_GS.ERR_COUNTER|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].error_counter)|154|0|32|||||||||地上局コマンド実行異常カウンタ||
TLC_GS.SOE_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].stop_on_error)|158|0|8||2.5|||||||||
TLC_GS.LOCKOUT_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS].lockout)|159|0|8||2.6|||||||||
TLC_GS.NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet)))|160|0|32|||||||||次TL0コマンド実行時刻|変数指定が大変なことに…|
TLC_GS.NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS]))->packet)))|164|0|16|||||||||次TL0コマンドID|変数指定が大変なことに…|
TLC_BC.COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])|166|0|32|||||||||TL1コマンドカウンタ||
TLC_BC.QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))|170|0|8|||||||||TL1登録コマンド数||
TLC_BC.LAST_EXEC_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.time.total_cycle)|171|0|32|||||||||最新TL1コマンド実行時刻||
TLC_BC.LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.code)|175|0|16|||||||||最新TL1コマンドID||
TLC_BC.LAST_EXEC_STS|int32_t|(int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev.sts)|177|0|32||7.1|||||||最新TL1コマンド実行結果||
TLC_BC.LAST_ERR_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.time.total_cycle)|181|0|32|||||||||最新TL1コマンド実行時刻||
TLC_BC.LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.code)|185|0|16|||||||||最新TL1コマンドID||
TLC_BC.LAST_ERR_STS|int32_t|(int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].prev_err.sts)|187|0|32||7.1|||||||最新TL1コマンド実行結果||
TLC_BC.ERR_COUNTER|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].error_counter)|191|0|32|||||||||地上局コマンド実行異常カウンタ||
TLC_BC.SOE_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].stop_on_error)|195|0|8||2.5|||||||||
TLC_BC.LOCKOUT_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_BC].lockout)|196|0|8||2.6|||||||||
TLC_BC.NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet)))|197|0|32|||||||||次TL1コマンド実行時刻|変数指定が大変なことに…|
TLC_BC.NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_BC]))->packet)))|201|0|16|||||||||次TL1コマンドID|変数指定が大変なことに…|
TLC_TLM.COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])|203|0|32|||||||||TL1コマンドカウンタ||
TLC_TLM.QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))|207|0|8|||||||||TL1登録コマンド数||
TLC_TLM.LAST_EXEC_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.time.total_cycle)|208|0|32|||||||||最新TL1コマンド実行時刻||
TLC_TLM.LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.code)|212|0|16|||||||||最新TL1コマンドID||
TLC_TLM.LAST_EXEC_STS|int32_t|(int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev.sts)|214|0|32||7.1|||||||最新TL1コマンド実行結果||
TLC_TLM.LAST_ERR_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.time.total_cycle)|218|0|32|||||||||最新TL1コマンド実行時刻||
TLC_TLM.LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.code)|222|0|16|||||||||最新TL1コマンドID||
TLC_TLM.LAST_ERR_STS|int32_t|(int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].prev_err.sts)|224|0|32||7.1|||||||最新TL1コマンド実行結果||
TLC_TLM.ERR_COUNTER|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].error_counter)|228|0|32|||||||||地上局コマンド実行異常カウンタ||
TLC_TLM.SOE_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].stop_on_error)|232|0|8||2.5|||||||||
TLC_TLM.LOCKOUT_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_DEPLOY_TLM].lockout)|233|0|8||2.6|||||||||
TLC_TLM.NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet)))|234|0|32|||||||||次TL1コマンド実行時刻|変数指定が大変なことに…|
TLC_TLM.NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_DEPLOY_TLM]))->packet)))|238|0|16|||||||||次TL1コマンドID|変数指定が大変なことに…|
TLC_MIS.COUNTER|uint32_t|PL_count_executed_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])|240|0|32|||||||||TL0コマンドカウンタ||
TLC_MIS.QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))|244|0|8|||||||||TL0登録コマンド数||
TLC_MIS.LAST_EXEC_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.time.total_cycle)|245|0|32|||||||||最新TL0コマンド実行時刻||
TLC_MIS.LAST_EXEC_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.code)|249|0|16|||||||||最新TL0コマンドID||
TLC_MIS.LAST_EXEC_STS|int32_t|(int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev.sts)|251|0|32||7.1|||||||最新TL0コマンド実行結果||
TLC_MIS.LAST_ERR_TIME|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.time.total_cycle)|255|0|32|||||||||最新TL0コマンド実行時刻||
TLC_MIS.LAST_ERR_ID|uint16_t|(uint16_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.code)|259|0|16|||||||||最新TL0コマンドID||
TLC_MIS.LAST_ERR_STS|int32_t|(int32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].prev_err.sts)|261|0|32||7.1|||||||最新TL0コマンド実行結果||
TLC_MIS.ERR_COUNTER|uint32_t|(uint32_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].error_counter)|265|0|32|||||||||地上局コマンド実行異常カウンタ||
TLC_MIS.SOE_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].stop_on_error)|269|0|8||2.5|||||||||
TLC_MIS.LOCKOUT_FLAG|uint8_t|(uint8_t)(timeline_command_dispatcher->dispatcher[TLCD_ID_FROM_GS_FOR_MISSION].lockout)|270|0|8||2.6|||||||||
TLC_MIS.NEXT_TIME|uint32_t|(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])) ? 0 : (uint32_t)CCP_get_ti((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))->packet)))|271|0|32|||||||||次TL0コマンド実行時刻|変数指定が大変なことに…|
TLC_MIS.NEXT_ID|uint16_t|(uint16_t)(PL_is_empty(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION])) ? 0 : CCP_get_id((const CommonCmdPacket*)(PL_get_head(&(PH_tl_cmd_list[TLCD_ID_FROM_GS_FOR_MISSION]))->packet)))|275|0|16|||||||||次TL0コマンドID|変数指定が大変なことに…|
BCT_BLK_PTR|uint8_t|(uint8_t)(block_command_table->pos.block)|277|0|8|||||||||BCT ブロックポインタ||
BCT_CMD_PTR|uint8_t|(uint8_t)(block_command_table->pos.cmd)|278|0|8|||||||||BCT コマンドポインタ||
BCT_REGD_TIME|uint32_t|((block_command_table->pos.cmd == 0) ? 0 : (uint32_t)BCT_get_ti(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1)))|279|0|32|||||||||BCT 登録コマンド時刻|変数指定が大変なことに…|
BCT_REGD_ID|uint16_t|(uint16_t)((block_command_table->pos.cmd == 0) ? 0 : BCT_get_id(block_command_table->pos.block, (uint8_t)(block_command_table->pos.cmd-1)))|283|0|16|||||||||BCT 登録コマンドID|変数指定が大変なことに…|
MS_BUFFER_AVAILABLE|uint8_t|gs_driver->ccsds_info.buffer_num|285|0|8|||||||||CCSDS MS VCDU 使用量||
MS_T2M_FLUSH_INTERVAL|uint32_t|(uint32_t)(DI_GS_ms_tlm_packet_handler->tc_packet_to_m_pdu.flush_interval)|286|0|32|||||||||TL0コマンドカウンタ||
RP_T2M_FLUSH_INTERVAL|uint32_t|(uint32_t)(DI_GS_rp_tlm_packet_handler->tc_packet_to_m_pdu.flush_interval)|290|0|32|||||||||TL0コマンドカウンタ||
MS_TLM_COUNTER|uint32_t|PL_count_executed_nodes(&PH_ms_tlm_list)|294|0|32|||||||||TL0コマンドカウンタ||
MS_TLM_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_ms_tlm_list))|298|0|8|||||||||TL0登録コマンド数||
ST_TLM_COUNTER|uint32_t|PL_count_executed_nodes(&PH_st_tlm_list)|299|0|32|||||||||TL0コマンドカウンタ||
ST_TLM_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_st_tlm_list))|303|0|8|||||||||TL0登録コマンド数||
RP_TLM_COUNTER|uint32_t|PL_count_executed_nodes(&PH_rp_tlm_list)|304|0|32|||||||||TL0コマンドカウンタ||
RP_TLM_QUEUED|uint8_t|(uint8_t)(PL_count_active_nodes(&PH_rp_tlm_list))|308|0|8|||||||||TL0登録コマンド数||
TCI_TX_CYCLE|uint32_t|(uint32_t)gs_driver->latest_info->tx.send_cycle|309|0|32|||||||||時刻校正情報::送出サイクル||
TCI_TX_BITRATE|uint32_t|gs_driver->driver_ccsds.ccsds_config.bitrate|313|0|32|||||||||時刻校正情報::送出ビットレート||
TCI_VCID|uint8_t|(uint8_t)gs_driver->latest_info->tx.vcid|317|0|8|||||||||時刻構成情報::送出VCID||
TCI_VCDU_COUNTER|uint32_t|gs_driver->latest_info->tx.vcdu_counter|318|0|32|||||||||時刻校正情報::送出VCDUカウンタ||
