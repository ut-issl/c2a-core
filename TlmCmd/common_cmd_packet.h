/**
 * @file
 * @brief  CCP 関連基本関数の宣言
 * @note   CCP:  CommonCmdPacket
 */
#ifndef COMMON_CMD_PACKET_H_
#define COMMON_CMD_PACKET_H_

#include "../System/TimeManager/obc_time.h"
#include <src_user/TlmCmd/command_definitions.h>

// ここで CCP_DEST_TYPE を定義する
// 詳細は /Examples/minimum_user/src/src_user/Settings/TlmCmd/common_cmd_packet_define.h 参照
/* 例
typedef enum
{
  CCP_DEST_TYPE_TO_ME     = 0,
  CCP_DEST_TYPE_TO_MOBC   = 1,
  CCP_DEST_TYPE_TO_AOBC   = 2,
  CCP_DEST_TYPE_TO_TOBC   = 3,
  CCP_DEST_TYPE_TO_UNKOWN = 4
} CCP_DEST_TYPE;
*/
// さらに， CCP_APID_TO_ME, CCP_MAX_LEN, CommonCmdPacket として使うパケット型を指定する
#include <src_user/Settings/TlmCmd/common_cmd_packet_define.h>

// ここで APID を定義する
// 詳細は common_tlm_cmd_packet.h を参照
#include <src_user/Settings/TlmCmd/Ccsds/apid_define.h>

// TL_MIS を有効にするかどうか
#include <src_user/Settings/Applications/timeline_command_dispatcher_define.h>

/**
 * @enum   CCP_EXEC_STS
 * @brief  コマンド実行結果コード
 * @note   uint8_t を想定
 */
typedef enum
{
  CCP_EXEC_SUCCESS = 0,         /*!< コマンド実行成功
                                     なお，コマンドの実行の結果なので，その後の処理が失敗しても，コマンドが実行されていればこれを返す */
  CCP_EXEC_ILLEGAL_LENGTH,      //!< コマンド実行時のコマンド引数長エラー
  CCP_EXEC_ILLEGAL_PARAMETER,   //!< コマンド実行時のパラメタエラー
  CCP_EXEC_ILLEGAL_CONTEXT,     //!< コマンド実行時のその他のエラー
  CCP_EXEC_CMD_NOT_DEFINED,     //!< cmdExec で用いる
  CCP_EXEC_ROUTING_FAILED,      //!< command router で用いる
  CCP_EXEC_PACKET_FMT_ERR,      //!< packet handler, ccp util で用いる
  CCP_EXEC_UNKNOWN              //!< 内部処理用．使わない．
} CCP_EXEC_STS;

/**
 * @enum   CCP_EXEC_TYPE
 * @brief  コマンド実行種別
 * @note   4bit を想定
 * @note   GS (WINGS) からの OBC_TL や， C2A 内の CCP_form_tlc はすべて TL0 扱いとすること！
 */
typedef enum
{
  CCP_EXEC_TYPE_GS,              //!< GS : Ground Station Command
  CCP_EXEC_TYPE_TL_FROM_GS,      //!< TL : Timeline Command
  CCP_EXEC_TYPE_BC,              //!< BC : Block Command
  CCP_EXEC_TYPE_RT,              //!< RT : Realtime Command
  CCP_EXEC_TYPE_UTL,             //!< UTL: Unixtime Timeline Command
  CCP_EXEC_TYPE_TL_DEPLOY_BC,
  CCP_EXEC_TYPE_TL_DEPLOY_TLM,
#ifdef TLCD_ENABLE_MISSION_TL
  CCP_EXEC_TYPE_TL_FOR_MISSION,  //!< TL_MIS  : Timeline Command for Mission
  CCP_EXEC_TYPE_UTL_FOR_MISSION, //!< UTL_MIS : Unixtime Timeline Command for Mission
#endif
  CCP_EXEC_TYPE_UNKNOWN
} CCP_EXEC_TYPE;

/**
 * @struct CCP_CmdRet
 * @brief  コマンド返り値
 */
typedef struct
{
  CCP_EXEC_STS exec_sts;    //!< CCP_EXEC_STS．Cmd の統一的なエラーコード
  uint32_t     err_code;    //!< 各 Cmd ユニークなエラーコード．各 App で定義する enum などを入れることを想定．
} CCP_CmdRet;


