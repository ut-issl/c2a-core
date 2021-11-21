/**
 * @file   common_tlm_cmd_packet.h
 * @brief  CTCP の宣言
 * @note   CTCP: Common Tlm Cmd Packet
 * @note   CCP:  Common Cmd Packet
 * @note   CTP:  Common Tlm Packet
 */
#ifndef COMMON_TLM_CMD_PACKET_H_
#define COMMON_TLM_CMD_PACKET_H_

#include <stddef.h> // for size_t
#include "../System/TimeManager/obc_time.h"
#include <src_user/CmdTlm/command_definitions.h>
#include <src_user/Library/stdint.h>
#include <src_user/Applications/app_registry.h>

#include <src_user/Settings/CmdTlm/common_tlm_cmd_packet_define.h>
#include <src_user/Settings/CmdTlm/common_tlm_cmd_packet_params.h>

/**
 * @enum   CCP_EXEC_STS
 * @brief  コマンド実行結果コード
 * @note   intを想定
 */
typedef enum
{
  CCP_EXEC_PACKET_FMT_ERR = -6,     //!< packet handler で用いる
  CCP_EXEC_ROUTING_FAILED = -5,     //!< command router で用いる
  CCP_EXEC_CMD_NOT_DEFINED = -4,    //!< cmdExec で用いる
  CCP_EXEC_ILLEGAL_CONTEXT = -3,    //!< コマンド実行時のその他のエラー
  CCP_EXEC_ILLEGAL_PARAMETER = -2,  //!< コマンド実行時のパラメタエラー
  CCP_EXEC_ILLEGAL_LENGTH = -1,     //!< コマンド実行時のコマンド引数長エラー
  CCP_EXEC_SUCCESS = 0,             /*!< コマンド実行成功
                                         なお，コマンドの実行の結果なので，その後の処理が失敗しても，コマンドが実行されていればこれを返す */
  CCP_EXEC_UNKNOWN                  //!< 内部処理用．使わない．
} CCP_EXEC_STS;


#include "block_command_table.h"      // CTCP, CCP_EXEC_STS の定義よりあとに include


/**
 * @enum   CTCP_TC_DSC (Tlm Cmd Discrimination)
 * @brief  tlm or cmd を規定
 * @note   uint8_tを想定
 */
typedef enum
{
  CTCP_TC_DSC_CMD,
  CTCP_TC_DSC_TLM,
  CTCP_TC_DSC_UNKNOWN
} CTCP_TC_DSC;

/**
 * @enum   CCP_EXEC_TYPE
 * @brief  コマンド実行種別
 * @note   0x0*を想定（上位4bitは他のC2Aを搭載したボード用に転送するために使うため）
 *         その定義はTCP_DEST_TYPEにある
 * @note   TCP_CMD_EXEC_TYPE と CCP_EXEC_TYPE の定義は一致 (正確には、前者は後者に含まれている)
 *         CCP_EXEC_TYPE CCP_get_exec_type(const CTCP* packet) を参照
 * @note   GSからのOBC.TLや，C2A内のCCP_form_tlcはすべてTL0扱いになる
 */
typedef enum
{
// GS : Ground Station Command
// TL : Timeline Command
// MC : Macro Command, Block Line Commandともいう
// RT : Realtime Command
  CCP_EXEC_TYPE_GS,
  CCP_EXEC_TYPE_TL0,
  CCP_EXEC_TYPE_MC,
  CCP_EXEC_TYPE_RT,
  CCP_EXEC_TYPE_TL1,
  CCP_EXEC_TYPE_TL2,
  CCP_EXEC_TYPE_UNKNOWN
} CCP_EXEC_TYPE;

/**
 * @enum   CTP_DEST_FLAG
 * @brief  配送先指定．テレメのキューを指定するフラグ．
 * @note   4bitのフラグ．重送できるように，bitごとのフラグとする
 */
typedef enum
{
  CTP_DEST_FLAG_HK = 0x01, // 00000001b: Housekeeping Telemetry
  CTP_DEST_FLAG_MS = 0x02, // 00000010b: Mission Telemetry
  CTP_DEST_FLAG_ST = 0x04, // 00000100b: Stored Telemetry
  CTP_DEST_FLAG_RP = 0x08, // 00001000b: Replay Telemetry
  CTP_DEST_FLAG_UNKNOWN
} CTP_DEST_FLAG;


// ********************
//  CTCP getter/setter
// ********************

/**
 * @brief  tlm か cmd かを判断
 * @param  packet: CTCP packet
 * @return CTCP_TC_DSC
 */
CTCP_TC_DSC CTCP_get_tc_dsc(const CTCP* packet);

/**
 * @brief  CTCP をコピー
 * @note   user実装次第だが，高速化のためすすべてではなく必要最低限しかコピーしない場合もある
 * @param[in,out] dest: コピー先 CTCP
 * @param[in]     src:  コピー元 CTCP
 * @return void
 */
