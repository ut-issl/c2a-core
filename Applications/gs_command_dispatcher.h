/**
 * @file
 * @brief ínè„ã«Ç©ÇÁî≠çsÇ≥ÇÍÇΩ RTC (Real Time Cmd) Çé¿çsÇ∑ÇÈ
 */
#ifndef GS_COMMAND_DISPATCHER_H_
#define GS_COMMAND_DISPATCHER_H_

#include "../CmdTlm/command_dispatcher.h"
#include "../System/ApplicationManager/app_info.h"

extern const CommandDispatcher* const gs_command_dispatcher;

/**
 * @brief  GSCD App ê∂ê¨ä÷êî
 * @param  void
 * @return GSCD AppInfo
 */
AppInfo GSCD_create_app(void);

CCP_EXEC_STS Cmd_GSCD_CLEAR_ERR_LOG(const CTCP* packet);

#endif
