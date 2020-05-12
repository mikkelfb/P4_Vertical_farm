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
#include "Communication.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


uint16 i = 0;
char cRecievedData; //program only works if this is a char (??)
BaseType_t DataRequest;

QueueHandle_t xQueueDataToCentral;
QueueHandle_t xQueueSendNewParams;

QueueHandle_t xQueueRecievedDataRequest;
QueueHandle_t xQueueRecievedNewParams;

/* TEST QUEUES */
//QueueHandle_t xQueueTestData;

/* Struct for holding data when sending to another task, either data request or new params */
struct Params{
    char cID;
    float Value[8];
};
struct Params RecievedParams; //for internally sending recieved params 

/* struct from the data storage task */
struct Data{
    uint16  iPHval;
    uint16  iECval;
    uint16  iWaterT;
    uint16  iWaterF;
    uint16  iC02val;
    uint16  iTempA;
    uint16  iRHval;
    uint16  iLightValue;
};    

void vBitShifterUART(uint16 command){

    uint8_t buf[2] = {0};
    
    buf[0] = command & 0xff;
    buf[1] = (command >> 8);
    
    UART_PutArray(buf, 2);
    
} 

void vTaskComsInit(){
    UART_Start();
    
    /* Create queue for sending data to the central */
    xQueueDataToCentral = xQueueCreate(10, sizeof( struct Data ));
    
    /* Create queue for sending new params to the New Params task */
    xQueueSendNewParams = xQueueCreate(8, sizeof(uint16));
    
    /* Creatw queues for sending recieved data requests and new params between functions in this file */
    xQueueRecievedDataRequest = xQueueCreate(10, sizeof( BaseType_t ));
    xQueueRecievedNewParams = xQueueCreate(10, sizeof( struct Params ));
        
    
    /* TEST QUEUES */
    //xQueueTestData = xQueueCreate(20, sizeof( struct Data ));
    
    xTaskCreate(vTaskRecieveFromFPGA, "FPGA recieve", 100, NULL, 2, NULL);
    xTaskCreate(vTaskSendDataRequest, "Send data request", 100, NULL, 2, NULL);
    xTaskCreate(vTaskSendNewParams, "Send new params", 100, NULL, 2, NULL);
    xTaskCreate(vTaskSendToFPGA, "Send message to FPGA", 100, NULL, 2, NULL);
    
}


  

/* This function consists of these steps: 
   - wait for not empty Rx fifo
   - if first recieved byte is 0 (data request): 
       - Datarequest is set to true, and sent in xQueueRecievedDataRequest
   - if first recieved byte is 1 (new params):
       - save second recieved byte as sensor ID 
       - save third recieved byte as new value
       - send these to vTaskSendNewParams()
*/
void vTaskRecieveFromFPGA(){
    
    const TickType_t xDelayms = pdMS_TO_TICKS( 100 );
    for(;;){
        
        if(UART_GetRxBufferSize() == 1) //returns 1 for not empty RX FIFO
            {
                cRecievedData = UART_GetByte(); //recieve the indentifier
                                
                switch(cRecievedData)
                {
                    case '0': //case with request for data
                        
                        //UART_PutString("Data request case \n"); //USED FOR TEST
                        
                        DataRequest = pdTRUE;
                        
                        /* USED FOR TEST
                        if(DataRequest == pdTRUE)
                        {
                            UART_PutString("\n");
                            UART_PutString("Data request to send: TRUE \n");
                            UART_PutString("\n");        
                        }    
                        else{    
                            UART_PutString("\n");
                            UART_PutString("Data request to send: FALSE \n");
                        }*/
                        
                        /* only works after merge with new params */
                        //xQueueSendToBack(xQueueCentralrequest, DataRequest, portMAX_DELAY);
        
                    
                        xQueueSendToBack(xQueueRecievedDataRequest, &(DataRequest), portMAX_DELAY);
                        
                        DataRequest = pdFALSE;
                        
                        break;
                    
                    case '1': //case with new param
                    
                        //UART_PutString("New param case \n"); //USED FOR TEST
                        vTaskDelay(xDelayms);
                        RecievedParams.cID = UART_GetChar(); 
                        
                        while(i < 10){
                            RecievedParams.Value[i] = UART_GetChar();
                            i++;
                        }    
                        i = 0;
                        
                        /* USED FOR TEST
                        UART_PutString("ID: ");
                        UART_PutChar(RecievedParams.cID);
                          
                        UART_PutString(", value: ");
                        
                        while(i < 10){
                            UART_PutChar(RecievedParams.Value[i]);
                            i++;
                        }
                        i = 0;
                        UART_PutString("\n");*/
                        
                        xQueueSendToBack(xQueueRecievedNewParams, (void *) &RecievedParams, portMAX_DELAY);
                        
                        break;
                    
                }
                
            }
            
    }
    
}