/**
 * @brief  有効なパケットかチェックする
 * @note   NULL チェックも行う
 * @note   OBC の外から来たパケットな可能性もあるので，このC2Aで扱えるサイズかもチェックする
 * @param  packet: CCP
 * @retval 1: True
 * @retval 0: False
 */
int CCP_is_valid_packet(const CommonCmdPacket* packet);

/**
 * @brief  TI を取得
 * @param  packet: CCP
 * @return ti
 */
cycle_t CCP_get_ti(const CommonCmdPacket* packet);

/**
 * @brief  TI を設定
 * @param[in,out] packet: CCP
 * @param[in]     ti: TI
 * @return void
 */
void CCP_set_ti(CommonCmdPacket* packet, cycle_t ti);

/**
 * @brief  APID を取得
 * @param  packet: CCP
 * @return APID
 */
APID CCP_get_apid(const CommonCmdPacket* packet);

/**
 * @brief  APID を設定
 * @param[in,out] packet: CCP
 * @param[in]     apid: APID
 * @return void
 */
void CCP_set_apid(CommonCmdPacket* packet, APID apid);

/**
 * @brief  EXEC_TYPE を取得
 * @param  packet: CCP
 * @return CCP_EXEC_TYPE
 */
CCP_EXEC_TYPE CCP_get_exec_type(const CommonCmdPacket* packet);

/**
 * @brief  EXEC_TYPE を設定
 * @param[in,out] packet: CCP
 * @param[in]     type: EXEC_TYPE
 * @return void
 */
void CCP_set_exec_type(CommonCmdPacket* packet, CCP_EXEC_TYPE type);

/**
 * @brief  DEST_TYPE を取得
 * @param  packet: CCP
 * @return CCP_DEST_TYPE
 */
CCP_DEST_TYPE CCP_get_dest_type(const CommonCmdPacket* packet);

/**
 * @brief  DEST_TYPE を設定
 * @param[in,out] packet: CCP
 * @param[in]     type: DEST_TYPE
 * @return void
 */
void CCP_set_dest_type(CommonCmdPacket* packet, CCP_DEST_TYPE type);

/**
 * @brief  CMD ID を取得
 * @param  packet: CCP
 * @return CMD_CODE
 */
CMD_CODE CCP_get_id(const CommonCmdPacket* packet);

/**
 * @brief  CMD ID を設定
 * @param[in,out] packet: CCP
 * @param[in]     id: CMD_CODE
 * @return void
 */
void CCP_set_id(CommonCmdPacket* packet, CMD_CODE id);

/**
 * @brief  パケット長を取得
 * @param  packet: CCP
 * @return packet len
 */
uint16_t CCP_get_packet_len(const CommonCmdPacket* packet);

/**
 * @brief  CMD パラメタ長を取得
 * @param  packet: CCP
 * @return param len
 */
uint16_t CCP_get_param_len(const CommonCmdPacket* packet);

/**
 * @brief  CMD パラメタ長の最大値を取得
 * @param  void
 * @return max param len
 */
uint16_t CCP_get_max_param_len(void);

/**
 * @brief  先頭のパラメタのポインタを取得
 * @param  packet: CCP
 * @return *param_head
 */
const uint8_t* CCP_get_param_head(const CommonCmdPacket* packet);

/**
 * @brief  パラメタを設定
 * @param[in,out] packet: CCP
 * @param[in]     param: パラメタ配列のポインタ
 * @param[in]     len:   パラメタ長
 * @return void
 */
void CCP_set_param(CommonCmdPacket* packet, const uint8_t* param, uint16_t len);

/**
 * @brief  共通部分の Header を構築
 * @param[in,out] packet: CCP
 * @return void
 */
void CCP_set_common_hdr(CommonCmdPacket* packet);

/**
 * @brief  CommonCmdPacket をコピー
 * @note   user 実装次第だが，高速化のためすすべてではなく必要最低限しかコピーしない場合もある
 * @param[in,out] dest: コピー先 CCP
 * @param[in]     src:  コピー元 CCP
 * @return void
 */
void CCP_copy_packet(CommonCmdPacket* dest, const CommonCmdPacket* src);

#endif
