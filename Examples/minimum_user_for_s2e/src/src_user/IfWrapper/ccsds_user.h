/**
 * @file
 * @brief CCSDS API 依存の関数を宣言
 */
#ifndef CCSDS_USER_H_
#define CCSDS_USER_H_

#include <src_core/IfWrapper/ccsds.h>
#include "../Library/stdint.h"

#define CCSDS_FIFO_SIZE (8) // 現在使っている CCSDS API の設計上決まっている
#define CCSDS_SELECT_NUM (6)

/**
 * @enum  CCSDS_ERR_CODE
 * @brief CCSDS IF 関数の返り値
 */
typedef enum
{
  CCSDS_ERR_TX_NO_BUFFER = -6,
  CCSDS_ERR_TX_INVALID   = -5,
  CCSDS_ERR_TX_SIZE_ERR  = -4,
  CCSDS_ERR_RX_4KBPS     = -3, //!< 4kbps に訂正出来ない BCH エラー
  CCSDS_ERR_RX_1KBPS     = -2, //!< 1kbps に訂正出来ない BCH エラー
  CCSDS_ERR_PARAM_ERR    = -1,
  CCSDS_ERR_OK           = 0   //!< OK は 0 を踏襲
} CCSDS_ERR_CODE;

/**
 * @struct CCSDS_Info
 * @brief CCSDS 現在使っている CCSDS API の実行結果
 */
typedef struct
{
  uint32_t uip_stat[CCSDS_SELECT_NUM]; //!< FPGA が保持する CCSDS sequence counter
  uint8_t buffer_num;                  //!< 送信バッファの残り数. 最大 CCSDS_FIFO_SIZE
} CCSDS_Info;

/**
 * @brief FPGA が保持する CCSDS sequence counter を読む API を呼び出す
 * @param[in] select レジスタ読み出しを選択
 * @param[out] uip_stat: 状態
 * @return CCSDS_ERR_CODE
 */
CCSDS_ERR_CODE CCSDS_read_sequence(uint32_t select, uint32_t* uip_stat);

/**
 * @brief CCSDS TX の残り buffer をカウントするAPI を呼びだす
 * @return 残り buffer 数
 */
uint8_t CCSDS_get_buffer_num(void);

/**
 * @brief CCSDS のレートを設定する API を呼び出す
 * @param[in] ui_rate: 40000000u をこれで割ったレートが設定される. 0xFF 以下である必要がある (超えていたら 0xFF 扱いになる)
 * @param[in] config: CCSDS_Config
 */
void CCSDS_set_rate(uint32_t ui_rate, CCSDS_Config* config);

#endif
