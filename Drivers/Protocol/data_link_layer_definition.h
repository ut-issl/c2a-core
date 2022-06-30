/**
 * @file
 * @brief Driver Super の通信で使うプロトコルスタックのデータリンク層定義
 */
#ifndef DATA_LINK_LAYER_DEFINITION_H_
#define DATA_LINK_LAYER_DEFINITION_H_

/**
 * @enum   DATA_LINK_LAYER
 * @brief  データリンク層の定義
 * @note   uint8_t を想定
 */
typedef enum
{
  DATA_LINK_LAYER_EB90_FRAME,   //!< EB90 Frame
  DATA_LINK_LAYER_UNKNOWN       //!< 不明なデータリンク層
} DATA_LINK_LAYER;

#endif
