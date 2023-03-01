#pragma section REPRO
/**
 * @file
 * @brief テスト用UARTのDriver
 */

#include "uart_test.h"
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_core/Library/print.h>
#include "../../Settings/sils_define.h"
#include "string.h"   // for memcpy
#include <stdio.h>    // SILSでのprint

// ヘッダーフッター
#define UART_TEST_HEADER_SIZE        (8)
#define UART_TEST_FOOTER_SIZE        (2)
#define UART_TEST_TX_FRAME_SIZE_MAX  (16)

static const uint8_t UART_TEST_header_[UART_TEST_HEADER_SIZE] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7};
static const uint8_t UART_TEST_footer_[UART_TEST_FOOTER_SIZE] = {0xBF, 0xBE};

#define UART_TEST_STREAM_FIX   (0)   //!< 固定長
#define UART_TEST_STREAM_VAR   (1)   //!< 可変長

static uint8_t UART_TEST_tx_frame_[UART_TEST_TX_FRAME_SIZE_MAX];

static DS_ERR_CODE UART_TEST_load_driver_super_init_settings_(DriverSuper* p_super);
static DS_ERR_CODE UART_TEST_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver);


DS_INIT_ERR_CODE UART_TEST_init(UART_TEST_Driver* uart_test_driver, uint8_t ch, DS_StreamRecBuffer* rx_buffers[DS_STREAM_MAX])
{
  DS_ERR_CODE ret;

  uart_test_driver->driver.uart_config.ch         = ch;
  uart_test_driver->driver.uart_config.baudrate   = 38400;
  uart_test_driver->driver.uart_config.parity_settings = PARITY_SETTINGS_NONE;
  uart_test_driver->driver.uart_config.data_length = UART_DATA_LENGTH_8BIT;
  uart_test_driver->driver.uart_config.stop_bit = UART_STOP_BIT_1BIT;

  ret = DS_init_streams(&(uart_test_driver->driver.super),
                        &(uart_test_driver->driver.uart_config),
                        rx_buffers,
                        UART_TEST_load_driver_super_init_settings_);
  if (ret != DS_ERR_CODE_OK) return DS_INIT_DS_INIT_ERR;
  return DS_INIT_OK;
}


static DS_ERR_CODE UART_TEST_load_driver_super_init_settings_(DriverSuper* p_super)
{
  DS_StreamConfig* p_stream_config;

  p_super->interface = UART;

  // stream0の設定
  p_stream_config = &(p_super->stream_config[UART_TEST_STREAM_FIX]);
  DSSC_enable(p_stream_config);
  DSSC_enable_strict_frame_search(p_stream_config);

  // 送信はする
  DSSC_set_tx_frame(p_stream_config, UART_TEST_tx_frame_);  // 送る直前に中身をmemcpyする
  DSSC_set_tx_frame_size(p_stream_config, 0);               // 送る直前に値をセットする

  // 定期的な受信はする
  DSSC_set_rx_header(p_stream_config, UART_TEST_header_, UART_TEST_HEADER_SIZE);
  DSSC_set_rx_footer(p_stream_config, UART_TEST_footer_, UART_TEST_FOOTER_SIZE);
  DSSC_set_rx_frame_size(p_stream_config, 12);
  DSSC_set_data_analyzer(p_stream_config, UART_TEST_analyze_rec_data_);

  // stream1の設定
  p_stream_config = &(p_super->stream_config[UART_TEST_STREAM_VAR]);
  DSSC_enable(p_stream_config);
  DSSC_enable_strict_frame_search(p_stream_config);

  // 送信はしない

  // 定期的な受信はする
  DSSC_set_rx_header(p_stream_config, UART_TEST_header_, UART_TEST_HEADER_SIZE);
  DSSC_set_rx_footer(p_stream_config, UART_TEST_footer_, UART_TEST_FOOTER_SIZE);
  DSSC_set_rx_frame_size(p_stream_config, -1);    // 可変
  DSSC_set_rx_framelength_pos(p_stream_config, UART_TEST_HEADER_SIZE);
  DSSC_set_rx_framelength_type_size(p_stream_config, 2);
  DSSC_set_rx_framelength_offset(p_stream_config, UART_TEST_HEADER_SIZE + UART_TEST_FOOTER_SIZE);
  DSSC_set_data_analyzer(p_stream_config, UART_TEST_analyze_rec_data_);

  // 定期TLMの監視機能の有効化しない → ので設定上書きなし

  return DS_ERR_CODE_OK;
}


