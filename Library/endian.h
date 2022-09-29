/**
 * @file
 * @brief エンディアンに関するものを定義する
 */
#ifndef ENDIAN_H_
#define ENDIAN_H_

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

#endif
