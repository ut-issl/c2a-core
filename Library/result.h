/**
 * @file
 * @brief Result enum の定義
 */
#ifndef RESULT_H_
#define RESULT_H_

/**
 * @enum   RESULT
 * @brief  Rust でいうところの Result<(), ()> のような，汎用的な成功したかどうかだけの返り値用 enum
 * @note   uint8_t を想定
 */
typedef enum
{
  RESULT_OK = 0,  //!< OK （C2A では慣習的に OK 系列挙子は 0 を使うので，万が一他の enum に cast されても事故が起きにくいように）
  RESULT_ERR      //!< Error or NG
} RESULT;

#endif
