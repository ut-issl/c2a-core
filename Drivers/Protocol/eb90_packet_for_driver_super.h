/**
 * @file
 * @brief コンポ間通信などで標準的に使う ネットワーク層 の EB90 Packet
 * @note  一般的には，データリンク層は EB90 Frame を使うことを想定
 * @note  Tlm か Cmd かはコンテキストで読み替える
 * @note  データリンク層は DS_StreamConfig.data_link_layer_ で規定する
 * @note  packet 構造
 *        |---------+-------+-------+------------------|
 *        | Pos     | Pos   | size  | name             |
 *        | [octet] | [bit] | [bit] |                  |
 *        |---------+-------+-------+------------------|
 *        | === Header =============================== |
 *        |---------+-------+-------+------------------|
 *        |       0 |     0 |     8 | Version ID       |
 *        |       1 |     0 |     8 | Tlm / Cmd Count  |
 *        |       2 |     0 |    16 | Tlm / Cmd ID     |
 *        |---------+-------+-------+------------------|
 *        | === User Data Field ====================== |
 *        |---------+-------+-------+------------------|
 *        |       4 |     0 |     * | User Data #0     |
 *        |       * |     0 |     * | User Data #1     |
 *        |       * |     0 |     * | User Data #2     |
 *        |       * |     0 |     * | ...              |
 *        |---------+-------+-------+------------------|
 */
#ifndef EB90_PACKET_FOR_DRIVER_SUPER_H_
#define EB90_PACKET_FOR_DRIVER_SUPER_H_

#include <stdint.h>
#include "../Super/driver_super.h"

#define EB90_PACKET_HEADER_SIZE  (4)  //!< EB90 Packet のヘッダサイズ

/**
 * @brief  Version ID の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return Version ID （バージョン 0x01 では uint8_t だが将来性のために uint32_t で返す）
 */
uint32_t EB90_PACKET_get_version_from_dssc(const DS_StreamConfig* p_stream_config);

/**
 * @brief  Tlm / Cmd Count の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @retval Tlm / Cmd Count （バージョン 0x01 では uint8_t だが将来性のために uint32_t で返す）
 * @retval 0 (入力が不正な場合)
 */
uint32_t EB90_PACKET_get_count_from_dssc(const DS_StreamConfig* p_stream_config);

/**
 * @brief  Tlm / Cmd ID の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @note   これは非 C2A 形式で使われる TLM HEADER 仕様である
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @retval Tlm / Cmd ID （バージョン 0x01 では uint16_t だが将来性のために uint32_t で返す）
 * @retval 0 (入力が不正な場合)
 */
uint32_t EB90_PACKET_get_id_from_dssc(const DS_StreamConfig* p_stream_config);

/**
 * @brief  受信フレームからPacket Field の先頭ポインタを取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @retval Packet Field の先頭ポインタ
 * @retval NULL (入力が不正な場合)
 */
const uint8_t* EB90_PACKET_get_user_data_head_from_dssc(const DS_StreamConfig* p_stream_config);

// TODO: dssc の tx_frame に packet を EB90 frame につめてから挿入する関数を作る
//       CTCP_set_tx_frame_to_dssc の EB90 packet 用

#endif
