# SAMPLE_MOBC_CMD_DB_CMD_DB

## POWER

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

## COM

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

## MISSION

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

## PROP

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

## AOCS

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

## Thermal

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

## Trajectory

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

## HILS

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

## C2A_CORE

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
Cmd_NOP|0x0|0|||||||||||||||ダミーコマンド||
Cmd_TMGR_SET_TIME|0x1|1|uint32_t||||||||||||||MOBC時刻設定コマンド||
Cmd_TMGR_UPDATE_UNIXTIME|0x2|3|double||uint32_t||uint32_t||||||||||MOBC UNIXTIME修正コマンド||
Cmd_TMGR_SET_UTL_UNIXTIME_EPOCH|0x3|1|double||||||||||||||UTL_cmdで用いるunixtimeの紀元を変更する|utl_unixtime_epoch [s]|
Cmd_TMGR_SET_CYCLE_CORRECTION|0x4|1|double||||||||||||||CYCLES_PER_SECの補正倍率を変更する, 初期値は1.0||
Cmd_TMGR_RESET_CYCLE_CORRECTION|0x5|0|||||||||||||||CYCLES_PER_SECの補正倍率を1.0に初期化する||
Cmd_TMGR_CLEAR_UNIXTIME_INFO|0x6|0|||||||||||||||unixtime_info_ を初期化する||
Cmd_AM_REGISTER_APP|0x7|3|uint32_t||uint32_t||uint32_t||||||||||アプリ登録コマンド||
Cmd_AM_INITIALIZE_APP|0x8|1|uint32_t||||||||||||||アプリ初期化コマンド||
Cmd_AM_EXECUTE_APP|0x9|1|uint32_t||||||||||||||アプリ実行コマンド||
Cmd_AM_SET_PAGE_FOR_TLM|0xa|1|uint8_t||||||||||||||テレメトリ生成用ページ番号設定||
Cmd_AM_CLEAR_APP_INFO|0xb|0|||||||||||||o||アプリ実行時間計測の初期化||
Cmd_MM_SET_MODE_LIST|0xc|2|uint8_t||uint16_t||||||||||||モード定義リスト設定コマンド||
Cmd_MM_SET_TRANSITION_TABLE|0xd|3|uint8_t||uint8_t||uint16_t||||||||||モード遷移定義テーブル書き換えコマンド||
Cmd_MM_START_TRANSITION|0xe|1|uint8_t||||||||||||||モード遷移開始コマンド||
Cmd_MM_FINISH_TRANSITION|0xf|0|||||||||||||||モード遷移終了通知コマンド||
Cmd_MM_UPDATE_TRANSITION_TABLE_FOR_TLM|0x10|0|||||||||||||||モードのTLMテーブル情報更新||
Cmd_TDSP_SET_TASK_LIST|0x11|1|uint8_t||||||||||||||タスクリスト設定コマンド||
Cmd_TLCD_CLEAR_ALL_TIMELINE|0x12|1|uint8_t||||||||||||o||全TLC登録解除||
Cmd_TLCD_CLEAR_TIMELINE_AT|0x13|2|uint8_t||uint32_t||||||||||||TI指定TLC登録解除||
Cmd_TLCD_DEPLOY_BLOCK|0x14|2|uint8_t||uint16_t||||||||||||BLC展開 ||
Cmd_TLCD_CLEAR_ERR_LOG|0x15|1|uint8_t||||||||||||||最新のコマンド実行異常記録をクリア||
Cmd_TLCD_SET_SOE_FLAG|0x16|2|uint8_t||uint8_t||||||||||||異常時実行停止機能設定(ライン番号, フラグ)||
Cmd_TLCD_SET_LOUT_FLAG|0x17|2|uint8_t||uint8_t||||||||||||実行停止機能設定(ライン番号, フラグ)||
Cmd_TLCD_SET_ID_FOR_TLM|0x18|1|uint8_t||||||||||||||テレメで降ろすTL_IDの設定とTLテレメの更新||
Cmd_TLCD_SET_PAGE_FOR_TLM|0x19|1|uint8_t||||||||||||||テレメトリ生成用ページ番号設定||
Cmd_GENERATE_TLM|0x1a|3|uint8_t||uint8_t||uint8_t||||||||||TLMパケット生成||
Cmd_BCT_CLEAR_BLOCK|0x1b|1|uint16_t||||||||||||||BLC登録解除コマンド||
Cmd_BCT_SET_BLOCK_POSITION|0x1c|2|uint16_t||uint8_t||||||||||||BLC操作ポインタ設定コマンド||
Cmd_BCT_COPY_BCT|0x1d|2|uint16_t||uint16_t||||||||||||BCT の内容を別の場所にコピー||
Cmd_BCT_OVERWRITE_CMD|0x1e|5|uint16_t||uint32_t||uint16_t||uint8_t||raw||||o||BCTの内容を上書き||
Cmd_BCT_FILL_NOP|0x1f|1|uint8_t||||||||||||||テレメ運用時の10個BCをNOP埋めする．（NOP個数）||
Cmd_BCE_ACTIVATE_BLOCK|0x20|0|||||||||||||||BLC有効化コマンド||
Cmd_BCE_ACTIVATE_BLOCK_BY_ID|0x21|1|uint16_t||||||||||||||BCを（無条件に）有効化||
Cmd_BCE_INACTIVATE_BLOCK_BY_ID|0x22|1|uint16_t||||||||||||||BCを（無条件に）無効化||
Cmd_BCE_ROTATE_BLOCK|0x23|1|uint16_t||||||||||||||BLC回転コマンド||
Cmd_BCE_COMBINE_BLOCK|0x24|1|uint16_t||||||||||||||BLC融合コマンド||
Cmd_BCE_TIMELIMIT_COMBINE_BLOCK|0x25|2|uint16_t||uint8_t||||||||||||時間制限付きBLC融合コマンド||
Cmd_BCE_RESET_ROTATOR_INFO|0x26|0|||||||||||||o||カウンタ類リセット||
Cmd_BCE_RESET_COMBINER_INFO|0x27|0|||||||||||||o||カウンタ類リセット||
Cmd_BCE_SET_ROTATE_INTERVAL|0x28|2|uint16_t||uint16_t||||||||||||BC回転時の回転間隔周期を設定する||
Cmd_GSCD_CLEAR_ERR_LOG|0x29|0|||||||||||||||最新のコマンド実行異常記録をクリア||
Cmd_RTCD_CLEAR_ALL_REALTIME|0x2a|0|||||||||||||o||全RTC登録解除||
Cmd_RTCD_CLEAR_ERR_LOG|0x2b|0|||||||||||||||最新のコマンド実行異常記録をクリア||
Cmd_MEM_SET_REGION|0x2c|2|uint32_t||uint32_t||||||||||||始点アドレスと終点アドレスを指定する||
Cmd_MEM_LOAD|0x2d|2|uint32_t||raw||||||||||||開始アドレス、データを指定|第2パラメータの指定は仮設定|
Cmd_MEM_SET_DESTINATION|0x2e|1|uint32_t||||||||||||||コピー先設定||
Cmd_MEM_COPY_REGION_SEQ|0x2f|1|uint32_t||||||||||||||領域コピー||
Cmd_AL_ADD_ANOMALY|0x30|2|uint32_t||uint32_t||||||||||||アノマリーコード(group, local)を発生させる||
Cmd_AL_CLEAR_LIST|0x31|0|||||||||||||||AnomalyListをクリアする||
Cmd_AL_SET_PAGE_FOR_TLM|0x32|1|uint8_t||||||||||||||テレメトリ生成用ページ番号設定||
Cmd_AL_INIT_LOGGING_ENA_FLAG|0x33|0|||||||||||||||ロギングのフラグを初期化（全有効化）||
Cmd_AL_ENABLE_LOGGING|0x34|1|uint32_t||||||||||||||ロギングを有効化||
Cmd_AL_DISABLE_LOGGING|0x35|1|uint32_t||||||||||||||ロギングを無効化||
Cmd_AL_SET_THRES_OF_NEARLY_FULL|0x36|1|uint16_t||||||||||||||AL_NEARLY_FULLアノマリを出すしきい値設定||
Cmd_AH_REGISTER_RULE|0x37|6|uint8_t||uint8_t||uint8_t||uint8_t||uint8_t||uint16_t||||AHパラメタを設定する||
Cmd_AH_ACTIVATE_RULE|0x38|1|uint8_t||||||||||||||idで指定したルールを有効化する||
Cmd_AH_INACTIVATE_RULE|0x39|1|uint8_t||||||||||||||idで指定したルールを無効化する||
Cmd_AH_CLEAR_LOG|0x3a|0|||||||||||||||アノマリ記録と関連情報を初期化する||
Cmd_AH_SET_PAGE_FOR_TLM|0x3b|1|uint8_t||||||||||||||テレメトリ生成用ページ番号設定||
Cmd_AHRES_LOG_CLEAR|0x3c|0|||||||||||||||アノマリ対処ログをクリア||
Cmd_AHRES_LOG_SET_PAGE_FOR_TLM|0x3d|1|uint8_t||||||||||||||アノマリ対処ログテレメトリ生成用ページ番号設定||
Cmd_EL_INIT|0x3e|0|||||||||||||||EL初期化||
Cmd_EL_CLEAR_LOG_ALL|0x3f|0|||||||||||||||全ログ削除||
Cmd_EL_CLEAR_LOG_BY_ERR_LEVEL|0x40|1|uint8_t||||||||||||||エラーレベル指定ログ削除||
Cmd_EL_CLEAR_STATISTICS|0x41|0|||||||||||||||統計情報削除||
Cmd_EL_CLEAR_TLOG|0x42|1|uint8_t||||||||||||||TLog削除||
Cmd_EL_CLEAR_CLOG|0x43|1|uint8_t||||||||||||||CLog削除||
Cmd_EL_RECORD_EVENT|0x44|4|uint32_t||uint32_t||uint8_t||uint32_t||||||||イベント記録||
Cmd_EL_TLOG_SET_PAGE_FOR_TLM|0x45|2|uint8_t||uint8_t||||||||||||TLogのダウンリンクテレメ設定||
Cmd_EL_CLOG_SET_PAGE_FOR_TLM|0x46|2|uint8_t||uint8_t||||||||||||CLogのダウンリンクテレメ設定||
Cmd_EL_INIT_LOGGING_SETTINGS|0x47|0|||||||||||||||ログ設定を初期化||
Cmd_EL_ENABLE_LOGGING|0x48|1|uint32_t||||||||||||||ロギングを有効化||
Cmd_EL_DISABLE_LOGGING|0x49|1|uint32_t||||||||||||||ロギングを無効化||
Cmd_EL_ENABLE_LOGGING_ALL|0x4a|0|||||||||||||||すべてのロギングを有効化||
Cmd_EL_DISABLE_LOGGING_ALL|0x4b|0|||||||||||||||すべてのロギングを無効化||
Cmd_EL_ENABLE_TLOG_OVERWRITE|0x4c|1|uint8_t||||||||||||||TLogのログ上書きを有効化||
Cmd_EL_DISABLE_TLOG_OVERWRITE|0x4d|1|uint8_t||||||||||||||TLogのログ上書きを無効化||
Cmd_EL_ENABLE_TLOG_OVERWRITE_ALL|0x4e|0|||||||||||||||すべてのTLogのログ上書きを有効化||
Cmd_EL_DISABLE_TLOG_OVERWRITE_ALL|0x4f|0|||||||||||||||すべてのTLogのログ上書きを無効化||
Cmd_EH_INIT|0x50|0|||||||||||||||EHの初期化||
Cmd_EH_CLEAR_ALL_RULE|0x51|0|||||||||||||||EHのすべてのルールをクリア||
Cmd_EH_LOAD_DEFAULT_RULE|0x52|0|||||||||||||||EHのデフォルトルールを読み込み||
Cmd_EH_SET_REGISTER_RULE_EVENT_PARAM|0x53|6|uint16_t||uint32_t||uint32_t||uint8_t||uint8_t||uint16_t||||登録するEHルールのイベントパラメータを設定||
Cmd_EH_SET_REGISTER_RULE_CONDITION_PARAM|0x54|4|uint8_t||uint16_t||uint32_t||uint8_t||||||||登録するEHルールの対応条件パラメタを設定||
Cmd_EH_REGISTER_RULE|0x55|0|||||||||||||||設定したパラメタでEHルールを登録||
Cmd_EH_DELETE_RULE|0x56|1|uint16_t||||||||||||||指定したルールを削除||
Cmd_EH_INIT_RULE|0x57|1|uint16_t||||||||||||||指定したルールを初期化（ルールを有効化し，ルールカウンタも初期化）||
Cmd_EH_INIT_RULE_FOR_MULTI_LEVEL|0x58|1|uint16_t||||||||||||||指定したルールを初期化（ルールを有効化し，ルールカウンタも初期化）（多段EH対応用）||
Cmd_EH_ACTIVATE_RULE|0x59|1|uint16_t||||||||||||||指定したルールを有効化||
Cmd_EH_INACTIVATE_RULE|0x5a|1|uint16_t||||||||||||||指定したルールを無効化||
Cmd_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL|0x5b|1|uint16_t||||||||||||||指定したルールを有効化（多段EH対応用）||
Cmd_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL|0x5c|1|uint16_t||||||||||||||指定したルールを無効化（多段EH対応用）||
Cmd_EH_SET_RULE_COUNTER|0x5d|2|uint16_t||uint16_t||||||||||||EH_Rule の counter をセット||
Cmd_EH_CLEAR_RULE_COUNTER|0x5e|1|uint16_t||||||||||||||EH_Rule の counter を0 クリア||
Cmd_EH_CLEAR_RULE_COUNTER_BY_EVENT|0x5f|3|uint32_t||uint32_t||uint8_t||||||||||EH_Rule の counter を EL_Event 指定で 0 クリア||
Cmd_EH_CLEAR_LOG|0x60|0|||||||||||||||EHの対応ログをクリア||
Cmd_EH_SET_MAX_RESPONSE_NUM|0x61|1|uint8_t||||||||||||||一度のEH実行で対応するイベントの最大数を設定||
Cmd_EH_SET_MAX_CHECK_EVENT_NUM|0x62|1|uint16_t||||||||||||||一度のEH実行でチェックするイベントの最大数を設定||
Cmd_EH_SET_MAX_MULTI_LEVEL_NUM|0x63|1|uint8_t||||||||||||||設定可能な最大段数を設定||
Cmd_EH_SET_PAGE_OF_RULE_TABLE_FOR_TLM|0x64|1|uint8_t||||||||||||||EHルールテーブルのテレメページの設定||
Cmd_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM|0x65|1|uint8_t||||||||||||||EHルールのソート済みテーブルのテレメページの設定||
Cmd_EH_SET_PAGE_OF_LOG_TABLE_FOR_TLM|0x66|1|uint8_t||||||||||||||EHログテーブルのテレメページの設定||
Cmd_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM|0x67|1|uint16_t||||||||||||||EHテレメに表示するルールのID設定||
Cmd_EH_MATCH_EVENT_COUNTER_TO_EL|0x68|0|||||||||||||||イベント発生を検知するためのEHカウンタをELカウンタに強制定期に合わせる||
Cmd_EVENT_UTIL_ENABLE_EH_EXEC|0x69|0|||||||||||||||EHの実行を有効に||
Cmd_EVENT_UTIL_DISABLE_EH_EXEC|0x6a|0|||||||||||||||EHの実行を無効に||
Cmd_EVENT_UTIL_EXEC_EH|0x6b|0|||||||||||||||EHを単発で実行||
Cmd_TF_INIT|0x6c|0|||||||||||||||TFの初期化||
Cmd_CA_INIT|0x6d|0|||||||||||||||CAの初期化||
Cmd_TF_REGISTER_TLM|0x6e|2|uint8_t||uint32_t||||||||||||テレメトリ登録コマンド||
Cmd_CA_REGISTER_CMD|0x6f|3|uint16_t||uint32_t||raw||||||||||コマンド登録コマンド||
Cmd_TF_SET_PAGE_FOR_TLM|0x70|1|uint8_t||||||||||||||テレメトリ定義テーブルページ番号設定||
Cmd_CA_SET_PAGE_FOR_TLM|0x71|1|uint8_t||||||||||||||コマンド定義テーブルページ番号設定||
Cmd_TLM_MGR_INIT|0x72|0|||||||||||||||初期化||
Cmd_TLM_MGR_INIT_MASTER_BC|0x73|0|||||||||||||||master bc の初期化||
Cmd_TLM_MGR_CLEAR_HK_TLM|0x74|0|||||||||||||||HKテレメを初期化||
Cmd_TLM_MGR_CLEAR_SYSTEM_TLM|0x75|0|||||||||||||||systemテレメを初期化||
Cmd_TLM_MGR_CLEAR_USER_TLM|0x76|0|||||||||||||||high_freq_tlm, low_freq_tlm を初期化||
Cmd_TLM_MGR_START_TLM|0x77|0|||||||||||||||TLM送出開始||
Cmd_TLM_MGR_STOP_TLM|0x78|0|||||||||||||||TLM送出一時停止||
Cmd_TLM_MGR_CLEAR_TLM_TL|0x79|0|||||||||||||||TLM送出用TLをクリア||
Cmd_TLM_MGR_REGISTER_HK_TLM|0x7a|3|uint8_t||uint8_t||uint8_t||||||||||HKテレメを登録||
Cmd_TLM_MGR_REGISTER_SYSTEM_TLM|0x7b|3|uint8_t||uint8_t||uint8_t||||||||||systemテレメを登録||
Cmd_TLM_MGR_REGISTER_HIGH_FREQ_TLM|0x7c|3|uint8_t||uint8_t||uint8_t||||||||||high_freq_tlm を登録||
Cmd_TLM_MGR_REGISTER_LOW_FREQ_TLM|0x7d|3|uint8_t||uint8_t||uint8_t||||||||||low_freq_tlm を登録||
Cmd_DCU_ABORT_CMD|0x7e|1|uint16_t||||||||||||||実行中の分割コマンドを停止する||
Cmd_DCU_DOWN_ABORT_FLAG|0x7f|1|uint16_t||||||||||||||エラー，またはコマンドによって中断ステータスとなっているコマンドを，実行可能状態に戻す||
Cmd_DCU_CLEAR_LOG|0x80|0|||||||||||||||ログをクリアする||

