/**
 * @file
 * @brief UART と DriverSuper テスト用
 */
#ifndef DI_UART_TEST_H_
#define DI_UART_TEST_H_

#include "../../Drivers/Etc/uart_test.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

extern const UART_TEST_Driver* const uart_test_driver;

// アプリケーション
AppInfo UART_TEST_update(void);

CCP_CmdRet Cmd_UART_TEST_INIT_DI(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_UART_TEST_UPDATE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_UART_TEST_SEND_TEST(const CommonCmdPacket* packet);


#endif
