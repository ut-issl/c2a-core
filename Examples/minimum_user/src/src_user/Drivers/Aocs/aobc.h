/**
 * @file
 * @brief AOBC の Driver
 */
#ifndef AOBC_H_
#define AOBC_H_

#include <src_core/IfWrapper/uart.h>
#include <src_core/Drivers/Super/driver_super.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "./aobc_telemetry_data_definitions.h"
#include "./aobc_telemetry_buffer.h"

/**
 * @enum   AOBC_TX_ERR_CODE
 * @brief  AOBC のコマンド送信エラーコード
 * @note   uint8_t を想定
 */
typedef enum
{
  AOBC_TX_ERR_CODE_OK = 0,
  AOBC_TX_ERR_CODE_CMD_NOT_FOUND
} AOBC_TX_ERR_CODE;

/**
 * @enum   AOBC_RX_ERR_CODE
 * @brief  AOBC のテレメ受信エラーコード
 * @note   uint8_t を想定
 */
typedef enum
{
  AOBC_RX_ERR_CODE_OK = 0,
  AOBC_RX_ERR_CODE_TLM_NOT_FOUND,
  AOBC_RX_ERR_CODE_CRC_ERR
} AOBC_RX_ERR_CODE;

/**
 * @struct AOBC_Info
 * @brief
 */
typedef struct
{
#if 0
  // C2A情報
  struct
  {
    cycle_t ti;
  } c2a;                           //!< C2A 情報
#endif
  // 通信情報
  struct
  {
    AOBC_TX_ERR_CODE tx_err_code;
    AOBC_RX_ERR_CODE rx_err_code;
  } comm;                         //!< 通信情報
} AOBC_Info;

/**
 * @struct  AOBC_Driver
 * @brief
 */
struct AOBC_Driver
{
  struct
  {
    DriverSuper super;        //!< DriverSuper class
    UART_Config uart_config;  //!< RS422 class
  } driver;
  AOBC_Info info;
  AOBC_TlmData tlm_data;
  AOBC_TlmBuffer tlm_buffer;
};


/**
 * @brief  AOBC 初期化
 *
 *         AOBC_Driver 構造体のポインタを渡すことでポートを初期化し， AOBC_Driver の各メンバも初期化する
 * @param  aobc_driver: 初期化する AOBC_Driver 構造体へのポインタ
 * @param  ch:          AOBC が接続されている UART ポート番号
 * @param  rx_buffer:   受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE AOBC_init(AOBC_Driver* aobc_driver, uint8_t ch, DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  AOBC のデータ（テレメ）受信
 * @param  aobc_driver: AOBC_Driver 構造体へのポインタ
 * @return DS_REC_ERR_CODE
 */
DS_REC_ERR_CODE AOBC_rec(AOBC_Driver* aobc_driver);

/**
 * @brief  AOBC へのコマンド送信
 * @param  aobc_driver: AOBC_Driver 構造体へのポインタ
 * @param  packet:      送信する packet
 * @return DS_CMD_ERR_CODE
 * @note   これを受信した AOBC C2A は， packet をそのまま PH_analyze_cmd_packet に流せばよい．
 */
DS_CMD_ERR_CODE AOBC_send_cmd(AOBC_Driver* aobc_driver, const CommonCmdPacket* packet);

#endif
