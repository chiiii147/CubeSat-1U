#include "mpu6050.h"

//private definitions
#define mpu6050_addr        0x68
#define who_am_i_reg        0x75
#define pwr_mgmt_1_reg      0x6B
#define smplrt_div          0x19
#define gyro_config_reg     0x1B
#define accel_config_reg    0x1C
#define accel_x_out_reg     0x3B

#define accel_config_bit_pos 3
#define gyro_config_bit_pos 3

#define I2C_trial 3
#define I2C_timeout 100 //ms

static mpu6050_status_t mpu6050_read_reg(mpu6050_handle_t *mpu, uint8_t reg, uint8_t *data);
static mpu6050_status_t mpu6050_write_reg(mpu6050_handle_t *mpu, uint8_t reg, uint8_t data);

mpu6050_status_t mpu6050_init(mpu6050_handle_t *mpu, I2C_HandleTypeDef *hi2c, uint8_t addr,
                              faxis3_t accel_scale, faxis3_t gyro_scale)
{
  if (mpu == NULL || hi2c == NULL)
  {
    return MPU6050_INVALID_PARAM;
  }

  mpu->i2c = hi2c;
  mpu->device_addr = addr;
  mpu->initialized = 0;

  //check i2c connection
  if (HAL_I2C_IsDeviceReady(hi2c, mpu->device_adrr << 1, I2C_trial, I2C_timeout) != HAL_OK)
  {
    return MPU6050_DEVICE_NOT_FOUND;
  }
  //check who_am_i
  uint8_t who_am_i;
  if (mpu6050_read_reg(mpu, who_am_i_reg, &who_am_i) != MPU6050_OK)
  {
    return MPU6050_I2C_ERROR;
  }

  if (who_am_i != mpu6050_addr)
  {
    return MPU6050_INVALID_WHO_AM_I;
  }
  
  //wake up mpu6050 (reset sleep bit)
  //write all 0 to wake up
  if (mpu6050_write_reg(mpu, pwr_mgmt_1_reg, 0x00) != MPU6050_OK)
  {
    return MPU6050_I2C_ERROR;
  }
  HAL_Delay(10);

  //config gyro
  if (mpu6050_write_reg(mpu, gyro_config_reg, (uint8_t)gyro_scale << gyro_config_bit_pos) != MPU6050_OK)
  {
    return MPU6050_I2C_ERROR;
  }
  //config accel
  if (mpu6050_write_reg(mpu, accel_config_reg, (uint8_t)accel_scale << accel_config_bit_pos) != MPU6050_OK)
  {
    return MPU6050_I2C_ERROR;
  }
  
  
  
  
}

static mpu6050_status_t mpu6050_read_reg(mpu6050_handle_t *mpu, uint8_t reg, uint8_t *data)
{
  if (HAL_I2C_Mem_Read(mpu->i2c, mpu->device_addr << 1, reg, 1, data, 1, I2C_timeout) != HAL_OK)
  {
    return MPU6050_I2C_ERROR;
  }
  return MPU6050_OK;
  
}

static mpu6050_status_t mpu6050_write_reg(mpu6050_handle_t *mpu, uint8_t reg, uint8_t *data)
{
  if (HAL_I2C_Mem_Write(mpu->i2c, mpu->device_addr << 1, reg, 1, data, 1, I2C_timeout) != HAL_OK)
  {
    return MPU6050_I2C_ERROR;
  }
  return MPU6050_OK;
  
}