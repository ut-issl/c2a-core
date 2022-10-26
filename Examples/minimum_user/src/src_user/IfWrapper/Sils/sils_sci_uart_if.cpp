#pragma section REPRO
/**
 * @file
 * @brief   sils_sci_uart_if
 * @details SILSでDriverのテストをするように作った
 */

#include "sils_sci_uart_if.hpp"


// 最初だけ初期化して、プログラム終了時にポートを閉じるようにしたい
#ifdef _WIN32
static SCIComPortUart SILS_SCI_UART_IF_sci_com_(13);
#else
static SCIComPortUart SILS_SCI_UART_IF_sci_com_(3);
#endif


int SILS_SCI_UART_IF_init(void)
{
  return 0;
}

int SILS_SCI_UART_IF_TX(unsigned char* data_v, int count)
{
  SILS_SCI_UART_IF_sci_com_.Send(data_v, 0, count);
  return 0;
}

int SILS_SCI_UART_IF_RX(unsigned char* data_v, int count)
{
  return SILS_SCI_UART_IF_sci_com_.Receive(data_v, 0, count);
}

#pragma section
