#pragma section REPRO
/**
* @file
* @brief I2C通信関連の一般 IO 以外の公開関数
* @note  レジスタの読み書き等，ハードウェアに依存しない関数群を定義する
*/

#include "../i2c.h"
#include <string.h>

#define I2C_MAX_TX_BYTE_COUNT (3)

static I2C_ERR_CODE I2C_write_bytes(void* my_i2c_v, uint8_t cmd_byte, void* data_v, uint8_t buffer_size);
static I2C_ERR_CODE I2C_read_bytes(void* my_i2c_v, uint8_t cmd_byte, void* data_v, uint8_t buffer_size);

I2C_ERR_CODE I2C_write_byte(void* my_i2c_v, uint8_t cmd_byte, void* data_v)
{
  return I2C_write_bytes(my_i2c_v, cmd_byte, data_v, 1);
}

I2C_ERR_CODE I2C_write_2bytes(void* my_i2c_v, uint8_t cmd_byte, void* data_v)
{
  return I2C_write_bytes(my_i2c_v, cmd_byte, data_v, 2);
}

static I2C_ERR_CODE I2C_write_bytes(void* my_i2c_v, uint8_t cmd_byte, void* data_v, uint8_t buffer_size)
{
  uint8_t tx_data[I2C_MAX_TX_BYTE_COUNT];

  tx_data[0] = cmd_byte;
  memcpy(tx_data + sizeof(cmd_byte), data_v, buffer_size);
  I2C_set_stop_flag(my_i2c_v, 1);
  return (I2C_ERR_CODE)I2C_tx(my_i2c_v, tx_data, sizeof(cmd_byte) + buffer_size);
}

I2C_ERR_CODE I2C_read_byte(void* my_i2c_v, uint8_t cmd_byte, void* data_v)
{
  return I2C_read_bytes(my_i2c_v, cmd_byte, data_v, 1);
}

I2C_ERR_CODE I2C_read_2bytes(void* my_i2c_v, uint8_t cmd_byte, void* data_v)
{
  return I2C_read_bytes(my_i2c_v, cmd_byte, data_v, 2);
}

static I2C_ERR_CODE I2C_read_bytes(void* my_i2c_v, uint8_t cmd_byte, void* data_v, uint8_t buffer_size)
{
  I2C_ERR_CODE ret = I2C_OK;

  // read setting
  I2C_set_rx_length(my_i2c_v, buffer_size);
  // send
  I2C_set_stop_flag(my_i2c_v, 0);
  ret = (I2C_ERR_CODE)I2C_tx(my_i2c_v, &cmd_byte, sizeof(cmd_byte));
  if (ret != I2C_OK)
  {
    return ret;
  }
  // read
  I2C_set_stop_flag(my_i2c_v, 1);
  ret = (I2C_ERR_CODE)I2C_rx(my_i2c_v, data_v, buffer_size);
  if (ret > 0)
  {
    ret = I2C_OK;
  }

  return ret;
}

#pragma section
