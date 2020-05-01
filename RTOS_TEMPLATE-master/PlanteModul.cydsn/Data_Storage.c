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



int testing = 1;
QueueHandle_t xQueueControllerData;
QueueHandle_t xQueueCentralrequest;
QueueHandle_t xQueueCentralData;
QueueHandle_t xQueueStorageReady;


void vInitDataStorage()
{
    xQueueControllerData = xQueueCreate(10, sizeof(struct dataMessage));
    xQueueCentralData = xQueueCreate(10, sizeof(struct allData));
    xQueueCentralrequest = xQueueCreate(1 , sizeof(uint16));
    xQueueStorageReady = xQueueCreate(1 , sizeof(struct allData));
    
    xTaskCreate(vTaskDataStorage, "Data Storage", 1000, NULL , 2 , NULL);
}


/*
dataMessage recivedmes
xQueueRecive(controllerdata, &recivedmes, portMAX_DELAY)
switch recivedmes.identifier
    case'p':
        alldata.iPhvalue = recivedmes.message


datamessage sendmessage
sendmessage.message = phvalue 
sendmessage.identifier = 'p'
xQueueSendToBack(sendmessage, &controllerdata, portMAX_DELAY)
*/

_Bool vDataQueueing()
{
    struct allData dataForQueue;
    struct dataMessage recievedMessage;
    _Bool pReady = 0, eReady = 0, wReady = 0, fReady = 0, cReady = 0, tReady = 0, rReady = 0, lReady = 0;
    
    
    BaseType_t  xStatus = xQueueReceive(xQueueControllerData, &recievedMessage, portMAX_DELAY);
    if (xStatus == pdPASS)
    {
        switch (recievedMessage.identifier)
        {
            case 'p':   //PHvalue
            dataForQueue.iPHvalue   = recievedMessage.message;
            pReady = 1;
            case 'e':   //ECvalue
            dataForQueue.iECvalue   = recievedMessage.message;
            eReady = 1;
            case 'w':   //WaterTemp
            dataForQueue.iWaterTemp = recievedMessage.message;
            wReady = 1;
            case 'f':   //WaterFlow
            dataForQueue.iWaterFlow = recievedMessage.message;
            fReady = 1;
            case 'c':   //Co2value
            dataForQueue.iC02       = recievedMessage.message;
            cReady = 1;
            case 't':   //AirTemp
            dataForQueue.iTempAir   = recievedMessage.message;
            tReady = 1;
            case 'r':   //RelativHum
            dataForQueue.iRH        = recievedMessage.message;
            rReady = 1;
            case 'l':   //lightVal
            dataForQueue.iLightVal  = recievedMessage.message;
            lReady = 1;
        }
    }
    if (pReady & eReady & wReady & fReady & cReady & tReady & rReady & lReady)
    {
        xQueueSendToBack(xQueueStorageReady, &dataForQueue, portMAX_DELAY);
        return 1;
        pReady = 0, eReady = 0, wReady = 0, fReady = 0, cReady = 0, tReady = 0, rReady = 0, lReady = 0;
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
    _Bool queueStatus;
    _Bool queueStatusCentral;
    
    for(;;)
    {
        queueStatus = vDataQueueing();
        if((queueStatus == 1) && (bufferFull == 0))
        {
            xQueueReceive(xQueueStorageReady, &allDataArray[writePtr], portMAX_DELAY);
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
