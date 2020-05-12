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

uint16 Buffersize;
uint16 i = 0;
char cRecievedData; //program only works if this is a char (??)
BaseType_t DataRequest;

//QueueHandle_t xQueueSendDataRequest;
QueueHandle_t xQueueDataToCentral;
QueueHandle_t xQueueSendNewParams;

QueueHandle_t xQueueRecievedDataRequest;
QueueHandle_t xQueueRecievedNewParams;

/* TEST QUEUES */
QueueHandle_t xQueueTestData;

/* Struct for holding data when sending to another task, either data request or new params */
struct Params{
    char cID;
    float Value[8];
};
struct Params RecievedParams; //for internally sending recieved params 
//struct Request RecievedData;   //for internally sending recieved data request

/* struct from the data storage task, the float and array were used for test */
struct Data{
    uint16  iPHval;
    float  iECval;
    uint16  iWaterT;
    uint16  iWaterF;
    uint16  iC02val[4];
    uint16  iTempA;
    uint16  iRHval;
    uint16  iLightValue;
};    
struct Data SendToCentral; //for sending data to the central

uint16_t command;

void vTaskComsInit(){
    UART_Start();
    //UART_PutString("function start \n"); USED FOR TEST
    /* Create queue for sending data request to the Data store task */
    //xQueueSendDataRequest = xQueueCreate(8, sizeof(uint16));
    
    /* Create queue for sending data to the central */
    xQueueDataToCentral = xQueueCreate(10, sizeof( struct Data ));
    
    /* Create queue for sending new params to the New Params task */
    xQueueSendNewParams = xQueueCreate(8, sizeof(uint16));
    
    /* Creatw queues for sending recieved data requests and new params between functions in this file */
    xQueueRecievedDataRequest = xQueueCreate(10, sizeof( BaseType_t ));
    xQueueRecievedNewParams = xQueueCreate(10, sizeof( struct Params ));
        
    
    /* TEST QUEUES */
    xQueueTestData = xQueueCreate(20, sizeof( struct Data ));
    
    xTaskCreate(vRecieveFromFPGA, "FPGA recieve", 100, NULL, 2, NULL);
    xTaskCreate(vSendDataRequest, "Send data request", 100, NULL, 2, NULL);
    xTaskCreate(vSendNewParams, "Send new params", 100, NULL, 2, NULL);
    xTaskCreate(vSendToFPGA, "Send message to FPGA", 100, NULL, 2, NULL);
    
    xTaskCreate(UART_writeCommandWithArguments, "bifshift", 100, (void*) command, 2, NULL);
}


