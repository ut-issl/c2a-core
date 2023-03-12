#ifndef UTILITY_COUNTER_H_
#define UTILITY_COUNTER_H_
// FIXME: このAppは現在管理されていない！
//        使用する前に確認すること

#if 0
// FIXME: 現在 AL に依存してしまっている
#include <src_user/Settings/System/anomaly_logger_define.h>
#ifdef AL_ENABLE

#include "../System/ApplicationManager/app_info.h"
#include "../System/AnomalyLogger/anomaly_logger.h"
#include "../TlmCmd/common_cmd_packet.h"

// 汎用カウンタ用構造体
typedef enum
{
  UTIL_COUNTER_RES00 = 0,
  UTIL_COUNTER_RES01,
  UTIL_COUNTER_RES02,
  UTIL_COUNTER_RES03,
  UTIL_COUNTER_RES04,
  UTIL_COUNTER_RES05,
  UTIL_COUNTER_RES06,
  UTIL_COUNTER_RES07,
  UTIL_COUNTER_RES08,
  UTIL_COUNTER_RES09,
  UTIL_COUNTER_RES10,
  UTIL_COUNTER_RES11,
  UTIL_COUNTER_RES12,
  UTIL_COUNTER_RES13,
  UTIL_COUNTER_RES14,
  UTIL_COUNTER_RES15,
  UTIL_COUNTER_RES16,
  UTIL_COUNTER_RES17,
  UTIL_COUNTER_RES18,
  UTIL_COUNTER_RES19,
  UTIL_COUNTER_RES20,
  UTIL_COUNTER_RES21,
  UTIL_COUNTER_RES22,
  UTIL_COUNTER_RES23,
  UTIL_COUNTER_RES24,
  UTIL_COUNTER_RES25,
  UTIL_COUNTER_RES26,
  UTIL_COUNTER_RES27,
  UTIL_COUNTER_RES28,
  UTIL_COUNTER_RES29,
  UTIL_COUNTER_RES30,
  UTIL_COUNTER_RES31,
  UTIL_COUNTER_MAX
} UTIL_COUNTER_NAME;

typedef struct
{
  unsigned int counter;
  unsigned int threshold;
  unsigned char anomaly_active;
} UTIL_COUNTER_counter;

typedef struct
{
  UTIL_COUNTER_counter cnt[UTIL_COUNTER_MAX];
} UtilityCounter;

extern const UtilityCounter* const utility_counter;

AppInfo UTIL_COUNTER_create_app(void);

// コマンド
CCP_CmdRet Cmd_UTIL_COUNTER_INCREMENT(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_UTIL_COUNTER_RESET(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_UTIL_COUNTER_SET_PARAM(const CommonCmdPacket* packet);

#endif
#endif
#endif
