#ifndef ANOMALY_LOGGER_GROUP_H_
#define ANOMALY_LOGGER_GROUP_H_

// #include <src_core/System/AnomalyLogger/anomaly_logger.h>
// ↑anomaly_logger.hからincludeされるべきであって，こっちからはincludeしない

// anomaly_logger.h で，コア部のAL_GROUPを既に定義している．
typedef enum
{
  // C2A CORE: 0-9
  // anomaly_logger.h のAL_GROUP参照

  // OBC: 10-29
  AL_GROUP_UART = 10,
  AL_GROUP_UTIL_CNT,
  AL_GROUP_SAMPLE,

  // とりあえず最大値は0xffに！
  AL_GROUP_MAX = 0x100       // これはいじらない！（あと，8の倍数であることを要請）
} AL_GROUP;

#endif
