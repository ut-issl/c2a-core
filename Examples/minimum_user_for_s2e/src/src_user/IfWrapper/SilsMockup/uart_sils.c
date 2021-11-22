#pragma section REPRO
#include <src_core/IfWrapper/uart.h>

int UART_init(void* my_uart_v)
{
  (void)my_uart_v;
  return 0;
}

int UART_rx(void* my_uart_v, void* data_v, int buffer_size)
{
  (void)my_uart_v;
  (void)data_v;
  (void)buffer_size;
  return 0;
}

int UART_tx(void* my_uart_v, void* data_v, int data_size)
{
  (void)my_uart_v;
  (void)data_v;
  (void)data_size;
  return 0;
}

int UART_reopen(void* my_uart_v, int reason)
{
  (void)my_uart_v;
  (void)reason;
  return 0;
}

#pragma section
