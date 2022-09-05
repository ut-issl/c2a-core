/**
 * @file
 * @brief TLCD_ID の定義
 * @note  この ID は App や Core や各所で参照され，循環参照の原因となるので，分離
 */
#ifndef TIMELINE_COMMAND_DISPATCHER_ID_DEFINE_H_
#define TIMELINE_COMMAND_DISPATCHER_ID_DEFINE_H_

#include <src_user/Settings/Applications/timeline_command_dispatcher_define.h>

/**
 * @enum   TLCD_ID
 * @brief  TimeLineを選ぶときに統一的に使うコード
 * @note   uint8_tを想定
 */
typedef enum
{
  TLCD_ID_FROM_GS = 0,
  TLCD_ID_DEPLOY_BC,
  TLCD_ID_DEPLOY_TLM,
#ifdef TLCD_ENABLE_MISSION_TL
  TLCD_ID_FROM_GS_FOR_MISSION,
#endif
  TLCD_ID_MAX
} TLCD_ID;
// FIXME: TL本数を可変にできるようにする

#endif
