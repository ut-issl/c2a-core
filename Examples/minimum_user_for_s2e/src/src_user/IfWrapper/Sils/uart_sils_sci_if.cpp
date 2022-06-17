#pragma section REPRO
/**
 * @file
 * @brief   uart_sils_sci_if
 * @details SILSでDriverのテストをするように作った
 */

#include "uart_sils_sci_if.hpp"

int SILS_SCI_UART_IF_init(void)
{
  return 0;
}

int SILS_SCI_UART_IF_TX(unsigned char* data_v, int count)
{
  SILS_SCI_IF_sci_com_uart_.Send(data_v, 0, count);
  return 0;
}

int SILS_SCI_UART_IF_RX(unsigned char* data_v, int count)
{
  return SILS_SCI_IF_sci_com_uart_.Receive(data_v, 0, count);
}

#pragma section
