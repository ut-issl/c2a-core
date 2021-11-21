/**
 * @file   common_tlm_cmd_packet_util.h
 * @brief  CTCP の汎用Util
 */
#ifndef COMMON_TLM_CMD_PACKET_UTIL_H_
#define COMMON_TLM_CMD_PACKET_UTIL_H_

#include "common_tlm_cmd_packet.h"

/**
 * @enum   CTCP_UTIL_ACK
 * @brief  CTCP Utility の汎用返り値
 * @note   uint8_t を想定
 */
typedef enum
{
  CTCP_UTIL_ACK_OK = 0,       //!< 正常終了
  CTCP_UTIL_ACK_PARAM_ERR     //!< パラメタエラー
} CTCP_UTIL_ACK;

/**
 * @brief  App実行コマンドを生成
 * @param[in,out] packet: CTCP
 * @param[in]     ti: TI
 * @param[in]     id: AR_APP_ID
 * @return void
 */
void CCP_form_app_cmd(CTCP* packet, cycle_t ti, AR_APP_ID id);

/**
 * @brief  Realtime command を生成
 * @param[in,out] packet: CTCP
 * @param[in]     packet: CMD_CODE
 * @param[in]     *param: パラメタ
 * @param[in]     len:    パラメタ長
 * @return CTCP_UTIL_ACK
 */
CTCP_UTIL_ACK CCP_form_rtc(CTCP* packet, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  Timeline command を生成
 * @param[in,out] packet: CTCP
 * @param[in]     ti:     TI
 * @param[in]     packet: CMD_CODE
 * @param[in]     *param: パラメタ
 * @param[in]     len:    パラメタ長
 * @return CTCP_UTIL_ACK
 */
CTCP_UTIL_ACK CCP_form_tlc(CTCP* packet, cycle_t ti, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief  BC展開 command を生成
 * @param[in,out] packet: CTCP
 * @param[in]     tl_no: Timeline no
 * @param[in]     block_no: BC ID
 * @return CTCP_UTIL_ACK
 */
CTCP_UTIL_ACK CCP_form_block_deploy_cmd(CTCP* packet, uint8_t tl_no, bct_id_t block_no);

/**
 * @brief  Realtime Command から Timeline Command へ変換
 * @param[in,out] packet: 変換する packet
 * @param[in]     ti:     TI
 * @return void
 */
void CCP_convert_rtc_to_tlc(CTCP* packet, cycle_t ti);

#endif
