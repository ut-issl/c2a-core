# SAMPLE_AOBC_CMD_DB_CMD_DB

## CDH

Name|Code|Params|Param1||Param2||Param3||Param4||Param5||Param6||Danger|Restricted|Desc.|Note
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-

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
Cmd_TLCD_SET_LINE_NO_FOR_TIMELINE_TLM|0x14|1|uint8_t||||||||||||||TLC状況一覧更新||
Cmd_TLCD_DEPLOY_BLOCK|0x15|2|uint8_t||uint16_t||||||||||||BLC展開 ||
Cmd_TLCD_SET_SOE_FLAG|0x16|2|uint8_t||uint8_t||||||||||||異常時実行停止機能設定(ライン番号, フラグ)||
Cmd_TLCD_SET_LOUT_FLAG|0x17|2|uint8_t||uint8_t||||||||||||実行停止機能設定(ライン番号, フラグ)||
Cmd_TLCD_SET_PAGE_FOR_TLM|0x18|1|uint8_t||||||||||||||テレメトリ生成用ページ番号設定||
Cmd_TLCD_CLEAR_ERR_LOG|0x19|1|uint8_t||||||||||||||最新のコマンド実行異常記録をクリア||
Cmd_GENERATE_TLM|0x1a|3|uint8_t||uint8_t||uint8_t||||||||||TLMパケット生成||

