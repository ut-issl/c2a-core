/**
 * @file
 * @brief コンポ間通信などで標準的に使う ネットワーク層 の Space Packet
 * @note  ここでは，データリンク層は CCSDS ではなく EB90 Frame を使うことを想定
 * @note  データリンク層は DS_StreamConfig.data_link_layer_ で規定する
 * @note  packet 構造 などは TlmCmd/Ccsds/space_packet.h を参照のこと
 */
#ifndef SPACE_PACKET_FOR_DRIVER_SUPER_H_
#define SPACE_PACKET_FOR_DRIVER_SUPER_H_

#include <src_user/Library/stdint.h>
#include "../Super/driver_super.h"



/**
 * @brief  CommonTlmPacket を受信データからコピーして取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは C2A 形式で使われる TLM HEADER 仕様である
 * @param[in]  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @param[out] ctp: 抽出したパケット
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: パケット長が異常など，エラーでコピーできない
 */
DS_ERR_CODE SP_get_ctp(const DS_StreamConfig* p_stream_config, CommonTlmPacket* ctp);

/**
 * @brief  CommonCmdPacket を受信データからコピーして取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは C2A 形式で使われる TLM HEADER 仕様である
 * @param[in]  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @param[out] ccp: 抽出したパケット
 * @retval DS_ERR_CODE_OK:  正常終了
 * @retval DS_ERR_CODE_ERR: パケット長が異常など，エラーでコピーできない
 */
DS_ERR_CODE SP_get_ccp(const DS_StreamConfig* p_stream_config, CommonCmdPacket* ccp);



#endif
