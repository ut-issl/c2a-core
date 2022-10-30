/**
 * @file
 * @brief   sils_sci_uart_if
 * @details SILSでDriverのテストをするように作った
            sils_sci_ccsds_if.c/hのほぼコピー
 */
#ifndef SILS_SCI_UART_IF_HPP_
#define SILS_SCI_UART_IF_HPP_

#include "sils_sci_if.hpp"

class SCIComPortUart : public SCIComPort
{
public:
    SCIComPortUart(int port) : SCIComPort(port) {};
    ~SCIComPortUart(void) {};
};

int SILS_SCI_UART_IF_init();
int SILS_SCI_UART_IF_tx(unsigned char* data_v, int count);
int SILS_SCI_UART_IF_rx(unsigned char* data_v, int count);

#endif
