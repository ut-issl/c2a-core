/**
 * @file
 * @brief I2C通信(Master側)のラッパークラスです。
 * @note I2C通信ラッパーは、
 *       I2Cのインターフェースを実現し、
 *       I2Cポートの初期化、データ送信、データ受信を行う、
 *       I2C通信ラッパーのクラスです。
 *       個々の機器のインターフェースモジュールに継承させて使用します。
 */

#ifndef I2C_H_
#define I2C_H_

#include <src_user/Library/stdint.h>

 /**
  * @struct I2cConfig
  * @brief I2Cポートの初期化、データ送信・受信の際に必要となる設定情報を格納する構造体です。
  */
typedef struct
{
  uint8_t  ch;                 //!< 継承先の機器がつながっているポート番号
  uint32_t frequency_khz;      //!< I2Cクロックの周波数 単位:kHz
  uint8_t  device_address;     //!< 継承先の機器のアドレス
  uint8_t  stop_flag;          //!< 最後にSTOPを送るか否かを選択 0->送らない，それ以外->送る
  uint32_t timeout_threshold;  //!< この値を超えるとタイムアウトエラーとなる 単位:カウント数（ユーザー依存） TODO: 時刻にするか要検討
  uint32_t rx_length;          //!< I2C通信における受信データ長 単位: Byte
} I2C_Config;

/**
 * @enum  I2C_ERR_CODE
 * @brief I2C用の汎用エラーコード
 * @note  型はint8_tを想定しているが, if_listでラップされているためI2Cの公開関数の返り値はintとなっている
 *        基本負なのはRXの返り値は正が送ったByte数, 負がエラーコードとなっているため
 */
typedef enum
{
  I2C_UNKNOWN_ERR     = -20,  //!< 原因不明
  I2C_RX_BUFFER_SMALL = -14,  //!< 受信バッファサイズが受信データ長より小さい
  I2C_DATA_NEGA_ERR   = -13,  //!< 送受信データサイズが負のエラー
  I2C_TIMEOUT         = -10,  //!< 通信時のTimeout
  I2C_YET_ERR         = -4,   //!< チャンネル未オープン
  I2C_ALREADY_ERR     = -3,   //!< チャンネルオープン済み
  I2C_FREQUENCY_ERR   = -2,   //!< 周波数異常
  I2C_CH_ERR          = -1,   //!< チャンネル異常 (Port_configに無い)
  I2C_OK              = 0     //!< OKは0を踏襲
} I2C_ERR_CODE;

/**
 * @brief I2cConfig構造体をグローバルインスタンスとして定義し、ポインタを渡すことでポートを初期化します。
 * @param[in] my_i2c_v 初期化するI2cConfig構造体へのポインタ
 * @return int I2C_ERR_CODEとしたいが, if_listでintと統一されている。
 * @note I2C通信ラッパーのクラスを使用する時は起動時に必ず実施してください。
 *       この関数を実行する前にI2cConfig構造体内の設定値を設定しておく必要があります。
 */
int I2C_init(void* my_i2c_v);

/**
 * @brief I2cConfig構造体にて指定されたchからデータを受信します
 * @param[in] my_i2c_v    : 対象とするI2cConfig構造体へのポインタ
 * @param[in] data_v      : 受信データ格納先へのポインタ
 * @param[in] buffer_size : 受信データ格納先のバッファサイズ、I2C通信による受信データ長よりも長い必要がある
 * @retval 0: 受信データなし
 * @retval 正: 受信データバイト数
 * @retval 負: エラー. I2C_ERR_CODE に従う
 * @note この関数内でI2C読み出し要求を送信しています
 */
int I2C_rx(void* my_i2c_v, void* data_v, int buffer_size);

/**
 * @brief I2cConfig構造体にて指定されたchへデータを送信します
 * @param[in] my_i2c_v : 対象とするI2cConfig構造体へのポインタ
 * @param[in] data_v     : 送信データ格納先へのポインタ
 * @param[in] count      : 送信データサイズ
 * @return int:  I2C_ERR_CODE に従う
 */
int I2C_tx(void* my_i2c_v, void* data_v, int data_size);

/**
 * @brief 指定されたchを開きなおす
 * @param[in] my_i2c_v 開きなおす際のポート設定
 * @param[in] reason   reopenする理由．0は正常操作を示す TODO: reasonのenumを作る？
 * @return int:  I2C_ERR_CODE に従う
 */
int I2C_reopen(void* my_i2c_v, int reason);

/**
 * @brief stop_flagを設定する
 * @param[in] stop_flag 設定するstop_flag
 * @return なし
 */
void I2C_set_stop_flag(void* my_i2c_v, const uint8_t stop_flag);

/**
 * @brief rx_lengthを設定する
 * @param[in] rx_length  設定する受信データ長
 * @return なし
 */
void I2C_set_rx_length(void* my_i2c_v, const uint32_t rx_length);

/**
 * @brief I2C_Config 構造体にて指定されたchから1バイトデータを書き込みます
 * @param[in] my_i2c_v : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] cmd_byte : コマンド
 * @param[in] data     : 書き込むデータ
 * @retval I2C_ERR_CODE
 */
I2C_ERR_CODE I2C_write_byte(void* my_i2c_v, uint8_t cmd_byte, uint8_t data);

/**
 * @brief I2C_Config 構造体にて指定されたchから2バイトデータを書き込みます
 * @param[in] my_i2c_v : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] cmd_byte : コマンド
 * @param[in] data     : 書き込むデータ
 * @retval I2C_ERR_CODE
 */
I2C_ERR_CODE I2C_write_2bytes(void* my_i2c_v, uint8_t cmd_byte, uint16_t data);

/**
 * @brief I2C_Config 構造体にて指定されたchから1バイトデータを読み込みます
 * @param[in] my_i2c_v : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] cmd_byte : コマンド
 * @param[out] data_v  : データ格納先へのポインタ
 * @retval I2C_ERR_CODE
 */
I2C_ERR_CODE I2C_read_byte(void* my_i2c_v, uint8_t cmd_byte, uint8_t* data_v);

/**
 * @brief I2C_Config 構造体にて指定されたchから2バイトデータを読み込みます
 * @param[in] my_i2c_v : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] cmd_byte : コマンド
 * @param[out] data_v  : データ格納先へのポインタ
 * @retval I2C_ERR_CODE
 */
I2C_ERR_CODE I2C_read_2bytes(void* my_i2c_v, uint8_t cmd_byte, uint16_t* data_v);

#endif
