/**
 * @file   command_analyze.h
 * @brief  コマンドの実行・登録，コマンド関連処理
 */
#ifndef COMMAND_ANALYZE_H_
#define COMMAND_ANALYZE_H_

#include "common_tlm_cmd_packet.h"
#include <src_user/CmdTlm/command_definitions.h>

#define CA_TLM_PAGE_SIZE  (64)                                   //!< コマンドテーブルの1テレメトリパケット(=1ページ)に格納されるコマンド数（ページネーション用）
#define CA_TLM_PAGE_MAX   (24)                                   //!< コマンドテーブルページ数（ページネーション用）
#define CA_MAX_CMDS       (CA_TLM_PAGE_SIZE * CA_TLM_PAGE_MAX)   //!< コマンドテーブルサイズ．すなわち登録できる最大コマンド数

#include <src_user/Settings/CmdTlm/command_analyze_params.h>

/**
 * @enum  AC_ACK
 * @brief CA の汎用返り値
 * @note  uint8_t を想定
 */
typedef enum
{
  CA_ACK_OK = 0,  //!< 正常終了
  CA_ACK_ERR      //!< エラー
} CA_ACK;

/**
 * @enum  CA_CMD_PARAM_LEN_TYPE
 * @brief コマンドのパラメタ長データの種別
 * @note  uint8_tを想定
 */
typedef enum
{
  CA_CMD_PARAM_LEN_TYPE_FIXED,        //!< パラメタ固定長
  CA_CMD_PARAM_LEN_TYPE_LOWER_LIMIT,  //!< パラメタ長の最小値指定（raw paramが含まれる時）
  CA_CMD_PARAM_LEN_TYPE_UNKNOWN       //!< 不明
} CA_CMD_PARAM_LEN_TYPE;

/**
 * @struct CA_CmdInfo
 * @brief  コマンドテーブルの要素となる構造体
 */
typedef struct
{
  CCP_EXEC_STS (*cmd_func)(const CTCP*);  //!< コマンドとなる関数
  CA_CMD_PARAM_LEN_TYPE param_len_type;   //!< パラメタ長の種別
  uint16_t param_len;                     //!< パラメタ長
} CA_CmdInfo;

/**
 * @struct CommandAnalyze
 * @brief  CommandAnalyze の Info 構造体
 */
typedef struct
{
  CA_CmdInfo cmd_table[CA_MAX_CMDS];  //!< コマンドテーブル
  uint8_t tlm_page_no;                //!< テレメで使うページ数
} CommandAnalyze;

extern const CommandAnalyze* const command_analyze;


/**
 * @brief  CA の初期化
 * @param  void
 * @return void
 */
void CA_initialize(void);

/**
 * @brief  コマンド実行の本体
 * @param  packet: 実行するコマンド
 * @return CCP_EXEC_STS
 */
CCP_EXEC_STS CA_execute_cmd(const CTCP* packet);

/**
 * @brief  コマンドパラメタ長をチェックする
 * @param  cmd_code:  チェックするコマンドのID
 * @param  param_len: パラメタ長
 * @return CA_ACK
 */
CA_ACK CA_ckeck_cmd_param_len(CMD_CODE cmd_code, uint16_t param_len);

/**
 * @brief  Cmd Tableのロード
 * @note   定義は /src_user/CmdTlm/CommandDefinitions.c にある
 * @param  cmd_table: Cmd Tableの実体
 * @return void
 */
void CA_load_cmd_table(CA_CmdInfo cmd_table[CA_MAX_CMDS]);

CCP_EXEC_STS Cmd_CA_REGISTER_CMD(const CTCP* packet);

CCP_EXEC_STS Cmd_CA_SET_PAGE_FOR_TLM(const CTCP* packet);

#endif
