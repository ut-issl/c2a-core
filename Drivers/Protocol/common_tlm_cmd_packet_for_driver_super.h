/**
 * @file
 * @brief コンポ間通信などで標準的に使う ネットワーク層 の Common Tlm Cmd Packet (概ね Space Packet)
 * @note  ここでは，データリンク層は CCSDS ではなく EB90 Frame を使うことを想定 (TODO: 今後拡張予定)
 * @note  データリンク層は DS_StreamConfig.data_link_layer_ で規定する
 * @note  packet 構造 などは TlmCmd/common_tlm_cmd_packet.h を参照のこと
 */
#ifndef COMMON_TLM_CMD_PACKET_FOR_DRIVER_SUPER_H_
#define COMMON_TLM_CMD_PACKET_FOR_DRIVER_SUPER_H_

#include <src_user/Library/stdint.h>
#include "../Super/driver_super.h"
#include "../../TlmCmd/common_tlm_cmd_packet.h"

/**
 * @brief  CommonTlmCmdPacket を受信データからコピーして取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @param[in]  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @param[out] ctcp: 抽出したパケット
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: パケット長が異常など，エラーでコピーできない
 */
DS_ERR_CODE CTCP_get_ctcp_from_dssc(const DS_StreamConfig* p_stream_config, CommonTlmCmdPacket* ctcp);

/**
 * @brief  CommonTlmPacket を受信データからコピーして取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @param[in]  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @param[out] ctp: 抽出したパケット
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: パケット長が異常など，エラーでコピーできない
 */
DS_ERR_CODE CTP_get_ctp_from_dssc(const DS_StreamConfig* p_stream_config, CommonTlmPacket* ctp);

/**
 * @brief  CommonCmdPacket を受信データからコピーして取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @param[in]  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @param[out] ccp: 抽出したパケット
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: パケット長が異常など，エラーでコピーできない
 */
DS_ERR_CODE CCP_get_ccp_from_dssc(const DS_StreamConfig* p_stream_config, CommonCmdPacket* ccp);

#endif
