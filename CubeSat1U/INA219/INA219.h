#ifndef INA219_H
#define INA219_H

#include "stdio.h"
#include "stm32h7xx_hal.h"

typedef enum {
    Address_gnd_gnd = 1000000,
    Address_gnd_vcc = 1000001,
    Address_gnd_sda = 1000010,
    Address_gnd_scl = 1000011,
    Address_vcc_gnd = 1000100,
    Address_vcc_vcc = 1000101,
    Address_vcc_sda = 1000110,
    Address_vcc_scl = 1000111,
    Address_sda_gnd = 1001000,
    Address_sda_vcc = 1001001,
    Address_sda_sda = 1001010,
    Address_sda_scl = 1001011,
    Address_scl_gnd = 1001100,
    Address_scl_vcc = 1001101,
    Address_scl_sda = 1001110,
    Address_scl_scl = 1001111
} ina_address_A0_A1;

//Registor AddressAddress
const uint8_t ina_reg_config          =      (0x00);
const uint8_t ina_reg_shuntvoltage    =      (0x01);
const uint8_t ina_reg_bus_voltage     =      (0x02);
const uint8_t ina_reg_power           =      (0x03);
const uint8_t ina_reg_current         =      (0x04);
const uint8_t ina_reg_calibration     =      (0x05);

const uint8_t ina_config_reset        =      (0x8000);

typedef enum {
    BRNG_16V = 0x0000,
    BRNG_32V = 0x2000
} ina_config_busvoltagerange;

typedef enum {
    ina_pga_1_40 = 0x0000,
    ina_pga_2_80 = 0x0800,
    ina_pga_4_160 = 0x1000,
    ina_pga_8_320 = 0x1800
} ina_pga_gain_range;

typedef enum {
    ina_config_badc_9bit = 0x0000,
    ina_config_badc_10bit = 0x0080,
    ina_config_badc_11bit = 0x0100,
    ina_config_badc_12bit = 0x0180,
    ina_config_badc_12bit_2s_1060us = 0x0480,
    ina_config_badc_12bit_4s_2130us = 0x0500,
    ina_config_badc_12bit_8s_4260us = 0x0580,
    ina_config_badc_12bit_16s_8150us = 0x0600,
    ina_config_badc_12bit_32s_17020us = 0x0680,
    ina_config_badc_12bit_64s_35050us = 0x0700,
    ina_config_badc_12bit_128s_68100us = 0x07800
} ina_config_badc;                             //Bit 7-1010

typedef enum {
    ina_config_shuntadc_9bit_84us = 0x0000,
    ina_config_shuntadc_10bit_146us = 0x0008,
    ina_config_shuntadc_11bit_276us = 0x0010,
    ina_config_shuntadc_12bit_532us = 0x0018,
    ina_config_shuntadc_12bit_2s_1060us = 0x0048,
    ina_config_shuntadc_12bit_4s_2130us = 0x0050,
    ina_config_shuntadc_12bit_8s_4260us = 0x0058,
    ina_config_shuntadc_12bit_16s_8150us = 0x0060,
    ina_config_shuntadc_12bit_32s_17020us = 0x0068,
    ina_config_shuntadc_12bit_64s_35050us = 0x0070,
    ina_config_shuntadc_12bit_128s_68100us = 0x000780
} ina_config_shuntadc;

typedef enum {
    ina_config_powerdown = 0x00,
    ina_config_shuntvoltage_triggered = 0x01,
    ina_config_busvoltage_triggered = 0x02,
    ina_config_shuntandbus_triggered = 0x03,
    ina_config_adc_off = 0x04,
    ina_config_shuntvoltage_continous = 0x05,
    ina_config_busvoltage_continous = 0x06,
    ina_config_shuntandbus_continous = 0x07
} ina_config_operatingmode;

#define ina_config_mode_mask (0x07)

typedef struct {
    I2C_HandleTypeDef *ina219_i2c;
    uint8_t Address;
} ina219_t;

typedef enum {
    battery_START,
    battery_Ok,
    battery_LOw
} battery_state;

uint16_t ina_ReadBusVoltage(ina219_t *ina219);
uint16_t ina_ReadCurrent(ina219_t *ina219);
uint16_t ina_ReadShuntVoltage(ina219_t *ina219);
uint16_t ina_ReadPower(ina219_t *ina219);


void ina_reset(ina219_t *ina219);
void ina_setCalibration(ina219_t *ina219, uint16_t Calibration);
void ina_getConfig(ina219_t *ina219);
void ina_setConfig(ina219_t *ina219, uint16_t Config);



#endif