void vTaskSendDataRequest(){
    //extern QueueHandle_t xQueueCentralrequest; //only works after merge with data storage
    BaseType_t SendDataRequest;
    struct Data Send;
    const TickType_t xDelayms = pdMS_TO_TICKS( 100 );
    
    for(;;)
    {
        xQueueReceive(xQueueRecievedDataRequest, &(SendDataRequest), portMAX_DELAY);
        
        if(SendDataRequest == pdTRUE)
        {
            /* USED FOR TEST
            UART_PutString("\n");
            UART_PutString("Data request: TRUE \n");
            UART_PutString("\n");   
            
            vTaskDelay(xDelayms);
            
            Send.iC02val = 48;
            Send.iECval = 49;          
            Send.iLightValue = 50;
            Send.iPHval = 51;
            Send.iRHval = 52;
            Send.iTempA = 53;
            Send.iWaterF = 54;
            Send.iWaterT = 55;
            
            xQueueSendToBack(xQueueTestData, (void *) &Send, portMAX_DELAY);*/
        }    
        else{    
            /* USED FOR TEST 
            UART_PutString("\n");
            UART_PutString("Data request: FALSE \n");*/
        }        
    }    
    
}  

void vTaskSendNewParams(){
    struct Params SendParams;
    
    for(;;)
    {
        xQueueReceive(xQueueRecievedNewParams, &(SendParams), portMAX_DELAY);
        
        /* USED FOR TEST
        UART_PutString("\n");
        UART_PutString("New params to send: \n");
        UART_PutString("ID: ");
        UART_PutChar(SendParams.cID);
        UART_PutString(", value: ");
        while(i < 10){
            UART_PutChar(SendParams.Value[i]);
            i++;
        }
        i = 0;
        UART_PutString("\n");*/
        
        /* only works after merge with new params */
        //xQueueSendToBack(xQueueSendNewParams, (void *) &SendParams, portMAX_DELAY);
        
        
    }    
    
}

void vTaskSendToFPGA(){
    //extern QueueHandle_t xQueueCentralData; //only works after merge with data storage
    struct Data SendToCentral; 
    
    
    for(;;)
    {
        //xQueueReceive(xQueueCentralData, &(SendToCentral), portMAX_DELAY); this queue is for real
        //xQueueReceive(xQueueTestData, &SendToCentral, portMAX_DELAY); //this Queue is for test
        
        //UART_PutString("Data recieved from data storage: \n"); //USED FOR TEST
        vBitShifterUART(SendToCentral.iPHval);
        vBitShifterUART(SendToCentral.iECval);
        vBitShifterUART(SendToCentral.iWaterT);
        vBitShifterUART(SendToCentral.iWaterF);
        vBitShifterUART(SendToCentral.iC02val);
        vBitShifterUART(SendToCentral.iTempA);
        vBitShifterUART(SendToCentral.iRHval);
        vBitShifterUART(SendToCentral.iLightValue);        
    }    
    
} 