DS_REC_ERR_CODE UART_TEST_rec(UART_TEST_Driver* uart_test_driver)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* p_stream_config;

  ret = DS_receive(&(uart_test_driver->driver.super));

  if (ret != DS_ERR_CODE_OK) return DS_REC_DS_RECEIVE_ERR;

  p_stream_config = &(uart_test_driver->driver.super.stream_config[UART_TEST_STREAM_FIX]);
  if (DSSC_get_rec_status(p_stream_config)->status_code == DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    ret = DS_analyze_rec_data(&(uart_test_driver->driver.super), UART_TEST_STREAM_FIX, uart_test_driver);
  }

  p_stream_config = &(uart_test_driver->driver.super.stream_config[UART_TEST_STREAM_VAR]);
  if (DSSC_get_rec_status(p_stream_config)->status_code == DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    ret = DS_analyze_rec_data(&(uart_test_driver->driver.super), UART_TEST_STREAM_VAR, uart_test_driver);
  }

  // 返り値ぐだぐだだけど，まあテストコードなので．．．
  if (ret != DS_ERR_CODE_OK) return DS_REC_ANALYZE_ERR;

  return DS_REC_OK;
}


static DS_ERR_CODE UART_TEST_analyze_rec_data_(DS_StreamConfig* p_stream_config, void* p_driver)
{
  UART_TEST_Driver* uart_test_driver = (UART_TEST_Driver*)p_driver;
  uint16_t i;

  (void)(uart_test_driver);

#ifndef SILS_FW
  Printf("DEC DATA: %d\n", DSSC_get_rec_status(p_stream_config)->fixed_frame_len);
  for (i = 0; i < DSSC_get_rec_status(p_stream_config)->fixed_frame_len; i++)
  {
    Printf("%02x ", DSSC_get_rx_frame(p_stream_config)[i]);
  }
  Printf("             \n");
#else
  printf("DEC DATA: %d\n", DSSC_get_rec_status(p_stream_config)->fixed_frame_len);
  for (i = 0; i < DSSC_get_rec_status(p_stream_config)->fixed_frame_len; i++)
  {
    printf("%02x ", DSSC_get_rx_frame(p_stream_config)[i]);
  }
  printf("             \n");
#endif
  return DS_ERR_CODE_OK;
}


DS_CMD_ERR_CODE UART_TEST_send(UART_TEST_Driver* uart_test_driver, uint8_t id)
{
  DS_ERR_CODE ret;
  DS_StreamConfig* p_stream_config;

  p_stream_config = &(uart_test_driver->driver.super.stream_config[UART_TEST_STREAM_FIX]);

  switch (id)
  {
  case 0:
    UART_TEST_tx_frame_[0] = 'i';
    UART_TEST_tx_frame_[1] = 'd';
    UART_TEST_tx_frame_[2] = '0';
    UART_TEST_tx_frame_[3] = 'a';
    UART_TEST_tx_frame_[4] = 'b';
    UART_TEST_tx_frame_[5] = 0x0d;
    UART_TEST_tx_frame_[6] = 0x0a;
    DSSC_set_tx_frame_size(p_stream_config, 7);
    break;
  case 1:
    UART_TEST_tx_frame_[0] = 'i';
    UART_TEST_tx_frame_[1] = 'd';
    UART_TEST_tx_frame_[2] = '1';
    UART_TEST_tx_frame_[3] = 'A';
    UART_TEST_tx_frame_[4] = 'B';
    UART_TEST_tx_frame_[5] = 0x0d;
    UART_TEST_tx_frame_[6] = 0x0a;
    DSSC_set_tx_frame_size(p_stream_config, 7);
    break;
  case 2:
    UART_TEST_tx_frame_[0] = 'i';
    UART_TEST_tx_frame_[1] = 'd';
    UART_TEST_tx_frame_[2] = '2';
    UART_TEST_tx_frame_[3] = 0x0d;
    UART_TEST_tx_frame_[4] = 0x0a;
    DSSC_set_tx_frame_size(p_stream_config, 5);
    break;
  default:
    UART_TEST_tx_frame_[0] = 'd';
    UART_TEST_tx_frame_[1] = 'e';
    UART_TEST_tx_frame_[2] = 'f';
    UART_TEST_tx_frame_[3] = 'a';
    UART_TEST_tx_frame_[4] = 'u';
    UART_TEST_tx_frame_[5] = 'l';
    UART_TEST_tx_frame_[6] = 't';
    UART_TEST_tx_frame_[7] = 0x0d;
    UART_TEST_tx_frame_[8] = 0x0a;
    DSSC_set_tx_frame_size(p_stream_config, 9);
    break;
  }

  ret = DS_send_general_cmd(&(uart_test_driver->driver.super), UART_TEST_STREAM_FIX);
  if (ret == DS_ERR_CODE_OK)
  {
    return DS_CMD_OK;
  }
  else
  {
    return DS_CMD_DRIVER_SUPER_ERR;
  }
}

#pragma section
