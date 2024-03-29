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
#include "Data_Storage.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "Shared_resources.c"

struct allData{ // Struct contains all nutrients values measured, 
    uint16  iPHval;
    uint16  iECval;
    uint16  iWaterT;
    uint16  iWaterF;
    uint16  iC02val;
    uint16  iTempA;
    uint16  iRHval;
    uint16  iLightValue;
};

struct dataMessage{ // struct type for messages sent by the controller tasks
    char identifier;
    uint16 message;
};


void vInitDataStorage()                 // Initializing and creating tasks for the DataStorage module
{
    xQueueControllerData = xQueueCreate(20, sizeof(struct dataMessage));
    xQueueCentralData = xQueueCreate(10, sizeof(struct allData));
    xQueueCentralrequest = xQueueCreate(1 , sizeof(uint16));
    xQueueStorageReady = xQueueCreate(10 , sizeof(struct allData));
    
    xTaskCreate(vTaskDataStorage, "Data Storage", 1000, NULL , 4 , NULL);
    xTaskCreate(vTaskDataQueueing, "Data Queueing", 1000, NULL, 4, NULL);
    
    #if Data_Storage_Test == 1
        vTestStorageInit();
    #endif
}

void vTaskDataQueueing()                // This task gathers datavalues from controllertasks and queue the 
{                                       // alldata struct for DataStorage task
    const TickType_t xDelayms = pdMS_TO_TICKS( 100 );
    struct allData dataForQueue;
    struct dataMessage recievedMessage;
    _Bool pReady = 0, eReady = 0, wReady = 0, fReady = 0, cReady = 0, tReady = 0, rReady = 0, lReady = 0;
    
    for(;;)
    {
        BaseType_t  xStatus = xQueueReceive(xQueueControllerData, &recievedMessage, portMAX_DELAY);
        if (xStatus == pdPASS)
        {
            vTaskDelay(xDelayms);
            switch (recievedMessage.identifier)
            {
                case 'p':   //PHvalue
                    dataForQueue.iPHval     = recievedMessage.message;
                    pReady = 1;
//                    SW_UART_TEST_USB_PutString("PH \n");
                    break;
                case 'e':   //ECvalue
                    dataForQueue.iECval     = recievedMessage.message;
                    eReady = 1;
//                    SW_UART_TEST_USB_PutString("EC \n");
                    break;
                case 'w':   //WaterTemp
                    dataForQueue.iWaterT    = recievedMessage.message;
                    wReady = 1;
//                    SW_UART_TEST_USB_PutString("WT \n");
                    break;
                case 'f':   //WaterFlow
                    dataForQueue.iWaterF    = recievedMessage.message;
                    fReady = 1;
//                    SW_UART_TEST_USB_PutString("WF \n");
                    break;
                case 'c':   //Co2value
                    dataForQueue.iC02val    = recievedMessage.message;
                    cReady = 1;
//                    SW_UART_TEST_USB_PutString("CO2 \n");
                    break;
                case 't':   //AirTemp
                    dataForQueue.iTempA     = recievedMessage.message;
                    tReady = 1;
//                    SW_UART_TEST_USB_PutString("AT \n");
                    break;
                case 'r':   //RelativHum
                    dataForQueue.iRHval     = recievedMessage.message;
                    rReady = 1;
//                    SW_UART_TEST_USB_PutString("RH \n");
                    break;
                case 'l':   //lightVal
                    dataForQueue.iLightValue= recievedMessage.message;
                    lReady = 1;
//                    SW_UART_TEST_USB_PutString("LV \n");
                    break;
            }
        }
        if (pReady & eReady & wReady & fReady & cReady & tReady & rReady & lReady)
        {
            xQueueSendToBack(xQueueStorageReady, &dataForQueue, portMAX_DELAY);
            pReady = 0, eReady = 0, wReady = 0, fReady = 0, cReady = 0, tReady = 0, rReady = 0, lReady = 0;
        //    SW_UART_TEST_USB_PutString("AllData Queued");
        //    SW_UART_TEST_USB_PutString("\n");
        }
    }
    
}



