/**
 * @file
 * @brief   uart_sils_sci_if
 * @details SILSでDriverのテストをするように作った
            ccsds_sils_sci_if.c/hのほぼコピー
 */
#ifndef UART_SILS_SCI_IF_HPP_
#define UART_SILS_SCI_IF_HPP_

#include <Windows.h>

class SCIComPortUart
{
public:
  SCIComPortUart(void);
  ~SCIComPortUart(void);

  int Send(unsigned char* buffer, size_t length, size_t offset);
  int Receive(unsigned char* buffer, size_t length, size_t offset);

private:
  HANDLE myHComPort_;
  DCB config_;
  bool init_success;
};

int SILS_SCI_UART_IF_init();
int SILS_SCI_UART_IF_TX(unsigned char* data_v, int count);
int SILS_SCI_UART_IF_RX(unsigned char* data_v, int count);

#endif
