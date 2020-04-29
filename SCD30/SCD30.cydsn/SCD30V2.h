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
#ifndef __SENSIRION_SCD30_H_V2__
#define __SENSIRION_SCD30_H_V2__

#include "project.h"

#define SCD30_I2C_1_ADDRESS                       0x61u

#define SCD30_READ_HEADER                       0xC3u
#define SCD30_WRITE_HEADER                      0xC2u
#define SCD30_CONTINUOUS_MEASUREMENT            0x0010u
#define SCD30_SET_MEASUREMENT_INTERVAL          0x4600u
#define SCD30_GET_DATA_READY                    0x0202u
#define SCD30_READ_MEASUREMENT                  0x0300u
#define SCD30_STOP_MEASUREMENT                  0x0104u
#define SCD30_AUTOMATIC_SELF_CALIBRATION        0x5306u
#define SCD30_SET_FORCED_RECALIBRATION_FACTOR   0x5204u
#define SCD30_SET_TEMPERATURE_OFFSET            0x5403u
#define SCD30_SET_ALTITUDE_COMPENSATION         0x5102u
#define SCD30_READ_SERIALNBR                    0xD033u

#define SCD30_SET_TEMP_OFFSET                   0x5403u


#define SCD30_POLYNOMIAL                        0x31u // P(x) = x^8 + x^5 + x^4 + 1 = 100110001


    int SCD30(void);

    void SCD30_initialize(void);

    _Bool SCD30_isAvailable(void);

    void SCD30_setAutoSelfCalibration(_Bool enable);
    void SCD30_setMeasurementInterval(uint16_t interval);

    void SCD30_startPeriodicMeasurment(void);
    void SCD30_stopMeasurement(void);
    void SCD30_setTemperatureOffset(uint16_t offset);

    void SCD30_getCarbonDioxideConcentration(float* result);


    uint8_t SCD30_calculateCrc(uint8_t* data, uint8_t len);

    void SCD30_writeCommand(uint16_t command);
    void SCD30_writeCommandWithArguments(uint16_t command, uint16_t arguments);
    uint16_t SCD30_readRegister(uint16_t address);

    void SCD30_writeBuffer(uint8_t* data, uint8_t len);
    void SCD30_readBuffer(uint8_t* data, uint8_t len);



#endif
/* [] END OF FILE */
