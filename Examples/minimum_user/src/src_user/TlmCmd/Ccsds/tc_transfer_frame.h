/**
 * @file
 * @brief CCSDS で規定される TC Transfer Frame の実装
 * @note  packet 構造
 *        |---------+-------+-------+--------------------------|
 *        | Pos     | Pos   | Size  | Name                     |
 *        | [octet] | [bit] | [bit] |                          |
 *        |---------+-------+-------+--------------------------|
 *        | === Primary Header ================================|
 *        |---------+-------+-------+--------------------------|
 *        |       0 |     0 |     2 | Version                  |
 *        |       0 |     2 |     2 | Type                     |
 *        |       0 |     4 |     2 | N/A                      |
 *        |       0 |     6 |    10 | Spacecraft ID            |
 *        |       2 |     0 |     6 | Virtual Channel ID       |
 *        |       2 |     6 |    10 | Frame Length             |
 *        |       4 |     0 |     8 | Frame Sequence Number    |
 *        |---------+-------+-------+--------------------------|
 *        | === User Data Field ===============================|
 *        |---------+-------+-------+--------------------------|
 *        |       5 |     0 |     * | TC Segment               |
 *        |---------+-------+-------+--------------------------|
 *        | === Frame Error Control Field =====================|
 *        |---------+-------+-------+--------------------------|
 *        |       * |     0 |    16 | Frame Error Control Word |
 *        |---------+-------+-------+--------------------------|
 */
#ifndef TC_TRANSFER_FRAME_H_
#define TC_TRANSFER_FRAME_H_

#include <stddef.h>

#include "../../Library/stdint.h"
#include "tc_segment.h"

#define TCTF_MAX_LEN     (1024)
#define TCTF_HEADER_SIZE (5)
#define TCTF_FECF_SIZE   (2)

#define TCTF_BC_CMD_CODE_UNLOCK   (0x00)
#define TCTF_BC_CMD_CODE_SET_VR_1ST_BYTE (0x82)
#define TCTF_BC_CMD_CODE_SET_VR_2ND_BYTE (0x00)

#if TCTF_HEADER_SIZE + TCS_PACKET_MAX_LENGTH + TCTF_FECF_SIZE > TCTF_MAX_LEN
#error TCTCF LEN IS LONGER THAN TCTF MAX LEN
#endif

/**
 * @struct TcTransferFrame
 * @brief  TC Transfer Frame のパケット構造体
 */
typedef struct
{
  uint8_t packet[TCTF_MAX_LEN];
} TcTransferFrame;

/**
 * @enum  TCTF_VER
 * @brief Version
 * @note  2 bit
 */
typedef enum
{
  TCTF_VER_1 = 0, // 00b: Version 1
  TCTF_VER_UNKNOWN
} TCTF_VER;

/**
 * @enum  TCTF_TYPE
 * @brief Type
 * @note  2 bit
 */
typedef enum
{
  TCTF_TYPE_AD = 0, // 00b: TC data with FARM
  TCTF_TYPE_BD = 2, // 10b: TC data without FARM
  TCTF_TYPE_BC = 3, // 11b: FARM Control Command
  TCTF_TYPE_UNKNOWN
} TCTF_TYPE;

/**
 * @enum  TCTF_SCID
 * @brief Spacecraft ID
 * @note  10 bit
 */
typedef enum
{
  TCTF_SCID_SAMPLE_SATELLITE = 0x157, // SCID for command of sample satellite
  TCTF_SCID_UNKNOWN
} TCTF_SCID;

/**
 * @enum  TCTF_VCID
 * @brief Virtual Channel ID
 * @note  6 bit
 */
typedef enum
{
  TCTF_VCID_REALTIME = 0,
  TCTF_VCID_UNKNOWN
} TCTF_VCID;

/**
 * @brief version を取得
 * @param[in] tctf: TcTransferFrame
 * @return TCTF_VER
 */
TCTF_VER TCTF_get_ver(const TcTransferFrame* tctf);

/**
 * @brief command type を取得
 * @param[in] tctf: TcTransferFrame
 * @return TCTF_TYPE
 */
TCTF_TYPE TCTF_get_type(const TcTransferFrame* tctf);

/**
 * @brief SCID を取得
 * @param[in] tctf: TcTransferFrame
 * @return TCTF_SCID
 */
TCTF_SCID TCTF_get_scid(const TcTransferFrame* tctf);

/**
 * @brief VCID を取得
 * @param[in] tctf: TcTransferFrame
 * @return TCTF_VCID
 */
TCTF_VCID TCTF_get_vcid(const TcTransferFrame* tctf);

/**
 * @brief Frame Length を取得
 * @param[in] tctf: TcTransferFrame
 * @return Frame Length
 */
uint16_t TCTF_get_frame_len(const TcTransferFrame* tctf);

/**
 * @brief  Frame Sequence Number を取得
 * @param[in] tctf: TcTransferFrame
 * @return  Frame Sequence Number
 */
uint8_t TCTF_get_frame_seq_num(const TcTransferFrame* tctf);

/**
 * @brief FECW を取得
 * @param[in] tctf: TcTransferFrame
 * @return FECW
 * @note エンディアンは既に考慮されている (ccsds は big endian)
 */
uint16_t TCTF_get_fecw(const TcTransferFrame* tctf);

/**
 * @brief TC Segment を取得
 * @param[in] tctf: TcTransferFrame
 * @return TC Segment
 */
const TcSegment* TCTF_get_tc_segment(const TcTransferFrame* tctf);

/**
 * @brief TCTF FECW のチェック
 * @param[in] tctf: TcTransferFrame
 * @return bool: 1: ok, 0: error
 * @note ここでは標準として CRC-16-CCITT を採用している
 */
uint8_t TCTF_check_fecw(const TcTransferFrame* tctf);

/**
 * @brief 受信 byte 列を TcTransferFrame* に変換
 * @param[in] byte: 受信 byte 列
 * @return TcTransferFrame*
 */
const TcTransferFrame* TCTF_convert_from_bytes_to_tctf(const uint8_t* byte);

#endif
