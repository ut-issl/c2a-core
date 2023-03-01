/**
 * @file
 * @brief Result enum の定義
 */
#ifndef RESULT_H_
#define RESULT_H_

/**
 * @enum   RESULT
 * @brief  Rust でいうところの Result<T, E> のような，汎用的な返り値用 enum
 * @note   uint8_t を想定
 */
typedef enum
{
  RESULT_OK = 0,  //!< OK
  RESULT_ERR      //!< Error or NG
} RESULT;

#endif
