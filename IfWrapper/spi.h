/**
 * @file
 * @brief SPI通信(Master側)のラッパークラスです。
 * @note SPI通信ラッパーは、
 *       SPIのインターフェースを実現し、
 *       SPIポートの初期化、データ送信、データ受信を行う、
 *       SPI通信ラッパーのクラスです。
 *       個々の機器のインターフェースモジュールに継承させて使用します。
 */
#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

 /**
  * @enum  SPI_MODE
  * @brief SPI動作モードの設定
  * @note  型はuint8_tを想定
  */
typedef enum
{
  SPI_MODE_0 = 0,  //!< CPOL=0, CPHA = 0
  SPI_MODE_1 = 1,  //!< CPOL=0, CPHA = 1
  SPI_MODE_2 = 2,  //!< CPOL=1, CPHA = 0
  SPI_MODE_3 = 3   //!< CPOL=1, CPHA = 1
} SPI_MODE;

/**
 * @enum  SPI_MODE
 * @brief SPI動作モードの設定
 * @note  型はuint8_tを想定
 */
typedef enum
{
  SPI_CS_STATE_AFTER_TX_HIGH = 0,   //!< 送信後にCSをHighにする
  SPI_CS_STATE_AFTER_TX_LOW  = 1    //!< 送信後にCSをLowのままにし、そのまま受信動作などを行う
} SPI_CS_STATE_AFTER_TX;

/**
 * @struct SPI_Config
 * @brief SPIポートの初期化、データ送信・受信の際に必要となる設定情報を格納する構造体です。
 */
typedef struct
{
  uint8_t  comm_ch;                        //!< 継承先の機器がつながっている通信ポート番号
  uint8_t  gpio_ch;                        //!< 継承先の機器がつながっているChip Select用GPIOポート番号
  uint32_t frequency_khz;                  //!< SPIクロックの周波数 単位:kHz
  SPI_MODE mode;                           //!< SPI動作モード
  uint8_t  tx_data_for_rx;                 //!< RX動作をするために定期的に送るTXデータ
  uint16_t rx_length;                      //!< 受信するデータ数、受信動作前に設定する
  SPI_CS_STATE_AFTER_TX cs_state_after_tx; //!< 送信動作後にChip SelectをHighにするかどうかのフラグ
} SPI_Config;

/**
 * @enum  SPI_ERR_CODE
 * @brief SPI用の汎用エラーコード
 * @note  型はint8_tを想定しているが, if_listでラップされているためSPIの公開関数の返り値はintとなっている
 *        基本負なのはRXの返り値は正が送ったByte数, 負がエラーコードとなっているため
 */
typedef enum
{
  SPI_ERR_UNKNOWN = -20,          //!< 原因不明
  SPI_ERR_RX_BUFFER_SMALL = -14,  //!< 受信バッファサイズが受信データ長より小さい
  SPI_ERR_DATA_NEGA = -13,        //!< 送受信データサイズが負のエラー
  SPI_ERR_DATA_LARGE = -12,       //!< 送受信データサイズが大きすぎる
  SPI_ERR_GPIO = -5,              //!< GPIO関連エラー
  SPI_ERR_YET = -4,               //!< チャンネル未オープン
  SPI_ERR_ALREADY = -3,           //!< チャンネルオープン済み
  SPI_ERR_FREQUENCY = -2,         //!< 周波数異常
  SPI_ERR_CH = -1,                //!< チャンネル異常 (Port_configに無い)
  SPI_ERR_OK = 0                  //!< OKは0を踏襲
} SPI_ERR_CODE;

/**
 * @brief SPI_Config構造体をグローバルインスタンスとして定義し、ポインタを渡すことでポートを初期化します。
 * @param[in] my_spi_v 初期化するSPI_Config構造体へのポインタ
 * @return int SPI_ERR_CODEとしたいが, if_listでintと統一されている。
 * @note SPI通信ラッパーのクラスを使用する時は起動時に必ず実施してください。
 *       この関数を実行する前にSPI_Config構造体内の設定値を設定しておく必要があります。
 */
int SPI_init(void* my_spi_v);

/**
 * @brief SPI_Config構造体にて指定されたchからデータを受信します
 * @param[in] my_spi_v    : 対象とするSPI_Config構造体へのポインタ
 * @param[in] data_v      : 受信データ格納先へのポインタ
 * @param[in] buffer_size : 受信データ格納先のバッファサイズ、SPI通信による受信データ長よりも長い必要がある
 * @retval 0: 受信データなし
 * @retval 正: 受信データバイト数
 * @retval 負: エラー. SPI_ERR_CODE に従う
 * @note この関数内でSPI読み出し要求を送信しています
 */
int SPI_rx(void* my_spi_v, void* data_v, int buffer_size);

/**
 * @brief SPI_Config構造体にて指定されたchへデータを送信します
 * @param[in] my_spi_v : 対象とするSPI_Config構造体へのポインタ
 * @param[in] data_v     : 送信データ格納先へのポインタ
 * @param[in] count      : 送信データサイズ
 * @return int:  SPI_ERR_CODE に従う
 */
int SPI_tx(void* my_spi_v, void* data_v, int data_size);

/**
 * @brief 指定されたchを開きなおす
 * @param[in] my_spi_v 開きなおす際のポート設定
 * @param[in] reason   reopenする理由．0は正常操作を示す TODO: reasonのenumを作る？
 * @return int:  SPI_ERR_CODE に従う
 */
int SPI_reopen(void* my_spi_v, int reason);

/**
 * @brief rx_lengthを設定する
 * @param[in] my_spi_v  : 対象とするSPI_Config構造体へのポインタ
 * @param[in] rx_length : 設定する受信データ長
 * @return なし
 */
void SPI_set_rx_length(void* my_spi_v, const uint16_t rx_length);

/**
 * @brief cs_state_after_txを設定する
 * @param[in] my_spi_v           : 対象とするSPI_Config構造体へのポインタ
 * @param[in] cs_state_after_tx  : 設定するcs_state_after_tx
 * @return なし
 */
void SPI_set_cs_state_after_tx(void* my_spi_v, const SPI_CS_STATE_AFTER_TX cs_state_after_tx);

#endif
