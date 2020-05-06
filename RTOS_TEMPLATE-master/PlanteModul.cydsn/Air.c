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
#include <stdio.h>
#include <stdlib.h>


struct AirCondition {
    uint16 iAirTemp;
    uint16 iRH;
    uint16 iCO2;
};

struct TestData{
    char identifier;
    uint16 message;
};

const int iSizeOfAir = 40; // Should be the same as the value of the array below
struct AirCondition currentAir[40]; 
int iAirTIndex = 0; // Use this index to save AirT measurements into the struct array
int iRHIndex = 0; // Use this index to save RH measurements into the struct array
int iCO2Index = 0; // Use this index to save CO2 measurements into the struct array


QueueHandle_t xQueueAirConditions;
QueueHandle_t xQueueAirTestValues;




    void vAirInit()
    {
        xTaskCreate(vTaskAirController, "Air controller", 1000, NULL, 3, NULL);

        xQueueAirConditions = xQueueCreate(5, sizeof(struct AirCondition));
        xQueueAirTestValues = xQueueCreate(5, sizeof(struct TestData));
        
        #if AIRTEST ==1
            vTestTaskAirInit();
        #endif
    }
    
    
    
    void vTaskAirController()
    {
        
    }
        








                 ////// Test taks //////

    void vTestTaskAirInit()
    {
        xTaskCreate(vTestTaskAirController, "Test task Air controller", 100, NULL, 3, NULL);
    }
    
    void vTestTaskAirController()
    {
        
    }






/* [] END OF FILE */
