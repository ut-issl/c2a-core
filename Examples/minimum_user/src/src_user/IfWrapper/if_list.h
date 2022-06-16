#ifndef IF_LIST_H_
#define IF_LIST_H_


#include <src_core/IfWrapper/ccsds.h>
#include <src_core/IfWrapper/uart.h>

typedef enum
{
  CCSDS,
  UART,
  IF_LIST_MAX
} IF_LIST_ENUM;

typedef enum
{
  IF_REOPEN_TLM_DISRUPTION = 100
} IF_REOPEN_REASON;

extern  int     (*IF_init[]  )(void* my_if);
extern  int     (*IF_RX[]    )(void* my_if, void* data_v, int buffer_size);
extern  int     (*IF_TX[]    )(void* my_if, void* data_v, int data_size);
extern  int     (*IF_reopen[])(void* my_if, int reason);

#endif
