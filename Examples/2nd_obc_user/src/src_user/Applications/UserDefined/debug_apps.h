#ifndef DEBUG_APPS_H_
#define DEBUG_APPS_H_

#include <src_core/System/ApplicationManager/app_info.h>

AppInfo APP_DBG_flush_screen(void);
AppInfo APP_DBG_print_time_stamp(void);
AppInfo APP_DBG_print_cmd_status(void);
// 実行時間の問題から分割している
AppInfo APP_DBG_print_event_logger0(void);
AppInfo APP_DBG_print_event_logger1(void);
AppInfo APP_DBG_print_event_handler(void);
AppInfo APP_DBG_print_git_rev(void);

#endif
