/**
 * @file
 * @brief  DriverSuperパラメタのオーバーライド用ヘッダー
 */
#ifndef DRIVER_SUPER_PARAMS_H_
#define DRIVER_SUPER_PARAMS_H_

#undef DS_STREAM_MAX
#undef DS_RX_BUFFER_SIZE_MAX
#undef DS_RX_FRAME_SIZE_MAX

#define DS_STREAM_MAX          (3)
#define DS_RX_BUFFER_SIZE_MAX  (1024)
#define DS_RX_FRAME_SIZE_MAX   (1024)

#endif
