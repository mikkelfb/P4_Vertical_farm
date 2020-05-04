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

/* [] END OF FILE */

#include "FreeRTOS.h"
#include "Nutrients.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

struct Nutrients{ // Struct contains nutrients values measured
    uint16  iPHvalue;
    uint16  iECvalue;
    uint16  iWaterTemp;
};

const int iSizeOfNutrients = 40; // Should be the same as the value of the array below
struct Nutrients currentNutrients[40]; // Create struct array with space for 40 measurements
int iPHIndex = 0; // Use this index to save pH measurements into the struct array
int iECIndex = 0; // Use this index to save EC measurements into the struct array
int iWateTempIndex = 0; // Use this index to save water temperature measurements into the struct array

const float fNeutralVoltage = 1500; //Voltage at pH 7, should be calibrated
const float fAcidVoltage = 2032.44; //Voltage at pH 4, should be calibrated

QueueHandle_t xQueueNutrientPump[3]; // Create a queue for 3 nutrient pumps
QueueHandle_t xQueuePHValue;        // Create a queue for sending pH values through UART
QueueHandle_t xQueueWaterTemp;
QueueHandle_t xQueueECValue;        //Create a queue for sending EC values to the test task.


const uint8 MAX_SPEED = 6; // Max speed constant for the nutrient pumps
const uint8 STOP_SPEED = 19; // Stop speed for the nutrient pumps
static const uint8 pcTextForNutrientPump[3] = {0, 1, 2};


void vNutrientsInit() {
    /*  The queue is created to hold a maximum of 1 value, each of which is
        large enough to hold a variable at the size of Bool. */
    xQueueNutrientPump[0] = xQueueCreate( 1, sizeof( _Bool ) );
    xQueueNutrientPump[1] = xQueueCreate( 1, sizeof( _Bool ) );
    xQueueNutrientPump[2] = xQueueCreate( 1, sizeof( _Bool ) );
    
    xQueuePHValue = xQueueCreate(1 , sizeof(uint16));
    xQueueWaterTemp = xQueueCreate(1 , sizeof(uint16));
    xQueueECValue = xQueueCreate(1, sizeof(uint16));
    
    
    /*  Create the task that will control one nutrient pump. The task is created with
        priority 1. */
    xTaskCreate(vTaskNutrientPump, "Pump 1", 100, (void*)pcTextForNutrientPump, 2, NULL);
    xTaskCreate(vTaskNutrientPump, "Pump 2", 100, (void*)pcTextForNutrientPump + 1, 2, NULL);
    xTaskCreate(vTaskNutrientPump, "Pump 2", 100, (void*)pcTextForNutrientPump + 2, 2, NULL);

    
    xTaskCreate(vTaskMeasurePH, "PH", 1000 , NULL , 2 , NULL);
    xTaskCreate(vTaskWaterTemp, "VandTemp", 1000, NULL, 4 , NULL);
    
    xTaskCreate(vTaskMeasureEC, "Get EC value", 100, NULL, 2, NULL);
    
    /*Initialize test tasks*/
    #if NUTRIENTSTEST == 1
        vTestTaskInit();
    #endif
    
    
}


void vTaskNutrientPump( void *pvParameters ) {
    _Bool bState;
    BaseType_t xStatus;
    uint8 *piNutrientPump;
    
    piNutrientPump = ( uint8 * ) pvParameters;
    
    for(;;) {
        /* Switch case for controlling the nutrient pumps */
        switch (*piNutrientPump) {
            case 0:
                xStatus = xQueueReceive( xQueueNutrientPump[0], &bState, portMAX_DELAY ); // Checks if there is a new message in the queue.
        
                if (xStatus == pdPASS) {
                    if (bState) {
                        PWM_PERISTALTISK_1_WriteCompare1(MAX_SPEED); // Start running the pump.
                    } else {
                        PWM_PERISTALTISK_1_WriteCompare1(STOP_SPEED); // Stop running the pump.
                    }
                }
                break;
            
            case 1:
                xStatus = xQueueReceive( xQueueNutrientPump[1], &bState, portMAX_DELAY ); // Checks if there is a new message in the queue.
        
                if (xStatus == pdPASS) {
                    if (bState) {
                        PWM_PERISTALTISK_1_WriteCompare2(MAX_SPEED);
                    } else {
                        PWM_PERISTALTISK_1_WriteCompare2(STOP_SPEED);
                    }
                }
                break;
                
            case 2:
                xStatus = xQueueReceive( xQueueNutrientPump[2], &bState, portMAX_DELAY ); // Checks if there is a new message in the queue.
        
                if (xStatus == pdPASS) {
                    if (bState) {
                        PWM_PERISTALTISK_2_WriteCompare(MAX_SPEED);
                    } else {
                        PWM_PERISTALTISK_2_WriteCompare(STOP_SPEED);
                    }
                }
                break;
        }
    }
}

