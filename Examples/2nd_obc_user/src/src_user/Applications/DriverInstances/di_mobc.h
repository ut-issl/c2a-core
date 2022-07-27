/**
 * @file   di_mobc.h
 * @brief  MOBC の DI． MOBC における GS との DI に相当する
 */
#ifndef DI_MOBC_H_
#define DI_MOBC_H_

#include "../../Drivers/Etc/mobc.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

extern const MOBC_Driver* const mobc_driver;           //!< MOBC driver

/**
 * @brief  MOBC update アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_MOBC_update(void);

/**
 * @brief  MOBC MS TLM (mission telemetry) PH (packet handler) アプリ生成関数
 * @param  void
 * @return AppInfo
 * @note   ひとまずテレメは MST のみ実装する
 */
AppInfo DI_MOBC_ms_tlm_packet_handler(void);

#endif
