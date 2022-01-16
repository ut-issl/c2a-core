/**
 * @file
 * @brief  テレメトリ定義に必要なヘッダー
 */
#ifndef TELEMETRY_SOURCE_H_
#define TELEMETRY_SOURCE_H_

#include "string.h" // for memcpy
#include <src_core/System/ApplicationManager/app_manager.h>
#include <src_core/System/TaskManager/task_dispatcher.h>
#include <src_core/System/ModeManager/mode_manager.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/System/EventManager/event_handler.h>
#include <src_core/System/WatchdogTimer/watchdog_timer.h>
#include <src_core/TlmCmd/command_analyze.h>
#include <src_core/TlmCmd/block_command_executor.h>
#include "../Applications/app_headers.h"
#include "../Library/git_revision.h"
#include "../Drivers/Com/gs_validate.h"
#include "../Settings/build_settings.h"

#endif