void vTaskMeasurePH(){
    float fPHVoltage;
    const TickType_t xDelayms = pdMS_TO_TICKS( 100 ); // Sets the measurement resolution.
    float fPHValue;
    float fMilliPHValue;
    uint16 iMilliPHValue;
    _Bool bState = 0;
    
    for(;;){
        if(bState == 0){
            ADC_PH_StartConvert();
            bState = 1;
        }
        else if( (ADC_PH_IsEndConversion(ADC_PH_RETURN_STATUS) != 0) && (bState == 1) ){
            fPHVoltage = ((ADC_PH_GetResult16()) / 4096.0) * 3000.0; // Calculate voltage from measured value
            fPHValue = fCalculatePHValue(fPHVoltage); // Convert the voltage to a pH value
            fMilliPHValue = fPHValue * 1000; // Convert to milli pH value. This makes sure that we can see float values. 
            iMilliPHValue = (uint16) fMilliPHValue; // Convert to a sendable message for UART (from float to int).
            currentNutrients[iPHIndex].iPHvalue = iMilliPHValue; // Save in the currentNutrients array on the respective index.
            iPHIndex++;
            if(iPHIndex == iSizeOfNutrients) {
                iPHIndex = 0;
            }
            bState = 0;
          //  xQueueSendToBack(xQueuePHValue , &iMilliPHValue , portMAX_DELAY); // Send the value to the back of the queue. Used for testing only.
            //xQueueSendToBack(xQueuePHValue , &iMilliPHValue , portMAX_DELAY); // Send the value to the back of the queue. Used for testing only.
            vTaskDelay(xDelayms);
        }
    }
}


float fCalculatePHValue(float fPHVoltage){
    float fSlope = (7.0-4.0) / ( (fNeutralVoltage - 1500.0)/3.0 - (fAcidVoltage - 1500.0)/3.0); //calculate a in y = ax+b
    float fIntercept = 7.0 - fSlope*(fNeutralVoltage-1500.0)/3.0; //calculate b in y = ax + b
    return fSlope*(fPHVoltage-1500.0)/3.0 + fIntercept; //return pH value
}



void vTaskWaterTemp(){
    const TickType_t xDelayms = pdMS_TO_TICKS( 2000 ); // Sets the measurement resolution.
    int16 waterTemp; 
    _Bool flagTimer = 1;
    for(;;) 
    { 
        if (flagTimer == 1){
            flagTimer = 0;
            DS18x8_SendTemperatureRequest(); //  Sending request to sensor, checking bus, and setting DataReady == 1 
        }
        if (DS18x8_DataReady) // DS18 completed temperature measurement - begin read data
        {   
            DS18x8_ReadTemperature(); //  Reads temperature from DS scratchpad and stores in DS18x8_Sensor[i]
            waterTemp = DS18x8_GetTemperatureAsInt100 (0);  // Converts reading to int16 ex: 38.06 --> 3806 , "index" depicts senosr
            xQueueSendToBack(xQueueWaterTemp, &waterTemp, portMAX_DELAY); // Send the value to the back of the queue. Used for testing only.
            flagTimer = 1;
            vTaskDelay(xDelayms);
        }    
    }   
}
/*This function writes 'R' to the EC sensor, then delays(300) before requesting data transfer from the EC sensor. 
This is in accordance with instructions given on the EC sensor datasheet. */
void vTaskMeasureEC()
{
    float fmicroECValue;
    //float fnanoECValue;
    uint16 imicroECValue;
    const TickType_t xDelaymsBeforeRead = pdMS_TO_TICKS( 300 ); // Sets the measurement resolution.
    const TickType_t xDelaymsTimerEvent = pdMS_TO_TICKS( 100 ); // Sets the measurement resolution.    
    for (;;){

        /* Defines the message to be sent, as an array of characters */
        unsigned char  ucMessage[]= "R";
     
        /* Initialize buffer with packet  , slave address is 9*/
        (void) I2C_MasterWriteBuf(0x009 , ucMessage, 1 , I2C_MODE_COMPLETE_XFER);

        //Waits until master completes write transfer 
        while (0u == (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT))
        {
        } 

        
        vTaskDelay(xDelaymsBeforeRead);
        int iResponselen = 10;
        char cResponse[iResponselen];
        (void) I2C_MasterReadBuf(0x009, (uint8 *) cResponse, iResponselen, I2C_MODE_COMPLETE_XFER);
        
        //Waits until master completes write transfer     
        while (0u == (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT))
        {
        }
            
        if (cResponse[0] == '1'){
            
            cResponse[0] = 0;                   //removes the initial '1' from the string, before converting to float.
            for(int i=0; i<iResponselen; i++){
                cResponse[i]=cResponse[i+1];
            }
            
            fmicroECValue = strtof(cResponse, NULL); //converts to float.
                                                     //fnanoECValue = fmicroECValue * 1000; // Convert to ns/cm (nano siemen per centimeter) value. This makes sure that we can see float values. 
            imicroECValue = (uint16) fmicroECValue; // Convert to a sendable message for UART (from float to int).
            currentNutrients[iECIndex].iECvalue = imicroECValue; // Save in the currentNutrients array on the respective index.
            iECIndex++;
            if(iECIndex == iSizeOfNutrients) {
                iECIndex = 0;
            }        
        }


        xQueueSendToBack(xQueueECValue, &imicroECValue, portMAX_DELAY);
        vTaskDelay(xDelaymsTimerEvent);
    }
}

