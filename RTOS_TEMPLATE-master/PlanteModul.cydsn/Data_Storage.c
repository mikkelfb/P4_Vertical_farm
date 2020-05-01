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
#include "Nutrients.h"
#include "Nutrients.c"
#include "Data_Storage.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

struct allData{ // Struct contains nutrients values measured
    uint16  iPHvalue;
    uint16  iECvalue;
    uint16  iWaterTemp;
    uint16  iWaterFlow;
    uint16  iC02;
    uint16  iTempAir;
    uint16  iRH;
    uint16  iLightVal;
};
struct dataMessage{
    char identifier;
    uint16 message;
};
dataMessage recivedmes
xQueueRecive(controllerdata, &recivedmes, portMAX_DELAY)
switch recivedmes.identifier
    case'p':
        alldata.iPhvalue = recivedmes.message


datamessage sendmessage
sendmessage.message = phvalue 
sendmessage.identifier = 'p'
xQueueSendToBack(sendmessage, &controllerdata, portMAX_DELAY)



int testing = 1;
QueueHandle_t xQueueControllerData;
QueueHandle_t xQueueCentralrequest;
QueueHandle_t xQueueCentralData;


void vInitDataStorage()
{
    xQueueControllerData = xQueueCreate(10, sizeof(struct allData));
    xQueueCentralData = xQueueCreate(10, sizeof(struct allData));
    xQueueCentralrequest = xQueueCreate(1 , sizeof(uint16));
    
    xTaskCreate(vTaskDataStorage, "Data Storage", 1000, NULL , 2 , NULL);
}


_Bool vDataQueueing()
{
    
    struct allData dataForQueue;
    if(testing == 0)
    {
        dataForQueue.iWaterTemp = currentNutrients[0].iWaterTemp;
        dataForQueue.iPHvalue   = currentNutrients[0].iPHvalue;
        dataForQueue.iECvalue   = currentNutrients[0].iECvalue; 
        xQueueSendToBack(xQueueControllerData, &dataForQueue, portMAX_DELAY);
        return 1;
    }
    else if (Data_Storage_Test == 1)
    {
        dataForQueue.iPHvalue   = 1;
        dataForQueue.iECvalue   = 2;
        dataForQueue.iWaterTemp = 3;
        dataForQueue.iWaterFlow = 4;
        dataForQueue.iC02       = 5;
        dataForQueue.iTempAir   = 6;
        dataForQueue.iRH        = 7;
        dataForQueue.iLightVal  = 8;
        xQueueSendToBack(xQueueControllerData, &dataForQueue, portMAX_DELAY);
        return 1;
    }
    
    else
    return 0;
}



void vTaskDataStorage()
{
    const TickType_t xDelayms = pdMS_TO_TICKS( 1000 ); // Sets the measurement resolution.
    int readPtr = 0;
    int writePtr = 0;
    _Bool bufferFull = 0;
    _Bool bufferEmpty = 1;
    BaseType_t centralRequest;
    const int storageSizes = 20;
    struct allData allDataArray[storageSizes];
    _Bool queueStatusCentral;
    
    for(;;)
    {
        vDataQueueing();
        if(vDataQueueing() && (bufferFull == 0))
        {
            xQueueReceive(xQueueControllerData, &allDataArray[writePtr], portMAX_DELAY);  
            writePtr ++;
            bufferEmpty = 0;
            if(writePtr >= storageSizes)
            {
                writePtr = 0;
            }
            if(readPtr == writePtr)
            {
                bufferFull = 1;
            }
        }
            
        centralRequest = xQueueReceive(xQueueCentralrequest, &queueStatusCentral, portMAX_DELAY);    
        if(centralRequest == pdTRUE && (bufferEmpty == 0))
        {
            xQueueSendToBack(xQueueCentralData, &allDataArray[readPtr], portMAX_DELAY);
            readPtr ++;
            if(readPtr >= storageSizes)
            {
                readPtr = 0;
            }
            if(readPtr == writePtr)
            {
                bufferEmpty = 1;
            }    
        } 
        vTaskDelay(xDelayms);
    }     
}



        // Test tasks here // 

void vTaskTestData()
{

}









/* [] END OF FILE */
