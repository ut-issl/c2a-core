#pragma section REPRO
#include "if_list.h"

#include <stddef.h>

int (*IF_init[])(void* my_if) = { CCSDS_init, UART_init};

int (*IF_RX[])(void* my_if, void* data_v, int buffer_size) = { CCSDS_rx, UART_rx};

int (*IF_TX[])(void* my_if, void* data_v, int data_size) = { CCSDS_tx, UART_tx};

int (*IF_reopen[])(void* my_if, int reason) = { CCSDS_reopen, UART_reopen};

#pragma section
