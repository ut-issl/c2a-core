/**
 * @file
 * @brief I2C通信関連の一般IO以外の公開関数
 * @note レジスタの読み書き等，ハードウェアに依存しない関数群を定義する
 */

#ifndef I2C_COMMON_H_
#define I2C_COMMON_H_

#include "../i2c.h"
#include "../../Drivers/Super/driver_super.h"

/**
 * @brief I2C_Config 構造体にて指定されたchから1バイトデータを書き込みます
 * @param[in] p_super  : DriverSuper 構造体へのポインタ
 * @param[in] stream   : 使用する stream_config の番号
 * @param[in] my_i2c_v : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] cmd_byte : コマンド
 * @param[in] data     : 書き込むデータ
 * @retval DS_ERR_CODE
 */
DS_ERR_CODE I2C_write_byte(DriverSuper* p_super, uint8_t stream, void* my_i2c_v, uint8_t cmd_byte, uint8_t data);

/**
 * @brief I2C_Config 構造体にて指定されたchから2バイトデータを書き込みます
 * @param[in] p_super  : DriverSuper 構造体へのポインタ
 * @param[in] stream   : 使用する stream_config の番号
 * @param[in] my_i2c_v : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] cmd_byte : コマンド
 * @param[in] data     : 書き込むデータ
 * @retval DS_ERR_CODE
 */
DS_ERR_CODE I2C_write_2bytes(DriverSuper* p_super, uint8_t stream, void* my_i2c_v, uint8_t cmd_byte, uint16_t data);

/**
 * @brief I2C_Config 構造体にて指定されたchから1バイトデータを読み込みます
 * @param[in] p_super  : DriverSuper 構造体へのポインタ
 * @param[in] stream   : 使用する stream_config の番号
 * @param[in] my_i2c_v : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] cmd_byte : コマンド
 * @param[out] data_v  : データ格納先へのポインタ
 * @retval DS_ERR_CODE
 */
DS_ERR_CODE I2C_read_byte(DriverSuper* p_super, uint8_t stream, void* my_i2c_v, uint8_t cmd_byte, uint8_t* data_v);

/**
 * @brief I2C_Config 構造体にて指定されたchから2バイトデータを読み込みます
 * @param[in] p_super  : DriverSuper 構造体へのポインタ
 * @param[in] stream   : 使用する stream_config の番号
 * @param[in] my_i2c_v : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] cmd_byte : コマンド
 * @param[out] data_v  : データ格納先へのポインタ
 * @retval DS_ERR_CODE
 */
DS_ERR_CODE I2C_read_2bytes(DriverSuper* p_super, uint8_t stream, void* my_i2c_v, uint8_t cmd_byte, uint16_t* data_v);

#endif
