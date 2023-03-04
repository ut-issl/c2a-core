#pragma section REPRO
/**
 * @file
 * @brief UART と DriverSuper テスト用
 */
#include "di_uart_test.h"
#include <stddef.h> // for NULL
#include <src_core/Library/print.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../Settings/port_config.h"
#include "../../Settings/DriverSuper/driver_buffer_define.h"

static void UART_TEST_init_by_AM_(void);
static void UART_TEST_init_(void);
static void UART_TEST_update_(void);
// TODO: 実装する
// static int  UART_TEST_fill_with_zero_(uint32_t no);
// static int  UART_TEST_abort_fill_nodata_(uint8_t err);
// static int  UART_TEST_set_rec_flag_(uint32_t no);
// static int  UART_TEST_unset_rec_flag_(uint32_t no);
// static int  UART_TEST_is_rec_flag_up_(uint32_t no);

static UART_TEST_Driver uart_test_driver_;
const UART_TEST_Driver* const uart_test_driver = &uart_test_driver_;

// バッファ
static DS_StreamRecBuffer DI_UART_TEST_rx_buffer_0_;
static DS_StreamRecBuffer DI_UART_TEST_rx_buffer_1_;
static uint8_t DI_UART_TEST_rx_buffer_allocation_0_[DS_STREAM_REC_BUFFER_SIZE_DEFAULT];
static uint8_t DI_UART_TEST_rx_buffer_allocation_1_[DS_STREAM_REC_BUFFER_SIZE_DEFAULT];


// !!!!!!!!!! 注意 !!!!!!!!!!
// このDriverは，AM_initialize_app_ では初期化されない！！
// initコマンドで初期化すること
AppInfo UART_TEST_update(void)
{
  return AI_create_app_info("update_uart_test", UART_TEST_init_by_AM_, UART_TEST_update_);
}


static void UART_TEST_init_by_AM_(void)
{
  // ひとまず何もしない
}


static void UART_TEST_init_(void)
{
  DS_ERR_CODE ret1;
  DS_INIT_ERR_CODE ret2;
  DS_StreamRecBuffer* rx_buffers[DS_STREAM_MAX];
  DS_nullify_stream_rec_buffers(rx_buffers);

  ret1 = DS_init_stream_rec_buffer(&DI_UART_TEST_rx_buffer_0_,
                                   DI_UART_TEST_rx_buffer_allocation_0_,
                                   sizeof(DI_UART_TEST_rx_buffer_allocation_0_));
  if (ret1 != DS_ERR_CODE_OK)
  {
    Printf("UART_TEST buffer0 init Failed ! %d \n", ret1);
  }
  ret1 = DS_init_stream_rec_buffer(&DI_UART_TEST_rx_buffer_1_,
                                   DI_UART_TEST_rx_buffer_allocation_1_,
                                   sizeof(DI_UART_TEST_rx_buffer_allocation_1_));
  if (ret1 != DS_ERR_CODE_OK)
  {
    Printf("UART_TEST buffer1 init Failed ! %d \n", ret1);
  }

  if (DS_STREAM_MAX < 2)
  {
    Printf("UART_TEST init Failed ! Lack of streams \n");
    return;
  }

  rx_buffers[0] = &DI_UART_TEST_rx_buffer_0_;
  rx_buffers[1] = &DI_UART_TEST_rx_buffer_1_;

  ret2 = UART_TEST_init(&uart_test_driver_, PORT_CH_UART_TEST, rx_buffers);
  if (ret2 != DS_INIT_OK)
  {
    Printf("UART_TEST init Failed! Err:%d \n", ret2);
  }

  Printf("######################### \n");
  Printf(" UART_TEST init !!!!!!!! \n");
  Printf("######################### \n");
}


static void UART_TEST_update_(void)
{
  DS_REC_ERR_CODE ret;

  ret = UART_TEST_rec(&uart_test_driver_);

  // TODO: エラー処理
  (void)ret;
}


CCP_CmdRet Cmd_UART_TEST_INIT_DI(const CommonCmdPacket* packet)
{
  (void)packet;

  UART_TEST_init_();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_UART_TEST_UPDATE(const CommonCmdPacket* packet)
{
  (void)packet;

  UART_TEST_update_();

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_UART_TEST_SEND_TEST(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t id;
  DS_CMD_ERR_CODE ret;

  id = param[0];

  ret = UART_TEST_send(&uart_test_driver_, id);
  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

#pragma section
