/**
 * @file
 * @brief エンディアンに関するものを定義する
 */
#ifndef ENDIAN_H_
#define ENDIAN_H_

#include <stddef.h>

/**
 * @enum  ENDIAN_TYPE
 * @brief エンディアンを定義するため
 * @note  uint8_t を想定
 */
typedef enum
{
  ENDIAN_TYPE_BIG,      //!< big-endian
  ENDIAN_TYPE_LITTLE,   //!< little-endian
  ENDIAN_TYPE_UNKNOWN   //!< 不明
} ENDIAN_TYPE;

/**
 * @brief  エンディアンを考慮した memcpy
 * @note   RAM 上の内部変数と，ビッグエンディアンでなされる外部との通信バッファなど
 *         （uint8_t の配列と同等の構造）とのコピーで用いる
 * @param[out] dest: コピー先メモリポインタ
 * @param[in]  src:  コピー元メモリポインタ
 * @param[in]  size: コピーバイト数
 * @return dest
 */
void* ENDIAN_memcpy(void* dest, const void* src, size_t size);

/**
 * @brief  エンディアンの変更
 * @note   uint8_t の配列とみなして，順番を逆転する
 * @param[out] after:  変更後
 * @param[in]  before: 変更前
 * @param[in]  size:   処理データのバイト数
 * @return void
 */
void ENDIAN_conv(void* after, const void* before, size_t size);

#endif
