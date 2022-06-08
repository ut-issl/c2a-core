/**
 * @file
 * @brief   uart_sils_sci_if
 * @details SILSでDriverのテストをするように作った
            ccsds_sils_sci_if.c/hのほぼコピー
 */
#ifndef UART_SILS_SCI_IF_HPP_
#define UART_SILS_SCI_IF_HPP_

#include "sils_sci_if.hpp"

class SCIComPortUart : public SCIComPort
{
public:
    SCIComPortUart(int port) : SCIComPort(port) {};
    ~SCIComPortUart(void) {};
};

int SILS_SCI_UART_IF_init();
int SILS_SCI_UART_IF_TX(unsigned char* data_v, int count);
int SILS_SCI_UART_IF_RX(unsigned char* data_v, int count);

#endif
