/**
 * @file
 * @brief  DriverSuperパラメタのオーバーライド用ヘッダー
 */
#ifndef DRIVER_SUPER_PARAMS_H_
#define DRIVER_SUPER_PARAMS_H_

#undef DS_STREAM_MAX
#undef DS_RX_PROCESSING_BUFFER_SIZE

#define DS_STREAM_MAX                 (3)
#define DS_RX_PROCESSING_BUFFER_SIZE  (1024 * 2)

#endif
