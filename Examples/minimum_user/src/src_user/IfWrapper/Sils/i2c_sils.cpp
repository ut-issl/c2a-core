#pragma section REPRO
#include <src_core/IfWrapper/i2c.h>

int I2C_init(void* my_i2c_v)
{
  (void)my_i2c_v;
  return 0;
}

int I2C_rx(void* my_i2c_v, void* data_v, int buffer_size)
{
  (void*)my_i2c_v;
  (void*)data_v;
  (void)buffer_size;
  return 0;
}

int I2C_tx(void* my_i2c_v, void* data_v, int data_size)
{
  (void*)my_i2c_v;
  (void*)data_v;
  (void)data_size;
  return 0;
}

int I2C_reopen(void* my_i2c_v, int reason)
{
  (void*)my_i2c_v;
  (void)reason;
  return 0;
}

void I2C_set_stop_flag(void* my_i2c_v, const unsigned char stop_flag)
{
  (void*)my_i2c_v;
  (void)stop_flag;
  return;
}

void I2C_set_rx_length(void* my_i2c_v, const uint32_t rx_length)
{
  (void*)my_i2c_v;
  (void)rx_length;
  return;
}

#pragma section
