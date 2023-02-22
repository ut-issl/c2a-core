/**
 * @file
 * @brief I2C 通信関連の一般 IO 以外の公開関数
 * @note レジスタの読み書き等，ハードウェアに依存しない関数群を定義する
 */

#ifndef I2C_COMMON_H_
#define I2C_COMMON_H_

#include "../../Drivers/Super/driver_super.h"
#include "../i2c.h"

/**
 * @brief I2C_Config 構造体にて指定されたデバイスのレジスタへ 1 バイトデータを書き込む
 * @param[in] p_super  : DriverSuper 構造体へのポインタ
 * @param[in] stream   : 使用する stream_config の番号
 * @param[in] p_i2c_config : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] register_address : 書き込むレジスタのアドレス
 * @param[in] data     : 書き込むデータ
 * @retval DS_ERR_CODE (DS_send_general_cmd の返り値)
 */
DS_ERR_CODE I2C_write_byte(DriverSuper* p_super, uint8_t stream, I2C_Config* p_i2c_config,
                           uint8_t register_address, uint8_t data);

/**
 * @brief I2C_Config 構造体にて指定されたデバイスのレジスタへ 2 バイトデータを書き込む
 * @param[in] p_super  : DriverSuper 構造体へのポインタ
 * @param[in] stream   : 使用する stream_config の番号
 * @param[in] p_i2c_config : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] register_address : 書き込むレジスタのアドレス
 * @param[in] data     : 書き込むデータ
 * @retval DS_ERR_CODE (DS_send_general_cmd の返り値)
 */
DS_ERR_CODE I2C_write_2bytes(DriverSuper* p_super, uint8_t stream, I2C_Config* p_i2c_config,
                             uint8_t register_address, uint16_t data);

/**
 * @brief I2C_Config 構造体にて指定されたデバイスのレジスタから 1 バイトデータを読み込む
 * @param[in] p_super  : DriverSuper 構造体へのポインタ
 * @param[in] stream   : 使用する stream_config の番号
 * @param[in] p_i2c_config : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] register_address : 読み込むレジスタのアドレス
 * @param[out] data_v  : データ格納先へのポインタ
 * @retval DS_ERR_CODE
 */
DS_ERR_CODE I2C_read_byte(DriverSuper* p_super, uint8_t stream, I2C_Config* p_i2c_config,
                          uint8_t register_address, uint8_t* data_v);

/**
 * @brief I2C_Config 構造体にて指定されたデバイスのレジスタから 2 バイトデータを読み込む
 * @param[in] p_super  : DriverSuper 構造体へのポインタ
 * @param[in] stream   : 使用する stream_config の番号
 * @param[in] p_i2c_config : 対象とする I2C_Config 構造体へのポインタ
 * @param[in] register_address : 読み込むレジスタのアドレス
 * @param[out] data_v  : データ格納先へのポインタ
 * @retval DS_ERR_CODE
 */
DS_ERR_CODE I2C_read_2bytes(DriverSuper* p_super, uint8_t stream, I2C_Config* p_i2c_config,
                            uint8_t register_address, uint16_t* data_v);

#endif
