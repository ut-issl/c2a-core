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
  // C2A CORE: 0-19
  // src_core/System/EventManager/event_logger.h の EL_CORE_GROUP にて定義

  // CDH: 20-39
  EL_GROUP_UART = 20,         // TODO
  EL_GROUP_UART_REOPEN,

  // TEST
  EL_GROUP_TEST             = 0xf0,        // pytest用
  EL_GROUP_TEST1            = 0xf1,        // pytest用
  EL_GROUP_TEST_EH          = 0xf2,        // pytest用 EH
  EL_GROUP_TEST_EH_RESPOND  = 0xf3,        // pytest用 EHの対応検知用
  EL_GROUP_TEST_EH1         = 0xf4,        // pytest用 EH

  // とりあえず最大値は0xffに！
  EL_GROUP_MAX = 0x100       // これはいじらない！
} EL_GROUP;

#endif
