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
char cRecievedData[8]; //program only works if this is a char (??)

QueueHandle_t xQueueSendDataRequest;
QueueHandle_t xQueueSendNewParams;

QueueHandle_t xQueueRecievedDataRequest;
QueueHandle_t xQueueRecievedNewParams;


/* Struct for holding data when sending to another task, either data request or new params */
struct Request{
    char cID;
    float Value[10];
};
struct Request RecievedParams; //for internally sending recieved params 
struct Request RecievedData;   //for internally sending recieved data request

void vTaskComsInit(){
    UART_Start();
    //UART_PutString("function start \n"); USED FOR TEST
    /* Create queue for sending data request to the Data store task */
    xQueueSendDataRequest = xQueueCreate(8, sizeof(uint16));
    
    /* Create queue for sending new params to the New Params task */
    xQueueSendNewParams = xQueueCreate(8, sizeof(uint16));
    
    /* Creatw queues for sending recieved data requests and new params between functions in this file */
    xQueueRecievedDataRequest = xQueueCreate(10, sizeof( struct Request ));
    xQueueRecievedNewParams = xQueueCreate(10, sizeof( struct Request ));
    
   
    xTaskCreate(vRecieveFromFPGA, "FPGA recieve", 100, NULL, 2, NULL);
    xTaskCreate(vSendDataRequest, "Send data request", 100, NULL, 2, NULL);
    xTaskCreate(vSendNewParams, "Send new params", 100, NULL, 2, NULL);
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
    for(;;){
        
        if(UART_GetRxBufferSize() == 1) //returns 1 for not empty RX FIFO
            {
                cRecievedData[0] = UART_GetByte(); //recieve the indentifier
                                
                switch(cRecievedData[0])
                {
                    case '0': //case with request for data
                        
                        /* I assume that we need X parameters in a data request: 
                           - identifier for the sensor 
                           - identifier for the amount of data 
                        */
                        
                        UART_PutString("Data request case \n"); //USED FOR TEST
                        RecievedData.cID = UART_GetChar();
                        
                        while(i < 10){
                            RecievedData.Value[i] = UART_GetChar();
                            i++;
                        }    
                        i = 0;
                        
                        
                        xQueueSendToBack(xQueueRecievedDataRequest, (void *) &RecievedData, portMAX_DELAY);
                    
                        break;
                    
                    case '1': //case with new param
                    
                        /* I assume that we need X parameters in a new param request: 
                           - identifier for the sensor 
                           - new value
                        */
                        
                        UART_PutString("New param case \n"); //USED FOR TEST
                        RecievedParams.cID = UART_GetChar(); 
                        
                        while(i < 10){
                            RecievedParams.Value[i] = UART_GetChar();
                            i++;
                        }    
                        i = 0;
                        
                        // USED FOR TEST
                        UART_PutString("ID: ");
                        UART_PutChar(RecievedParams.cID);
                          
                        UART_PutString(", value: ");
                        
                        while(i < 10){
                            UART_PutChar(RecievedParams.Value[i]);
                            i++;
                        }
                        i = 0;
                        UART_PutString("\n");
                        
                        xQueueSendToBack(xQueueRecievedNewParams, (void *) &RecievedParams, portMAX_DELAY);
                        
                        break;
                    
                }
                
            }
            
    }
    
}

void vSendDataRequest(){
    struct Request SendDataRequest;
    
    for(;;)
    {
        xQueueReceive(xQueueRecievedDataRequest, &(SendDataRequest), portMAX_DELAY);
        
        /*UART_PutString("\n");
        UART_PutString("Data request to send: \n");
        UART_PutString("ID: ");
        UART_PutChar(SendDataRequest.cID);
        UART_PutString(", value: ");
        for(int i = 0; i == 1; i++){
            UART_PutChar(SendDataRequest.Value[i]);
        }
        UART_PutString("\n");
        */
        // send data request to Data storage task
        
    }    
    
}  

void vSendNewParams(){
    struct Request SendParams;
    
    for(;;)
    {
        xQueueReceive(xQueueRecievedNewParams, &(SendParams), portMAX_DELAY);
        
        /*UART_PutString("\n");
        UART_PutString("New params to send: \n");
        UART_PutString("ID: ");
        UART_PutChar(SendParams.cID);
        UART_PutString(", value: ");
        for( i = 0; i < 10; i++){
            UART_PutChar(SendParams.Value[i]);
        }
        UART_PutString("\n");*/
        
        // send params to the queue in New Params task 
        
        
    }    
    
}    