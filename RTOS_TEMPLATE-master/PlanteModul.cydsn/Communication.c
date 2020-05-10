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

char8 Buffersize;
uint i = 0;
char cRecievedData[8]; //program only works if this is a char (??)

QueueHandle_t xQueueSendDataRequest;
QueueHandle_t xQueueSendNewParams;

QueueHandle_t xQueueRecievedDataRequest;
QueueHandle_t xQueueRecievedNewParams;


/* Struct for holding data when sending to another task, either data request or new params */
struct Request{
    char cID;
    float Value;
};
struct Request RecievedParams;

void vTaskComsInit(){
    UART_Start();
    UART_PutString("function start \n");
    /* Create queue for sending data request to the Data store task */
    xQueueSendDataRequest = xQueueCreate(8, sizeof(uint16));
    
    /* Create queue for sending new params to the New Params task */
    xQueueSendNewParams = xQueueCreate(8, sizeof(uint16));
    
    /* Creatw queues for sending recieved data requests and new params between functions in this file */
    xQueueRecievedDataRequest = xQueueCreate(1, sizeof(uint16));
    xQueueRecievedNewParams = xQueueCreate(1, sizeof(uint16));
    
   
    xTaskCreate(vRecieveFromFPGA, "FPGA recieve", 100, NULL, 2, NULL);
    //xTaskCreate(vSendDataRequest, "Send data request", 100, NULL, 2, NULL);
    //xTaskCreate(vSendNewParams, "Send new params", 100, NULL, 2, NULL);
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
    UART_PutString("function2 start \n");
    for(;;){
        
        if(UART_GetRxBufferSize() == 1) //returns 1 for not empty RX FIFO
            {
                cRecievedData[0] = UART_GetByte(); //recieve the indentifier
                UART_PutChar(cRecievedData[0]);
                
                switch(cRecievedData[0])
                {
                    case '0': //case with request for data
                        
                        /* I assume that we need X parameters in a data request: 
                           - identifier for the sensor 
                           - identifier for the amount of data 
                        */
                        
                        UART_PutString("Data request case \n");    
                    
                        break;
                    
                    case '1': //case with new param
                    
                        /* I assume that we need X parameters in a new param request: 
                           - identifier for the sensor 
                           - new value
                        */
                        UART_PutString("New param case \n");
                        RecievedParams.cID = UART_GetByte(); 
                        RecievedParams.Value = UART_GetByte();
                        
                        UART_PutString("ID: ");
                        UART_PutChar(RecievedParams.cID);
                        UART_PutString(", value: ");
                        UART_PutChar(RecievedParams.Value);
                        UART_PutString("\n");
                    
                        break;
                    
                }
                
            }
            /*
        else //returns 0 for empty RX FIFO
            {
                i = 0;
            }*/
    }
    
}
/*
void vSendDataRequest(){
    
    
}  */  

void vSendNewParams(){
    
    for(;;){
        
        
        
    }    
    
}    