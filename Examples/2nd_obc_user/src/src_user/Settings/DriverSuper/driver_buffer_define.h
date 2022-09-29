/**
 * @file
 * @brief  Driver の各種バッファサイズなどの定義
 */
#ifndef DRIVER_BUFFER_DEFINE_H_
#define DRIVER_BUFFER_DEFINE_H_

#define DS_RX_BUFFER_SIZE_UART                      (1024)    //!< UART ラインの物理バッファサイズ
#define DS_RX_FRAME_BUFFER_SIZE_DEFAULT             (1024)    //!< rx_frame_buffer_size_ のデフォルト値
#define DS_RX_FRAME_CARRY_OVER_BUFFER_SIZE_DEFAULT  (1024)    //!< rx_frame_carry_over_buffer_size_ のデフォルト値

#endif
