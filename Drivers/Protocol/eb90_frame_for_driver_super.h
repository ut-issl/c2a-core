/**
 * @file
 * @brief コンポ間通信などで標準的に使う データリンク層 の EB90 Frame
 * @note  frame 構造
 *        |---------+-------+-------+------------------|
 *        | Pos     | Pos   | size  | name             |
 *        | [octet] | [bit] | [bit] |                  |
 *        |---------+-------+-------+------------------|
 *        | === Header =============================== |
 *        |---------+-------+-------+------------------|
 *        |       0 |     0 |    16 | STX              |
 *        |       2 |     0 |    16 | Packet Length    |
 *        |---------+-------+-------+------------------|
 *        | === Packet Field ========================= |
 *        |---------+-------+-------+------------------|
 *        |       4 |     0 |     * | EB90 Packet や   |
 *        |         |       |       |   Space Packet   |
 *        |         |       |       |   など           |
 *        |       * |     0 |     * | ...              |
 *        |---------+-------+-------+------------------|
 *        | === Footer =============================== |
 *        |---------+-------+-------+------------------|
 *        |   N - 4 |     0 |    16 | CRC              |
 *        |   N - 2 |     0 |    16 | ETX              |
 *        |---------+-------+-------+------------------|
 *
 *        Packet Length:
 *          Packet Field の長さ
 *        CRC
 *          CRC-16/CCITT-FALSE (CRC-16/AUTOSAR, CRC-16/IBM-3740 とも)
 *          Packet Field の CRC
 *          Header は含めない
 */
#ifndef EB90_FRAME_FOR_DRIVER_SUPER_H_
#define EB90_FRAME_FOR_DRIVER_SUPER_H_

#include <stdint.h>
#include "../Super/driver_super.h"

#define EB90_FRAME_STX_SIZE     (2)           //!< EB90 Frame の STX のサイズ
#define EB90_FRAME_ETX_SIZE     (2)           //!< EB90 Frame の ETX のサイズ
#define EB90_FRAME_LEN_SIZE     (2)           //!< EB90 Frame の LEN のサイズ
#define EB90_FRAME_CRC_SIZE     (2)           //!< EB90 Frame の CRC のサイズ
#define EB90_FRAME_HEADER_SIZE  (EB90_FRAME_STX_SIZE + EB90_FRAME_LEN_SIZE)  //!< EB90 Frame のヘッダサイズ
#define EB90_FRAME_FOOTER_SIZE  (EB90_FRAME_ETX_SIZE + EB90_FRAME_CRC_SIZE)  //!< EB90 Frame のフッタサイズ

#define EB90_FRAME_STX_1ST_BYTE     (0xEB)    //!< EB90 Frame の STX 1st byte
#define EB90_FRAME_STX_2ND_BYTE     (0x90)    //!< EB90 Frame の STX 2nd byte
#define EB90_FRAME_ETX_1ST_BYTE     (0xC5)    //!< EB90 Frame の ETX 1st byte
#define EB90_FRAME_ETX_2ND_BYTE     (0x79)    //!< EB90 Frame の ETX 2nd byte

extern const uint8_t EB90_FRAME_kStx[EB90_FRAME_STX_SIZE];
extern const uint8_t EB90_FRAME_kEtx[EB90_FRAME_ETX_SIZE];

/**
 * @brief  Packet Length の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return Packet Length
 */
uint16_t EB90_FRAME_get_packet_length_from_dssc(const DS_StreamConfig* p_stream_config);

/**
 * @brief  受信フレームから Packet Field の先頭ポインタを取得する
 * @note   フレームが確定しているときに呼び出すこと
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return Packet Field の先頭ポインタ
 */
const uint8_t* EB90_FRAME_get_packet_head_from_dssc(const DS_StreamConfig* p_stream_config);

/**
 * @brief  CRC の取得
 * @note   フレームが確定しているときに呼び出すこと
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @return CRC
 */
uint16_t EB90_FRAME_get_crc_from_dssc(const DS_StreamConfig* p_stream_config);

/**
 * @brief  EB90 Frame に付与された CRC が正しいかチェック
 * @param  p_stream_config: DriverSuper 構造体の DS_StreamConfig
 * @retval 1: 正しい
 * @retval 0: 正しくない
 */
uint8_t EB90_FRAME_is_valid_crc_of_dssc(const DS_StreamConfig* p_stream_config);

/**
 * @brief  EB90 Frame の CRC の計算
 * @note   CRC-16-CCITT を使う
 * @param  data: CRC を計算するデータのポインタ
 * @param  len:  データ長
 * @return CRC
 */
uint16_t EB90_FRAME_calc_crc(const uint8_t* data, size_t len);

#endif
