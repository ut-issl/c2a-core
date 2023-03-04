/**
 * @file
 * @brief MOBC の Driver
 */
#ifndef MOBC_H_
#define MOBC_H_

#include <src_core/IfWrapper/uart.h>
#include <src_core/Drivers/Super/driver_super.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_core/TlmCmd/common_tlm_packet.h>
#include <src_core/TlmCmd/packet_handler.h>

/**
 * @enum   MOBC_TX_ERR_CODE
 * @brief  MOBC の コマンド送信（ MOBC からみたらテレメ）エラーコード
 * @note   uint8_t を想定
 */
typedef enum
{
  MOBC_TX_ERR_CODE_OK = 0
  // MOBC_TX_ERR_CODE_CMD_NOT_FOUND,
} MOBC_TX_ERR_CODE;


/**
 * @enum   MOBC_RX_ERR_CODE
 * @brief  MOBC のテレメ受信（ MOBC からみたらコマンド）エラーコード
 * @note   uint8_t を想定
 */
typedef enum
{
  MOBC_RX_ERR_CODE_OK = 0,
  // MOBC_RX_ERR_CODE_TLM_NOT_FOUND,
  MOBC_RX_ERR_CODE_CRC_ERR,
  MOBC_RX_ERR_CODE_INVALID_PACKET
} MOBC_RX_ERR_CODE;


/**
 * @struct MOBC_Info
 * @brief  MOBCとの通信の諸情報をもつ
 */
typedef struct
{
  // C2A 情報
  struct
  {
    DS_CMD_ERR_CODE send_tlm_err_code;    //!< テレメトリ送信時のエラーコード
    PH_ACK          ph_ack;               //!< 受信したコマンドを PH に渡したときの返り値
  } c2a;                                  //!< C2A 情報
  // 通信情報
  struct
  {
    MOBC_TX_ERR_CODE tx_err_code;         //!< DriverSuper ではなく Driver が持つべき通信エラー情報
    MOBC_RX_ERR_CODE rx_err_code;         //!< DriverSuper ではなく Driver が持つべき通信エラー情報
  } comm;                                 //!< 通信情報
} MOBC_Info;


/**
 * @struct  MOBC_Driver
 * @brief   MOBC Driver
 */
typedef struct
{
  struct
  {
    DriverSuper super;                    //!< DriverSuper class
    UART_Config uart_config;              //!< UART class
  } driver;
  MOBC_Info info;
} MOBC_Driver;


/**
 * @brief  MOBC初期化
 *
 *         MOBC_Driver 構造体のポインタを渡すことでポートを初期化し， MOBC_Driver の各メンバも初期化する
 * @param  mobc_driver: 初期化する MOBC_Driver 構造体へのポインタ
 * @param  ch:          MOBC が接続されている UART ポート番号
 * @param  rx_buffer:   受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE MOBC_init(MOBC_Driver* mobc_driver, uint8_t ch, DS_StreamRecBuffer* rx_buffer);


/**
 * @brief  MOBC からのデータ（ MOBC → AOBC のコマンド）受信
 * @param  mobc_driver: MOBC_Driver 構造体へのポインタ
 * @return DS_REC_ERR_CODE
 */
DS_REC_ERR_CODE MOBC_rec(MOBC_Driver* mobc_driver);


/**
 * @brief  MOBC へのデータ（MOBC → AOBCのテレメ）送信
 * @param  mobc_driver: MOBC_Driver 構造体へのポインタ
 * @param  packet: 送信する CTP packet
 * @return DS_CMD_ERR_CODE
 */
DS_CMD_ERR_CODE MOBC_send(MOBC_Driver* mobc_driver, const CommonTlmPacket* packet);

#endif
