/**
 * @file
 * @brief  コマンドの実行・登録，コマンド関連処理
 */
#ifndef COMMAND_ANALYZE_H_
#define COMMAND_ANALYZE_H_

#include "common_cmd_packet.h"
#include "common_cmd_packet_util.h"
#include <src_user/TlmCmd/command_definitions.h>

#define CA_TLM_PAGE_SIZE      (32)                                  //!< コマンドテーブルの1テレメトリパケット(=1ページ)に格納されるコマンド数（ページネーション用）
#define CA_TLM_PAGE_MAX       (48)                                  //!< コマンドテーブルページ数（ページネーション用）
#define CA_MAX_CMDS           (CA_TLM_PAGE_SIZE * CA_TLM_PAGE_MAX)  //!< コマンドテーブルサイズ．すなわち登録できる最大コマンド数
#define CA_MAX_CMD_PARAM_NUM  (6)                                   //!< コマンドパラメタ（引数）の最大数．現時点ではユーザー側で可変にすることを想定してはない

#include <src_user/Settings/TlmCmd/command_analyze_params.h>


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
 * @struct CA_PackedParamSizeInfo
 * @brief  パラメタのサイズ情報を圧縮した構造体
 * @note   2つのパラメタサイズ情報を uint8_t に押し込める
 * @note   値は CA_PARAM_SIZE_TYPE
 */
typedef struct
{
  union
  {
    uint8_t byte;
    struct
    {
      unsigned first: 4;
      unsigned second: 4;
    } bit;
  } packed_info;
} CA_PackedParamSizeInfo;

/**
 * @enum  CA_PARAM_SIZE_TYPE
 * @brief コマンドパラメタサイズ
 * @note  unsigned 4bit 変数を想定
 */
typedef enum
{
  CA_PARAM_SIZE_TYPE_NONE = 0,    //!< パラメタなし
  CA_PARAM_SIZE_TYPE_1BYTE,       //!< 1 byte
  CA_PARAM_SIZE_TYPE_2BYTE,       //!< 2 byte
  CA_PARAM_SIZE_TYPE_4BYTE,       //!< 4 byte
  CA_PARAM_SIZE_TYPE_8BYTE,       //!< 8 byte
  CA_PARAM_SIZE_TYPE_RAW = 0xf    //!< RAW パラメタ
} CA_PARAM_SIZE_TYPE;

/**
 * @struct CA_CmdInfo
 * @brief  コマンドテーブルの要素となる構造体
 */
typedef struct
{
  CCP_CmdRet (*cmd_func)(const CommonCmdPacket*);                           //!< コマンドとなる関数
  CA_PackedParamSizeInfo param_size_infos[(CA_MAX_CMD_PARAM_NUM + 1) / 2];  //!< パラメタサイズ情報
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
 * @return CCP_CmdRet
 */
CCP_CmdRet CA_execute_cmd(const CommonCmdPacket* packet);

/**
 * @brief  コマンドパラメタ数を取得する
 * @param  cmd_code:  チェックするコマンドのID
 * @return コマンドパラメタ数
 * @note   不正な引数の場合は 0 を返す
 */
uint8_t CA_get_cmd_param_num(CMD_CODE cmd_code);

/**
 * @brief  コマンドパラメタサイズを取得する
 * @param  cmd_code:  チェックするコマンドのID
 * @param  n: N番目のパラメタ （0起算）
 * @return コマンドパラメタサイズ
 * @note   不正な引数の場合は 0 を返す
 */
uint8_t CA_get_cmd_param_size(CMD_CODE cmd_code, uint8_t n);

/**
 * @brief  最小コマンドパラメタ長を取得する
 * @param  cmd_code:  チェックするコマンドのID
 * @return コマンドパラメタ長
 * @note   不正な引数の場合は 0 を返す
 * @note   RAW パラメタはサイズ 0 として計算するため，最小コマンドパラメタ長となる
 */
uint16_t CA_get_cmd_param_min_len(CMD_CODE cmd_code);

/**
 * @brief  コマンドパラメタ長をチェックする
 * @param  cmd_code:  チェックするコマンドのID
 * @param  param_len: パラメタ長
 * @return CA_ACK
 * @note   不正な引数の場合は CA_ACK_ERR を返す
 */
CA_ACK CA_ckeck_cmd_param_len(CMD_CODE cmd_code, uint16_t param_len);

/**
 * @brief  RAW パラメタを持っているか？
 * @param  cmd_code:  チェックするコマンドのID
 * @retval 1: RAW パラメタあり
 * @retval 0: RAW パラメタなし or 不正な引数
 */
int CA_has_raw_param(CMD_CODE cmd_code);

/**
 * @brief  Cmd Tableのロード
 * @note   定義は /src_user/TlmCmd/CommandDefinitions.c にある
 * @param  cmd_table: Cmd Tableの実体
 * @return void
 */
void CA_load_cmd_table(CA_CmdInfo cmd_table[CA_MAX_CMDS]);

CCP_CmdRet Cmd_CA_INIT(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_CA_REGISTER_CMD(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_CA_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);

#endif