## CDH

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
Cmd_DI_GS_CCSDS_TX_START|0x100|0|||||||||||||||CCSDS 通信を有効に||
Cmd_DI_GS_CCSDS_TX_STOP|0x101|0|||||||||||||o||CCSDS通信を無効に||
Cmd_DI_GS_DRIVER_RESET|0x102|0|||||||||||||||MS VCDUの排出間隔設定||
Cmd_DI_GS_SET_MS_FLUSH_INTERVAL|0x103|1|uint32_t||||||||||||||MS VCDUの排出間隔設定||
Cmd_DI_GS_SET_RP_FLUSH_INTERVAL|0x104|1|uint32_t||||||||||||||RP VCDUの排出間隔設定||
Cmd_DI_GS_SET_FARM_PW|0x105|1|uint8_t||||||||||||||FARM-1のPositive Window Width設定||
Cmd_DI_GS_SET_INFO|0x106|1|uint8_t||||||||||||||GS送信TLMが0ならCCSDSに, 1ならUARTの情報が出てくる||
Cmd_DI_GS_CCSDS_GET_BUFFER|0x107|0|||||||||||||||||
Cmd_DI_GS_CCSDS_SET_RATE|0x108|1|uint8_t||||||||||||o||40M/パラメータ が bps になる．初期値は 0xAD で 230.4 Kbps (微妙に対応していない)||
Cmd_WDT_INIT|0x109|0|||||||||||||||WDT初期化．WDTがアプリイニシャライザを持たなくなったので追加||
Cmd_WDT_ENABLE|0x10a|0|||||||||||||||WDT有効化||
Cmd_WDT_DISABLE|0x10b|0|||||||||||||||WDT無効化||
Cmd_WDT_STOP_CLEAR|0x10c|0|||||||||||||o||WDTのカウンタクリアを止める＝リセット実行||
Cmd_WDT_START_CLEAR|0x10d|0|||||||||||||||WDTのカウンタクリアを開始する||
Cmd_UART_TEST_INIT_DI|0x10e|0|||||||||||||||DI初期化||
Cmd_UART_TEST_UPDATE|0x10f|0|||||||||||||||DI update||
Cmd_UART_TEST_SEND_TEST|0x110|1|uint8_t||||||||||||||DI send test||

## Other

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-
Cmd_UTIL_CMD_ADD|0x3e0|6|uint8_t||uint32_t||uint32_t||uint32_t||uint32_t||uint32_t||||汎用コマンドのバッファに指定サイズのデータを入れる||
Cmd_UTIL_CMD_SEND|0x3e1|1|uint8_t||||||||||||||引数のCHに汎用コマンドのバッファにあるデータを送信する||
Cmd_UTIL_CMD_RESET|0x3e2|0|||||||||||||||汎用コマンドのバッファをクリアする||
Cmd_UTIL_COUNTER_INCREMENT|0x3e3|1|uint8_t||||||||||||||引数の汎用カウンタをインクリメントする||
Cmd_UTIL_COUNTER_RESET|0x3e4|1|uint8_t||||||||||||||引数の汎用カウンタをリセットする．0xffでALL_CLR||
Cmd_UTIL_COUNTER_SET_PARAM|0x3e5|3|uint8_t||uint32_t||uint8_t||||||||||引数の汎用カウンタの閾値・アノマリ発行有無を設定する||

