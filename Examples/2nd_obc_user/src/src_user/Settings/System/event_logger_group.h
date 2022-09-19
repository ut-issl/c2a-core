/**
 * @file
 * @brief event_logger の イベント識別IDを定義
 * @note  前半は C2A core で使われるため，IDのかぶりに注意すること
 */
#ifndef EVENT_LOGGER_GROUP_H_
#define EVENT_LOGGER_GROUP_H_

/**
 * @enum  EL_GROUP
 * @brief event_logger の Event Group の user 定義部分
 * @note  uint32_t を想定
 * @note  前半の id は C2A core 側で EL_CORE_GROUP として定義されていることに注意する
 */
typedef enum
{
  // C2A CORE: 0-49
  // src_core/System/EventManager/event_logger.h の EL_CORE_GROUP にて定義

  // とりあえず最大値は0xffに！
  EL_GROUP_MAX = 0x100       // これはいじらない！
} EL_GROUP;

#endif
