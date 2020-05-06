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


#include "FreeRTOS.h"
#include "Air.h"
#include "task.h"
#include "queue.h"
#include "SCD30_v2.h"

float SCDResults [3] = {0};

QueueHandle_t xQueueSCD[3]; 


void vAirInit(){
    vInitSCD30(2); // Starting SCD30 sensor and setting meas interval to every 2nd sec
    xTaskCreate(vTaskGetMeasSCD30, "SCD30", 1000 , NULL , 2 , NULL);
    xTaskCreate(vTaskTestI2C , "test" , 100 , NULL , 3 , NULL);
    xTaskCreate(vTaskInitializeTask, "show", 100, NULL, 4, NULL);
   
    xQueueSCD[0] = xQueueCreate(1, sizeof(float));
    xQueueSCD[1] = xQueueCreate(1, sizeof(float));
    xQueueSCD[2] = xQueueCreate(1, sizeof(float));
    
    #if AIRTEST == 1
        vAirTestTaskInit();
    #endif
    
}


void vInitSCD30(uint16_t interval){
    SCD30_setMeasurementInterval(interval); // takes the input and calibrates the sensor with the req. time interval
    SCD30_startPeriodicMeasurment(); // read it and weep
}

void vTaskGetMeasSCD30(){
    const TickType_t xDelayms = pdMS_TO_TICKS( 2000 ); // Sets the measurement resolution.
    for(;;){
        if (SCD30_isAvailable())
        {
            SCD30_getCarbonDioxideConcentration(SCDResults); // Reading the data and storing them in SCD30Results 
            xQueueSendToBack(xQueueSCD[0], &SCDResults[0], portMAX_DELAY);
            xQueueSendToBack(xQueueSCD[1], &SCDResults[1], portMAX_DELAY);
            xQueueSendToBack(xQueueSCD[2], &SCDResults[2], portMAX_DELAY);
            vTaskDelay(xDelayms);
        }
    }
}

void vTaskInitializeTask()
{
    for(;;)
    {
        vInitSCD30(2);
        vTaskDelay(portMAX_DELAY);
    }

}

void vTaskTestI2C(){
    
    const TickType_t xDelaymsBeforeRead = pdMS_TO_TICKS( 300 );
    for(;;){
        unsigned char  ucMessage[]= "R";
     
        /* Initialize buffer with packet  , slave address is 9*/
        (void) I2C_1_MasterWriteBuf(0x61 , ucMessage, 1 , I2C_1_MODE_COMPLETE_XFER);

 

        //Waits until master completes write transfer 
        while (0u == (I2C_1_MasterStatus() & I2C_1_MSTAT_WR_CMPLT))
        {
        } 
        vTaskDelay(xDelaymsBeforeRead);
    }
}

// ----- Test tasks ----- //

void vAirTestTaskInit(){
xTaskCreate(vTaskTestSCD30, "SCD30Test", 1000, NULL, 3, NULL);
}

void vTaskTestSCD30(){
    static float testResults[3];
    for(;;){   
        xQueueReceive(xQueueSCD[0],&testResults[0], portMAX_DELAY);
        xQueueReceive(xQueueSCD[1],&testResults[1], portMAX_DELAY);
        xQueueReceive(xQueueSCD[2],&testResults[2], portMAX_DELAY);

        SW_UART_TEST_USB_PutString("CO2: ");
        SW_UART_TEST_USB_PutHexInt(testResults[0]);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutString("TEMP: ");
        SW_UART_TEST_USB_PutHexInt(testResults[1]);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutString("RH: ");
        SW_UART_TEST_USB_PutHexInt(testResults[2]);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutString("\n");
    }
}
/* [] END OF FILE */