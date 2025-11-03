#ifndef MPU_6050_H
#define MPU_6050_H

#include "stm32h7xx_hal.h"
#include "axis.h"

typedef struct mpu6050_data_t
{
  float ax, ay, az;
  float gx, gy, gz;
  float temp;
};

typedef struct mpu6050_handle_t
{
  I2C_HandleTypeDef *i2c;
  uint8_t device_addr;

  //data
  axis3_t accel_raw;
  axis3_t gyro_raw;
  int16_t temp_raw;

  faxis3_t accel_scaled;
  faxis3_t gyro_scaled;
  float temp_scaled;

  //config and state
  float  accel_sensitivity;
  float gyro_sensitivity;
  uint8_t is_initialized;
};


typedef enum {
  MPU6050_OK = 0,
  MPU6050_ERROR,
  MPU6050_I2C_ERROR,
  MPU6050_DEVICE_NOT_FOUND,
  MPU6050_INVALID_WHO_AM_I,
  MPU6050_TIMEOUT,
  MPU6050_INVALID_PARAM
} mpu6050_status_t;

//config accel
typedef enum {
  MPU6050_ACCEL_SCALE_2G = 0,
  MPU6050_ACCEL_SCALE_4G,
  MPU6050_ACCEL_SCALE_8G,
  MPU6050_ACCEL_SCALE_16G
}mpu6050_acc_scale_t;

//config gyro
typedef enum {
  MPU6050_GYRO_SCALE_250DPS = 0,
  MPU6050_GYRO_SCALE_500DPS,
  MPU6050_GYRO_SCALE_1000DPS,
  MPU6050_GYRO_SCALE_2000DPS  
}mpu6050_gyro_scale_t;


mpu6050_status_t mpu6050_init(mpu6050_handle_t *mpu ,I2C_HandleTypeDef *hi2c, uint8_t addr,
                              faxis3_t accel_scale, faxis_t gyro_scale);
mpu6050_status_t mpu6050_read_all (mpu6050_handle_t *mpu);


#endif