/* This function consists of these steps: 
   - wait for not empty Rx fifo
   - if first recieved byte is 0 (data request): 
       - TBD
   - if first recieved byte is 1 (new params):
       - save second recieved byte as sensor ID 
       - save third recieved byte as new value
       - send these to vSendNewParams()
*/
void vRecieveFromFPGA(){
    //UART_PutString("function2 start \n"); USED FOR TEST
    const TickType_t xDelayms = pdMS_TO_TICKS( 100 );
    for(;;){
        
        if(UART_GetRxBufferSize() == 1) //returns 1 for not empty RX FIFO
            {
                cRecievedData = UART_GetByte(); //recieve the indentifier
                                
                switch(cRecievedData)
                {
                    case '0': //case with request for data
                        
                        /* I assume that we need X parameters in a data request: 
                           - identifier for the sensor 
                           - identifier for the amount of data 
                        */
                        
                        //UART_PutString("Data request case \n"); //USED FOR TEST
                        
                        DataRequest = pdTRUE;
                        
                        if(DataRequest == pdTRUE)
                        {
                            UART_PutString("\n");
                            UART_PutString("Data request to send: TRUE \n");
                            UART_PutString("\n");        
                        }    
                        else{    
                            UART_PutString("\n");
                            UART_PutString("Data request to send: FALSE \n");
                        }
                        /* only works after merge with new params */
                        //xQueueSendToBack(xQueueCentralrequest, DataRequest, portMAX_DELAY);
        
                    
                    
                        xQueueSendToBack(xQueueRecievedDataRequest, &(DataRequest), portMAX_DELAY);
                        
                        DataRequest = pdFALSE;
                        
                        break;
                    
                    case '1': //case with new param
                    
                        /* I assume that we need X parameters in a new param request: 
                           - identifier for the sensor 
                           - new value
                        */
                        
                        //UART_PutString("New param case \n"); //USED FOR TEST
                        vTaskDelay(xDelayms);
                        RecievedParams.cID = UART_GetChar(); 
                        
                        while(i < 10){
                            RecievedParams.Value[i] = UART_GetChar();
                            i++;
                        }    
                        i = 0;
                        
                        /*// USED FOR TEST
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

void vSendDataRequest(){
    //extern QueueHandle_t xQueueCentralrequest; //only works after merge with data storage
    //struct Request SendDataRequest;
    BaseType_t SendDataRequest;
    struct Data Send;
    const TickType_t xDelayms = pdMS_TO_TICKS( 100 );
    
    for(;;)
    {
        xQueueReceive(xQueueRecievedDataRequest, &(SendDataRequest), portMAX_DELAY);
        
        if(SendDataRequest == pdTRUE)
        {
            UART_PutString("\n");
            UART_PutString("Data request: TRUE \n");
            UART_PutString("\n");   
            
            vTaskDelay(xDelayms);
            
            Send.iC02val[0] = '1';
            Send.iC02val[1] = ',';
            Send.iC02val[2] = '3';
            Send.iC02val[3] = '8';
            Send.iECval = 2.5;          // used for testing the sprintf function
            Send.iLightValue = 1;
            Send.iPHval = 42;
            Send.iRHval = 5;
            Send.iTempA = 3;
            Send.iWaterF = 1;
            Send.iWaterT = 2;
            
            xQueueSendToBack(xQueueTestData, (void *) &Send, portMAX_DELAY);
        }    
        else{    
            UART_PutString("\n");
            UART_PutString("Data request: FALSE \n");
        }
        /* only works after merge with new params */
        //xQueueSendToBack(xQueueCentralrequest, DataRequest, portMAX_DELAY);
        
        /* This line should be completed in the process of putting the communication task together with the data storage */
        //xQueueRecieve (the correct Queue, a location to save it, portMAX_DELAY);
        
        
    }    
    
}  

void vSendNewParams(){
    struct Params SendParams;
    
    for(;;)
    {
        xQueueReceive(xQueueRecievedNewParams, &(SendParams), portMAX_DELAY);
        
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
        UART_PutString("\n");
        
        /* only works after merge with new params */
        //xQueueSendToBack(xQueueSendNewParams, (void *) &SendParams, portMAX_DELAY);
        // send params to the queue in New Params task 
        
        
    }    
    
}

void vSendToFPGA(){
    //extern QueueHandle_t xQueueCentralData; //only works after merge with data storage
    const TickType_t xDelayms = pdMS_TO_TICKS( 100 );
    char test[3];
    float test2 = 2.3;  // used for testing the sprintf function
    
    for(;;)
    {
        //xQueueReceive(xQueueCentralData, &(SendToCentral), portMAX_DELAY);
        xQueueReceive(xQueueTestData, &(SendToCentral), portMAX_DELAY);
        //i = 0;
        
        
        
        
        //char test = (char)SendToCentral.iC02val;
        UART_PutString("Recieved data to send to central: \n");
        
        vTaskDelay(xDelayms);
        //UART_PutArray((uint8 *) &SendToCentral.iC02val, 8);
        /*while(i < 4){
            UART_PutChar(SendToCentral.iC02val[i]);
            i++;
        }
        i = 0;*/
        //UART_PutChar(SendToCentral.iC02val);
        //UART_PutChar((uint16)1);
        UART_PutString("\n");
        
        //sprintf(test, "%f", test2);     // around here things stop working
        //UART_PutString(test);
        //vTaskDelay(xDelayms);
        //UART_ClearTxBuffer();
        //UART_PutString("\n");
        //UART_PutChar(SendToCentral.iLightValue);
        //UART_PutString("\n");
        
        
        /*UART_PutChar(SendToCentral.iPHval);
        UART_PutString("\n");
        UART_PutChar(SendToCentral.iRHval);
        UART_PutString("\n");*/
        /*UART_PutChar(SendToCentral.iTempA);
        UART_PutString("\n");
        UART_PutChar(SendToCentral.iWaterF);
        UART_PutString("\n");
        UART_PutChar(SendToCentral.iWaterT);
        UART_PutString("\n");*/
        
        
        /* only works after merge with data store */
        //xQueueSendToBack(xQueueDataToCentral, (void *) &SendToCentral, portMAX_DELAY);
        
    }    
    
}

void UART_writeCommandWithArguments(uint16_t command){
    static uint8_t buf[2] = {0};
    
    buf[0] = command >> 8;
    buf[1] = command;
    
    UART_PutArray(buf, 2);
    
}    