void CTCP_copy_packet(CTCP* dest, const CTCP* src);


// *******************
//  CCP getter/setter
// *******************

/**
 * @brief  TI を取得
 * @param  packet: CTCP packet
 * @return ti
 */
cycle_t CCP_get_ti(const CTCP* packet);

/**
 * @brief  TI を設定
 * @param[in,out] packet: CTCP
 * @param[in]     ti: TI
 * @return void
 */
void CCP_set_ti(CTCP* packet, cycle_t ti);

/**
 * @brief  宛先 (TCPではAPID) を取得
 * @param  packet: CTCP packet
 * @return CTCP_DEST_ID
 */
CTCP_DEST_ID CCP_get_dest_id(const CTCP* packet);

/**
 * @brief  宛先 (TCPではAPID) を設定
 * @param[in,out] packet: CTCP
 * @param[in]     id: 宛先
 * @return void
 */
void CCP_set_dest_id(CTCP* packet, CTCP_DEST_ID id);

/**
 * @brief  EXEC_TYPE を取得
 * @param  packet: CTCP packet
 * @return CCP_EXEC_TYPE
 */
CCP_EXEC_TYPE CCP_get_exec_type(const CTCP* packet);

/**
 * @brief  EXEC_TYPE を設定
 * @param[in,out] packet: CTCP
 * @param[in]     type: EXEC_TYPE
 * @return void
 */
void CCP_set_exec_type(CTCP* packet, CCP_EXEC_TYPE type);

/**
 * @brief  DEST_TYPE を取得
 * @param  packet: CTCP packet
 * @return CCP_DEST_TYPE
 * @note   2021/01/22に CCP_EXEC_TYPE の上位4bitを拝借する形で追加実装
 */
CCP_DEST_TYPE CCP_get_dest_type(const CTCP* packet);

/**
 * @brief  DEST_TYPE を設定
 * @param[in,out] packet: CTCP
 * @param[in]     type: DEST_TYPE
 * @return void
 * @note   2021/01/22に CCP_EXEC_TYPE の上位4bitを拝借する形で追加実装
 */
void CCP_set_dest_type(CTCP* packet, CCP_DEST_TYPE type);

/**
 * @brief  CMD ID を取得
 * @param  packet: CTCP packet
 * @return CMD_CODE
 */
CMD_CODE CCP_get_id(const CTCP* packet);

/**
 * @brief  CMD ID を設定
 * @param[in,out] packet: CTCP
 * @param[in]     id: CMD_CODE
 * @return void
 */
void CCP_set_id(CTCP* packet, CMD_CODE id);

/**
 * @brief  CTCP (CCP) のパケット長を取得
 * @param  packet: CTCP packet
 * @return packet len
 */
uint16_t CCP_get_packet_len(const CTCP* packet);

/**
 * @brief  CMD パラメタ長を取得
 * @param  packet: CTCP packet
 * @return param len
 */
uint16_t CCP_get_param_len(const CTCP* packet);

/**
 * @brief  CMD パラメタ長の最大値を取得
 * @return max param len
 */
uint16_t CCP_get_max_param_len(void);

/**
 * @brief  先頭のパラメタのポインタを取得
 * @param  packet: CTCP packet
 * @return *param_head
 */
const uint8_t* CCP_get_param_head(const CTCP* packet);

/**
 * @brief  パラメタを設定
 * @param[in,out] packet: CTCP
 * @param[in]     param: パラメタ配列のポインタ
 * @param[in]     len:   パラメタ長
 * @return void
 */
void CCP_set_param(CTCP* packet, const uint8_t* param, uint16_t len);

/**
 * @brief  共通部分の Header を構築
 * @param[in,out] packet: CTCP
 * @return void
 */
void CCP_set_common_hdr(CTCP* packet);


// *******************
//  CTP getter/setter
// *******************

/**
 * @brief  CTP_DEST_FLAG を取得
 * @param  packet: CTCP packet
 * @return CTP_DEST_FLAG
 */
CTP_DEST_FLAG CTP_get_dest_flag(const CTCP* packet);

/**
 * @brief  CTP_DEST_FLAG を設定
 * @param[in,out] packet: CTCP
 * @param[in]     flag: CTP_DEST_FLAG
 * @return void
 */
void CTP_set_dest_flag(CTCP* packet, CTP_DEST_FLAG flag);

/**
 * @brief  ADU分割なしの場合のUser Data先頭のポインタを取得
 * @param  packet: CTCP packet
 * @note   TCPの場合，ADU分割なし時のポインタを取得
 * @note   この関数で取得したポインタ経由でtlmのデータをセットする事があるため，constをつけていない
 */
uint8_t* CTP_get_user_data_head(CTCP* packet);

#endif
