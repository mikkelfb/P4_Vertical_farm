/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "SCD30.h"


     uint8_t devAddr = SCD30_I2C_ADDRESS;


void SCD30_initialize(void) {
    //Set temperature offset。
    //setTemperatureOffset(0);

    SCD30_setMeasurementInterval(2); // 2 seconds between measurements
    SCD30_startPeriodicMeasurment(); // start periodic measuments

    //setAutoSelfCalibration(true); // Enable auto-self-calibration
}


void SCD30_setTemperatureOffset(uint16_t offset) {
    SCD30_writeCommandWithArguments(SCD30_SET_TEMP_OFFSET, offset);
}

_Bool SCD30_isAvailable(void) {
    return SCD30_readRegister(SCD30_GET_DATA_READY);
}

void SCD30_setAutoSelfCalibration(_Bool enable) {
    if (enable) {
        SCD30_writeCommandWithArguments(SCD30_AUTOMATIC_SELF_CALIBRATION, 1);    //Activate continuous ASC
    } else {
        SCD30_writeCommandWithArguments(SCD30_AUTOMATIC_SELF_CALIBRATION, 0);    //Deactivate continuous ASC
    }
}

void SCD30_setMeasurementInterval(uint16_t interval) {
    SCD30_writeCommandWithArguments(SCD30_SET_MEASUREMENT_INTERVAL, interval);
}

void SCD30_startPeriodicMeasurment(void) {
    SCD30_writeCommandWithArguments(SCD30_CONTINUOUS_MEASUREMENT, 0x0000);
}

void SCD30_stopMeasurement(void) {
   SCD30_writeCommand(SCD30_STOP_MEASUREMENT);
}

void SCD30_getCarbonDioxideConcentration(float* result) {
    uint8_t buf[18] = { 0 };
    uint32_t co2U32 = 0;
    uint32_t tempU32 = 0;
    uint32_t humU32 = 0;
    float co2Concentration = 0;
    float temperature = 0;
    float humidity = 0;

    SCD30_writeCommand(SCD30_READ_MEASUREMENT);
    SCD30_readBuffer(buf, 18);

    co2U32 = (uint32_t)((((uint32_t)buf[0]) << 24) | (((uint32_t)buf[1]) << 16) |
                        (((uint32_t)buf[3]) << 8) | ((uint32_t)buf[4]));

    tempU32 = (uint32_t)((((uint32_t)buf[6]) << 24) | (((uint32_t)buf[7]) << 16) |
                         (((uint32_t)buf[9]) << 8) | ((uint32_t)buf[10]));

    humU32 = (uint32_t)((((uint32_t)buf[12]) << 24) | (((uint32_t)buf[13]) << 16) |
                        (((uint32_t)buf[15]) << 8) | ((uint32_t)buf[16]));

    memcpy(&result[0], &co2U32, sizeof(co2Concentration));
    memcpy(&result[1], &tempU32, sizeof(temperature));
    memcpy(&result[2], &humU32, sizeof(humidity));

}

void SCD30_writeCommand(uint16_t command) {
   I2C_MasterSendStart(SCD30_I2C_ADDRESS, 0);
   I2C_MasterWriteByte(command >> 8); // MSB
   I2C_MasterWriteByte(command & 0xff); // LSB
   I2C_MasterSendStop();
                  UART_PutHexInt(command);             UART_PutString("Command Written");       UART_PutString(" \n ");
}

void SCD30_writeCommandWithArguments(uint16_t command, uint16_t arguments) {
    uint8_t checkSum, buf[5] = { 0 };

    buf[0] = command >> 8;
    buf[1] = command & 0xff;
    buf[2] = arguments >> 8;
    buf[3] = arguments & 0xff;
    checkSum = SCD30_calculateCrc(&buf[2], 2);
    buf[4] = checkSum;

    SCD30_writeBuffer(buf, 5);
}

uint16_t SCD30_readRegister(uint16_t address) {
    uint8_t buf[2] = { 0 };

    SCD30_writeCommand(address);
    SCD30_readBuffer(buf, 2);

    return ((((uint16_t)buf[0]) << 8) | buf[1]);
}

void SCD30_writeBuffer(uint8_t* data, uint8_t len) {
    I2C_MasterSendStart(SCD30_I2C_ADDRESS, 0);
    for(int i = 0; i<= len; i++){
        I2C_MasterWriteByte(data[i]);
    }
    I2C_MasterSendStop();
                            for(int k = 0; k <=len; k++){
                            UART_PutHexByte(data);
                            }
}

void SCD30_readBuffer(uint8_t* data, uint8_t len) {
    uint8_t i = 0;
   // I2C_MasterClearReadBuf();
   // I2C_MasterSendStart(SCD30_I2C_ADDRESS, 1);
   // while (i < len) {
 //       data[i ++] = I2C_MasterReadByte(I2C_ACK_DATA);
//    }
    I2C_MasterReadBuf();
    UART_PutString("Data Read");
}

uint8_t SCD30_calculateCrc(uint8_t data[], uint8_t len) {
    uint8_t bit, byteCtr, crc = 0xff;

    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < len; byteCtr ++) {
        crc ^= (data[byteCtr]);

        for (bit = 8; bit > 0; -- bit) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ SCD30_POLYNOMIAL;
            } else {
                crc = (crc << 1);
            }
        }
    }

    return crc;
}





/* [] END OF FILE */
