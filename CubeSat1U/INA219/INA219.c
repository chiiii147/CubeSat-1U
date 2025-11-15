#include "INA219.h"

float Rshunt = 0.1  //Ohm


uint8_t ina_address = Address_gnd_gnd;

float MaxExpertCurrent;
float Current_LSB = MaxExpertCurrent / 32768.0f;
if(Current_LSB * 1e6f < 6.25f) Current_LSB = 6.5f / 1e6f;
float Power_LSB = 20.0f*Current_LSB;
//calculate current_lsb and power_lsb

uint16_t ina_current_mA = (uint16_t)(1.0f /(Current_LSB * 1000.0f) + 0.5f);
uint16_t ina_power_mA = (uint16_t)(1.0f /(Power_LSB * 1000.0f) + 0.5f);

uint16_t calculate_calibration(Rshunt, Current_LSB) {
    float result = 0.04096f / (Current_LSB * Rshunt);
    result = truncf(result);
    return (uint16_t)result;
}
//Calculate calibrationcalibration

uint16_t read_16bit(ina219_t *ina219, uint8_t Register) {
    uint8_t value[2];
    HAL_I2C_Mem_Read(ina219->ina219_i2c, ina_address << 1, Register, 1, Value, 2, 1000);
    //Read 2 byte from Register and add into Value array
    //Stop if error or after waiting 1000ms
    return ((value[0] << 8) | value[1])
    //Shift value[0] by 8 bits and OR it with value[1]
}

HAL_StatusTypeDef write_16bit(ina219_t *ina219, uint8_t Register, uint16_t value){
    uint8_t data[2];
    data[0] = (value << 8) & 0xff;
    data[1] = value & 0xff;
    return HAL_I2C_Mem_Write(ina219->ina219_i2c, ina_address << 1, Register, 1, (uint8_t*)data, 2, 1000);
}

uint16_t ina_ReadBusVoltage(ina219_t *ina219) {
    uint8_t result = read_16bit(ina219, ina_reg_bus_voltage);
    return (result >> 3) * 4;
    //Bus Voltage Register contents must be shifted right by three bits
    //result must be multiplied by the Bus Voltage LSB of 4mV
}

uint16_t ina_ReadCurrent(ina219_t *ina219) {
    uint8_t result_raw = read_16bit(ina219, ina_reg_current);
    return result_raw / ina_current_mA;
    // current 
}

uint16_t ina_ReadShuntVoltage(ina219_t *ina219) {
    uint8_t result_raw = read_16bit(ina219, ina_reg_shuntvoltage);
    return result_raw * 0.01;
    // Shunt Voltage must be multiplied by the Shunt Voltage LSB of 10uV
    // Multiplied 0.01 : uV -> mV
}

uint16_t ina_ReadPower(ina219_t *ina219) {
    uint16_t result_raw = read_16bit(ina219, ina_reg_power);
    return result_raw * ina_power_mA;
}



void ina_reset(ina219_t *ina219) {
    write_16bit(ina219, ina_reg_config, ina_config_reset);
    HAL_Delay(1);
}

void ina_setCalibration(ina219_t *ina219, uint16_t calibration) {
    write_16bit(ina219, ina_reg_config, calibration);
}

void ina_getConfig(ina219_t *ina219) {
    uint16_t result = read_16bit(ina219, ina_reg_config);
    return result;
}

void ina_setConfig(ina219_t *ina219, uint16_t Config) {
    write_16bit(ina219, ina_reg_config, Config);
}

// Example: Set Config
void ina_Config(ina219_t *ina219) {
    uint16_t Config = BRNG_32V | ina_pga_8_320 |
                                 ina_config_badc_12bit 
                                 | ina_config_shuntadc_12bit_532us 
                                 | ina_config_shuntandbus_continous;
    // BRND_32V, ina_pga_8_320, adc 12bit and shunt and bus continous mode is default
    // modify the realreal value
    uint16_t calibration = calculate_calibration();
    ina_setCalibration(ina219, calibration);
    ina_setConfig(ina219, Config);
}

void set_PowerMode(ina219_t *ina219, ina_config_operatingmode mode) {
    uint16_t config = getConfig(ina219);
    // get now config
    switch(mode) {
        case ina_config_powerdown:
        config = (config & ~ina_config_mode_mask) | (ina_config_powerdown & ina_config_mode_mask);
        ina_setConfig(ina219, config);
        break;

        case ina_config_shuntvoltage_triggered:
        config = (config & ~ina_config_mode_mask) | (ina_config_shuntvoltage_triggered & ina_config_mode_mask);
        ina_setConfig(ina219, config);
        break;

        case ina_config_busvoltage_triggered:
        config = (config & ~ina_config_mode_mask) | (ina_config_busvoltage_triggered & ina_config_mode_mask);
        ina_setConfig(ina219, config);
        break;

        case ina_config_shuntandbus_triggered:
        config = (config & ~ina_config_mode_mask) | (ina_config_shuntandbus_triggered & ina_config_mode_mask);
        ina_setConfig(ina219, config);
        break;

        case ina_config_adc_off:
        config = (config & ~ina_config_mode_mask) | (ina_config_adc_off & ina_config_mode_mask);
        ina_setConfig(ina219, config);
        break;

        case ina_config_shuntvoltage_continous:
        config = (config & ~ina_config_mode_mask) | (ina_config_shuntvoltage_continous & ina_config_mode_mask);
        ina_setConfig(ina219, config);
        break;

        case ina_config_busvoltage_continous:
        config = (config & ~ina_config_mode_mask) | (ina_config_busvoltage_continous & ina_config_mode_mask);
        ina_setConfig(ina219, config);
        break;

        case ina_config_shuntandbus_continous:
        config = (config & ~ina_config_mode_mask) | (ina_config_shuntandbus_continous & ina_config_mode_mask);
        ina_setConfig(ina219, config);
        break;

        default:
        break;
    }

    
}