/**
 * @file
 * @brief UART通信ラッパーのクラスです。
 * @note UART，具体的にはUARTやLVTTLを想定している
 *       UART通信ラッパーは、
 *       UARTのインターフェースを実現し、
 *       UARTポートの初期化、データ送信、データ受信を行う、
 *       UART通信ラッパーのクラスです。
 *       個々の機器のインターフェースモジュールに継承させて使用します。
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/**
 * @enum  PARITY_SETTINGS
 * @brief UART 用のパリティ設定
 */
typedef enum
{
  PARITY_SETTINGS_NONE, //!< パリティチェックをしない
  PARITY_SETTINGS_ODD,  //!< 奇数パリティ
  PARITY_SETTINGS_EVEN, //!< 偶数パリティ
  PARITY_SETTINGS_MARK, //!< パリティが常に 1
  PARITY_SETTINGS_SPACE //!< パリティが常に 0
} PARITY_SETTINGS;

/**
 * @enum  UART_DATA_LENGTH
 * @brief UART データ長 (7 or 8)
 */
typedef enum
{
  UART_DATA_LENGTH_7BIT,
  UART_DATA_LENGTH_8BIT
} UART_DATA_LENGTH;

/**
 * @enum  UART_STOP_BIT
 * @brief UART ストップビット
 */
typedef enum
{
  UART_STOP_BIT_1BIT,
  UART_STOP_BIT_15BIT,
  UART_STOP_BIT_2BIT
} UART_STOP_BIT;

/**
 * @enum  UART_ERR_CODE
 * @brief UART用の汎用エラーコード
 * @note  型はint8_tを想定しているが, if_listでラップされているためUARTの公開関数の返り値はintとなっている
 *        基本負なのはRXの返り値は正が送ったByte数, 負がエラーコードとなっているため
 */
typedef enum
{
  UART_UNKNOWN_ERR     = -14, //!< 原因不明
  UART_DATA_NEGA_ERR   = -13, //!< 送受信データサイズが負のエラー
  UART_FIFO_FULL_ERR   = -12, //!< TX時に送信FIFOフル
  UART_RX_ALL_ERR      = -11, //!< パリティ and FIFO and ストップビット
  UART_FIFO_STOP_ERR   = -10, //!< FIFO and ストップビット
  UART_PARITY_STOP_ERR = -9,  //!< パリティ and ストップビット
  UART_STOP_BIT_ERR    = -8,  //!< ストップビットエラー
  UART_PARITY_FIFO_ERR = -7,  //!< パリティ and FIFO
  UART_FIFO_OVER_ERR   = -6,  //!< FIFOオーバーラン
  UART_PARITY_ERR      = -5,  //!< パリティエラー
  UART_YET_ERR         = -4,  //!< チャンネル未オープン
  UART_ALREADY_ERR     = -3,  //!< チャンネルオープン済み
  UART_BAUDRATE_ERR    = -2,  //!< ボーレート異常
  UART_CH_ERR          = -1,  //!< チャンネル異常 (Port_configに無い)
  UART_OK              = 0    //!< OKは0を踏襲
} UART_ERR_CODE;

/**
 * @struct UART_Config
 * @brief  UARTポートの初期化、データ送信、データ受信の際に必要となる設定情報を格納する構造体
 */
typedef struct
{
  uint8_t          ch;                //!< 継承先の機器がつながっているポート番号
  char*            device_file_name;  //!< 継承先の機器がつながっているファイル名 (Linuxなどのデバイスファイル環境向け)
  uint32_t         baudrate;          //!< 継承先の機器のボーレート値
  PARITY_SETTINGS  parity_settings;   //!< パリティ設定
  UART_DATA_LENGTH data_length;       //!< データ長
  UART_STOP_BIT    stop_bit;          //!< ストップビット
} UART_Config;

/**
 * @brief UART_CONFIG構造体をグローバルインスタンスとして定義し, ポインタを渡すことでポートを初期化する.
 * @param[in] my_uart_v 初期化するUART_CONFIG構造体へのポインタ
 * @return int if_listでintと統一されている
 * @note UART通信ラッパーのクラスを使用する時は起動時に必ず実施してください.
 *       この関数を実行する前にUART_CONFIG構造体内の設定値を設定しておく必要があります.
 */
int UART_init(void* my_uart_v);


/**
 * @brief UART_CONFIG構造体にて指定されたchからデータを受信します
 * @param[in] my_uart_v   : 対象とするUART_CONFIG構造体へのポインタ
 * @param[in] data_v      : 受信データ格納先へのポインタ
 * @param[in] buffer_size : 受信データ格納先のデータサイズ
 * @retval 0: 受信データなし
 * @retval 正: 受信データバイト数
 * @retval 負: エラー. UART_ERR_CODE に従う
 */
int UART_rx(void* my_uart_v, void* data_v, int buffer_size);


/**
 * @brief UART_CONFIG構造体にて指定されたchへデータを送信します
 * @param[in] my_uart_v : 対象とするUART_CONFIG構造体へのポインタ
 * @param[in] data_v    : 送信データ格納先へのポインタ
 * @param[in] data_size : 送信データサイズ
 * @return int:  UART_ERR_CODE に従う
 */
int UART_tx(void* my_uart_v, void* data_v, int data_size);


/**
 * @brief 指定されたchを開きなおす
 * @param[in] my_uart_v : 開きなおす際のポート設定
 * @param[in] reason    : repoenの理由を示すコードなど
 */
int UART_reopen(void* my_uart_v, int reason);

#endif