void vTaskDataStorage()                         // This task works as a FIFO buffer for the 
{                                               // central task to request data from
    const TickType_t smallDelay = pdMS_TO_TICKS( 10 );
    const TickType_t bigDelay = pdMS_TO_TICKS( 1000 );
    int readPtr = 0;
    int writePtr = 0;
    _Bool bufferFull = 0;
    _Bool bufferEmpty = 1;
    BaseType_t centralRequest;
    BaseType_t queueRequest;
    const int storageSizes = 20;
    struct allData allDataArray[storageSizes];
    _Bool queueStatus;
    _Bool queueStatusCentral;
    
    for(;;)
    {
        if(bufferFull == 0)
        {// task takes allData struct from dataQueueing task
            queueRequest = xQueueReceive(xQueueStorageReady, &allDataArray[writePtr], smallDelay);
            if((queueRequest == pdPASS) && (bufferFull == 0))
            {
                writePtr ++;
/*
                SW_UART_TEST_USB_PutString("Data in array");
                SW_UART_TEST_USB_PutString("\n");
                SW_UART_TEST_USB_PutString("PH: ");
                SW_UART_TEST_USB_PutHexInt(allDataArray[readPtr].iPHval);
                SW_UART_TEST_USB_PutString("\n");
                SW_UART_TEST_USB_PutString("EC: " );
                SW_UART_TEST_USB_PutHexInt(allDataArray[readPtr].iECval);
                SW_UART_TEST_USB_PutString("\n");
                SW_UART_TEST_USB_PutString("WT: " );
                SW_UART_TEST_USB_PutHexInt(allDataArray[readPtr].iWaterT);
                SW_UART_TEST_USB_PutString("\n");
                SW_UART_TEST_USB_PutString("WF: " );
                SW_UART_TEST_USB_PutHexInt(allDataArray[readPtr].iWaterF);
                SW_UART_TEST_USB_PutString("\n");
                SW_UART_TEST_USB_PutString("CO2: " );
                SW_UART_TEST_USB_PutHexInt(allDataArray[readPtr].iC02val);
                SW_UART_TEST_USB_PutString("\n");
                SW_UART_TEST_USB_PutString("AT: " );
                SW_UART_TEST_USB_PutHexInt(allDataArray[readPtr].iTempA);
                SW_UART_TEST_USB_PutString("\n");
                SW_UART_TEST_USB_PutString("RH: " );
                SW_UART_TEST_USB_PutHexInt(allDataArray[readPtr].iRHval);
                SW_UART_TEST_USB_PutString("\n");
                SW_UART_TEST_USB_PutString("LV: " );
                SW_UART_TEST_USB_PutHexInt(allDataArray[readPtr].iLightValue);
                SW_UART_TEST_USB_PutString("\n");
*/
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
        }
                                                // task checks for request from central and sends allData struct if true
        centralRequest = xQueueReceive(xQueueCentralrequest, &queueStatusCentral, bigDelay);    
        if((centralRequest == pdPASS) && (bufferEmpty == 0))
        {
            //SW_UART_TEST_USB_PutString("Sending data to central");
            //SW_UART_TEST_USB_PutString("\n");
            bufferFull = 0;
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
     //   vTaskDelay(xDelayms);
    }
}




                    /////////////////// Test tasks here /////////////////////////////////////// 
void vTestStorageInit()
{
//    xTaskCreate(vTaskTestData, "StorageTest", 1000, NULL, 3, NULL);
}

void vTaskTestData()
{                                   // Creating structs to simulate messages from controllertasks
    int centralRequest = 1;
    struct allData testData;
    const TickType_t xDelayms = pdMS_TO_TICKS( 2000 );
    
    struct dataMessage testMessage1;
    testMessage1.identifier = 'p';
    testMessage1.message = 1;
    
    struct dataMessage testMessage2;
    testMessage2.identifier = 'e';
    testMessage2.message = 2;
    
    struct dataMessage testMessage3;
    testMessage3.identifier = 'w';
    testMessage3.message = 3;
    
    struct dataMessage testMessage4;
    testMessage4.identifier = 'f';
    testMessage4.message = 4;
    
    struct dataMessage testMessage5;
    testMessage5.identifier = 'c';
    testMessage5.message = 5;
    
    struct dataMessage testMessage6;
    testMessage6.identifier = 't';
    testMessage6.message = 6;
    
    struct dataMessage testMessage7;
    testMessage7.identifier = 'r';
    testMessage7.message = 7;
    
    struct dataMessage testMessage8;
    testMessage8.identifier = 'l';
    testMessage8.message = 8;
   
    
/*    
    struct allData testForBitshift;
    testForBitshift.iPHval = 1;
    testForBitshift.iECval = 1;
    testForBitshift.iLightValue = 1;
    testForBitshift.iC02val = 1;
    testForBitshift.iRHval = 1;
    testForBitshift.iTempA = 1;
    testForBitshift.iWaterF = 1;
    testForBitshift.iWaterT = 1;
*/    
    
    
    for(;;)
    {                                   // Sending testmessages for DataQueueing task to pick up
        xQueueSendToBack(xQueueControllerData, &testMessage1, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &testMessage2, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &testMessage3, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &testMessage4, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &testMessage5, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &testMessage6, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &testMessage7, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &testMessage8, portMAX_DELAY);

                                        // Requesting what the central task would request
//        xQueueSendToBack(xQueueCentralrequest, &centralRequest, portMAX_DELAY);
                                        // Recieving what the central task would recieve
//        xQueueReceive(xQueueCentralData, &testData, portMAX_DELAY);

/*        // Printing values via UART
        SW_UART_TEST_USB_PutString("testdata:");
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutHexByte(testData.iPHval);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutHexByte(testData.iECval);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutHexByte(testData.iWaterT);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutHexByte(testData.iWaterF);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutHexByte(testData.iC02val);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutHexByte(testData.iTempA);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutHexByte(testData.iRHval);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutHexByte(testData.iLightValue);
        SW_UART_TEST_USB_PutString("\n");

 */                                   // incrementing values for the test messages
        testMessage1.message ++;
        testMessage2.message ++;
        testMessage3.message ++;
        testMessage4.message ++;
        testMessage5.message ++;
        testMessage6.message ++;
        testMessage7.message ++;
        testMessage8.message ++;
        
//        xQueueSendToBack(xQueueCentralData, &testForBitshift, portMAX_DELAY);
        vTaskDelay(xDelayms);
    }    
}

/* [] END OF FILE */
