#pragma section REPRO

#include "../ccsds_user.h"

int CCSDS_init(void* my_ccsds_v)
{
  (void)my_ccsds_v;
  return 0;
}

int CCSDS_rx(void* my_ccsds_v, void* data_v, int buffer_size)
{
  (void)my_ccsds_v;
  (void)data_v;
  (void)buffer_size;
  return 0;
}

int CCSDS_tx(void* my_ccsds_v, void* data_v, int data_size)
{
  (void)my_ccsds_v;
  (void)data_v;
  (void)data_size;
  return 0;
}

int CCSDS_reopen(void* my_ccsds_v, int reason)
{
  (void)my_ccsds_v;
  (void)reason;
  return 0;
}

CCSDS_ERR_CODE CCSDS_read_sequence(uint32_t select, uint32_t* uip_stat)
{
  (void)select;
  (void)uip_stat;
  return CCSDS_ERR_OK;
}

uint8_t CCSDS_get_buffer_num(void)
{
  return 0;
}

void CCSDS_set_rate(uint32_t ui_rate, CCSDS_Config* config)
{
  (void)ui_rate;
  (void)config;
  return;
}

#pragma section
