#pragma section REPRO
/**
 * @file
 * @brief I2C 通信関連の一般 IO 以外の公開関数
 * @note  レジスタの読み書き等，ハードウェアに依存しない関数群を定義する
 */

#include "i2c_common.h"
#include "../../Library/endian.h"
#include <string.h>

static DS_ERR_CODE I2C_write_bytes_(DriverSuper* p_super, uint8_t stream, void* my_i2c_v,
                                    uint8_t register_address, void* data_v, uint8_t buffer_size);
static DS_ERR_CODE I2C_read_bytes_(DriverSuper* p_super, uint8_t stream, void* my_i2c_v,
                                   uint8_t register_address, void* data_v, uint8_t buffer_size);

DS_ERR_CODE I2C_write_byte(DriverSuper* p_super, uint8_t stream, void* my_i2c_v, uint8_t register_address, uint8_t data)
{
  return I2C_write_bytes_(p_super, stream, my_i2c_v, register_address, &data, sizeof(data));
}

DS_ERR_CODE I2C_write_2bytes(DriverSuper* p_super, uint8_t stream, void* my_i2c_v, uint8_t register_address, uint16_t data)
{
  return I2C_write_bytes_(p_super, stream, my_i2c_v, register_address, &data, sizeof(data));
}

static DS_ERR_CODE I2C_write_bytes_(DriverSuper* p_super, uint8_t stream, void* my_i2c_v,
                                    uint8_t register_address, void* data_v, uint8_t buffer_size)
{
  uint8_t tx_data[sizeof(register_address) + sizeof(uint16_t)];
  DS_StreamConfig* stream_config = &(p_super->stream_config[stream]);

  tx_data[0] = register_address;
  ENDIAN_memcpy(tx_data + sizeof(register_address), data_v, buffer_size);
  I2C_set_stop_flag(my_i2c_v, 1);
  DSSC_set_tx_frame(stream_config, tx_data);
  DSSC_set_tx_frame_size(stream_config, sizeof(register_address) + buffer_size);

  return DS_send_general_cmd(p_super, stream);
}

DS_ERR_CODE I2C_read_byte(DriverSuper* p_super, uint8_t stream, void* my_i2c_v,
                          uint8_t register_address, uint8_t* data)
{
  return I2C_read_bytes_(p_super, stream, my_i2c_v, register_address, data, sizeof(uint8_t));
}

DS_ERR_CODE I2C_read_2bytes(DriverSuper* p_super, uint8_t stream, void* my_i2c_v,
                            uint8_t register_address, uint16_t* data)
{
  return I2C_read_bytes_(p_super, stream, my_i2c_v, register_address, data, sizeof(uint8_t));
}

static DS_ERR_CODE I2C_read_bytes_(DriverSuper* p_super, uint8_t stream, void* my_i2c_v,
                                   uint8_t register_address, void* data_v, uint8_t buffer_size)
{
  DS_ERR_CODE ret = DS_ERR_CODE_OK;
  const uint8_t* rx_data;
  DS_StreamConfig* stream_config = &(p_super->stream_config[stream]);

  // read setting
  DSSC_set_rx_frame_size(stream_config, buffer_size);
  I2C_set_rx_length(my_i2c_v, buffer_size);
  DS_clear_rx_buffer(p_super);
  // send
  I2C_set_stop_flag(my_i2c_v, 0);
  DSSC_set_tx_frame(stream_config, &register_address);
  DSSC_set_tx_frame_size(stream_config, sizeof(register_address));
  ret = DS_send_req_tlm_cmd(p_super, stream);
  if (ret != DS_ERR_CODE_OK) return ret;
  // read
  I2C_set_stop_flag(my_i2c_v, 1);
  ret = DS_receive(p_super);
  if (ret != DS_ERR_CODE_OK) return ret;
  if (DSSC_get_rec_status(stream_config)->status_code != DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    return DS_ERR_CODE_ERR;
  }

  rx_data = DSSC_get_rx_frame(stream_config);
  ENDIAN_memcpy(data_v, rx_data, buffer_size);

  return ret;
}

#pragma section