/* --- TEST TASK --- */

/*
    Task for testing existing implementations.

    DO NOT delete this functions/tasks, but comment them out if they aren't nesessary 
    for your current implementation/testing
    
    All test task/functions, must start with (type)Test(Task/Function/Var)(Name)

    
*/

/*void vTestTaskWaterTemp(){
    uint16 WaterTempTest;
    for(;;){
        xQueueReceive( xQueueWaterTemp, &WaterTempTest, portMAX_DELAY);
        SW_UART_TEST_USB_PutString("Watertemp: ");
        SW_UART_TEST_USB_PutHexInt(WaterTempTest);
        SW_UART_TEST_USB_PutString("\n");
     }
}


void vTestTaskInit(){
    xTaskCreate(vTestTaskNutrientPump, "Test Pump 1", 100, NULL, 1, NULL); 
   // xTaskCreate(vTestTaskUARTDataTransmit, "Test PH print", 100, NULL, 2, NULL); 
    xTaskCreate(vTestTaskWaterTemp, "Test Water Temp", 100, NULL, 3, NULL);
    xTaskCreate(vTestTaskMeasureEC, "Test EC print", 100, NULL, 2, NULL); 
}


void vTestTaskUARTDataTransmit(){
    uint16 iPHValueForPrint;
    for(;;){
        xQueueReceive( xQueuePHValue, &iPHValueForPrint, portMAX_DELAY );
        SW_UART_TEST_USB_PutString("i: ");
        SW_UART_TEST_USB_PutHexInt(iPHIndex);
        SW_UART_TEST_USB_PutString(" 0x");
        SW_UART_TEST_USB_PutHexInt(currentNutrients[iPHIndex].iPHvalue);
        SW_UART_TEST_USB_PutString("\n");
    }
}



void vTestTaskNutrientPump() {
    _Bool bTestState1 = 1;
    _Bool bTestState2 = 0;
    const TickType_t xDelayms = pdMS_TO_TICKS( 200 );
    
    for(;;) {
        if(bTestState1 == 1) {
            bTestState1 = 0;
            bTestState2 = 1;
        }
        else {
            bTestState1 = 1;
            bTestState2 = 0;
        }
        xQueueSendToBack(xQueueNutrientPump[0], &bTestState1, portMAX_DELAY);
        xQueueSendToBack(xQueueNutrientPump[1], &bTestState2, portMAX_DELAY);
        xQueueSendToBack(xQueueNutrientPump[2], &bTestState1, portMAX_DELAY);
        vTaskDelay(xDelayms);
    }
}

void vTestTaskMeasureEC (){
    uint16 testValueEC;
    for(;;){
        xQueueReceive(xQueueECValue, &testValueEC, portMAX_DELAY);
        SW_UART_TEST_USB_PutString("ECValue");
        SW_UART_TEST_USB_PutHexInt(testValueEC);
        SW_UART_TEST_USB_PutString("\n ");
    }
}
*/