/**
 * @file
 * @brief CCSDS通信ラッパーのクラスです。(MOBC用 バックグラウンド取り込みVer)
 *
 *        CCSDS通信ラッパーは、
 *        CCSDSのインターフェースを実現し、
 *        CCSDSポートの初期化、データ送信、データ受信を行う、
 *        CCSDS通信ラッパーのクラスです。
 *        個々の機器のインターフェースモジュールに継承?せさて使用します。
 */

#ifndef CCSDS_H_
#define CCSDS_H_

#include <stdint.h>

/**
 * @struct CCSDS_Config
 * @brief  CCSDS ポートの初期化, データ送信, データ受信の際に必要となる設定情報を格納する構造体
 */
typedef struct
{
  uint8_t  ch;      //!< 将来的にchが追加されることを想定して
  uint32_t bitrate; //!< 継承先の機器のビットレート値 (bps)
} CCSDS_Config;

/**
 * @brief CCSDS の init
 * @note  実際には初期化 API は無いので bps の設定のみ行っている
 * @param[in] my_ccsds_v: CCSDS config
 * @return int: 初期化結果 (OK)
 */
int CCSDS_init(void* my_ccsds_v);

/**
 * @brief CCSDS 受信. API を呼び出し, buffer に受信結果をコピーする
 * @param[in] my_ccsds_v: CCSDS config
 * @param[out] data_v: buffer
 * @param[in] buffer_size: buffer size
 * @return int: 非負の場合は受信データサイズ, 負の場合は CCSDS_ERR_CODE に従う
 */
int CCSDS_rx(void* my_ccsds_v, void* data_v, int buffer_size);

/**
 * @brief CCSDS 送信. API を呼び出し, 送信を行う
 * @param[in] my_ccsds_v: CCSDS config
 * @param[out] data_v: 送信 データ
 * @param[in] data_size: 送信 size
 * @return CCSDS_ERR_CODE
 */
int CCSDS_tx(void* my_ccsds_v, void* data_v, int data_size);

/**
 * @brief CCSDS reopen
 * @note  実際には reopen API は無いので何もしない
 * @param[in] my_ccsds_v: CCSDS config
 * @param[in] reason: reopen 理由
 * @return 0
 */
int CCSDS_reopen(void* my_ccsds_v, int reason);

#endif
