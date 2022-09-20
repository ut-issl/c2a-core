/**
 * @file
 * @brief CRC用のライブラリ
 */
#ifndef CRC_H_
#define CRC_H_

#include <stddef.h>   // for size_t
#include <src_user/Library/stdint.h>

/**
 * @brief CRC-16-IBM
 *
 *        生成多項式: x^16 + x^15 + x^2 + 1
 *        ビット送り: 右送り, POLLY: 0xa001
 *        読み出し: 1byte (8 bit)
 * @param[in] crc: CRC初期値
 * @param[in] c: CRCを計算するbyte列
 * @param[in] n: 列の長さ
 * @param[in] rev_flag: 反転するかどうか
 * @return uint16_t: 計算結果
 */
uint16_t CRC_calc_crc_16_ibm_right(uint16_t crc, const uint8_t* c, size_t n, int rev_flag);

/**
 * @brief CRC-16-CCITT
 *
 *        生成多項式: x^16 + x^12 + x^5 + 1
 *        ビット送り: 左送り, POLLY: 0x1021
 *        読み出し: 1byte (8 bit)
 * @param[in] crc: CRC初期値
 * @param[in] c: CRCを計算するbyte列
 * @param[in] n: 列の長さ
 * @param[in] rev_flag: 反転するかどうか
 * @return uint16_t: 計算結果
 */
uint16_t CRC_calc_crc_16_ccitt_left(uint16_t crc, const uint8_t* c, size_t n, int rev_flag);

/**
 * @brief CRC-16-CCITT
 *
 *        生成多項式: x^16 + x^12 + x^5 + 1
 *        ビット送り: 右送り, POLLY: 0x8408
 *        読み出し: 1byte (8 bit)
 * @param[in] crc: CRC初期値
 * @param[in] c: CRCを計算するbyte列
 * @param[in] n: 列の長さ
 * @param[in] rev_flag: 反転するかどうか
 * @return uint16_t: 計算結果
 */
uint16_t CRC_calc_crc_16_ccitt_right(uint16_t crc, const uint8_t* c, size_t n, int rev_flag);

/**
 * @brief crc用のテーブル作成
 *
 *        読み出し: 1byte (8bit) (sizeof(table) = 256)
 * @param[out] table: テーブル
 * @param[in] crc_poly: 生成多項式
 * @param[in] shift: 右:1, 左:0
 * @note 使わないとは思いますが...
 */
void CRC_make_crc_8_table(uint8_t* table, uint8_t crc_poly, uint8_t shift);
void CRC_make_crc_16_table(uint16_t* table, uint16_t crc_poly, uint8_t shift);
void CRC_make_crc_32_table(uint32_t* table, uint32_t crc_poly, uint8_t shift);

// 使わないと思いますがデバッグ用に
// uint16_t CRC_calc_crc_16_right(uint16_t crc, uint16_t crc_poly, const uint8_t* c, size_t n, int rev_flag);
// uint16_t CRC_calc_crc_16_left(uint16_t crc, uint16_t crc_poly, const uint8_t* c, size_t n, int rev_flag);

#endif
