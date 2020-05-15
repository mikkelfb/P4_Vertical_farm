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
#include "Shared_resources.c"



char cRecievedData; //program only works if this is a char (??)
BaseType_t DataRequest;

QueueHandle_t xQueueDataToCentral;

QueueHandle_t xQueueRecievedNewParams;
QueueHandle_t xQueueRecievedDataRequest;

QueueHandle_t xQueueRecievedAlarm;
QueueHandle_t xQueueAlarmACK;
QueueHandle_t xQueueWaitForACK;

/* TEST QUEUES */
//QueueHandle_t xQueueTestData;
//QueueHandle_t xQueueTestAlarm;

/* Struct for holding data when sending to another task, either data request or new params */
struct Params{
    char cID;
    uint8_t Value[2];
};


/* Struct for holding recieved alarm data from alarm task */
struct Message{
    char cID;
    uint16 iMessage;
};    

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

uint16_t vBitShiftForMessage(uint8 command[2])
{
    uint16_t buff;
    buff = (uint16_t) ((((uint16_t) command[0] ) << 8 )|
                        ((uint16_t)  command[1]));
    return buff;
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
    
    /* Create queues for sending recieved alarms and alarm ACK from FPGA between functions in this file */
    xQueueRecievedAlarm = xQueueCreate(1, sizeof(struct Message));
    xQueueAlarmACK = xQueueCreate(1, sizeof(_Bool));
    xQueueWaitForACK = xQueueCreate(1, sizeof(_Bool));
    
    /* TEST QUEUES */
    //xQueueTestData = xQueueCreate(20, sizeof( struct Data ));
    //xQueueTestAlarm = xQueueCreate(1, sizeof(struct AlarmMessage));
    
    xTaskCreate(vTaskRecieveFromFPGA, "FPGA recieve", 100, NULL, 3, NULL);
//    xTaskCreate(vTaskSendDataRequest, "Send data request", 100, NULL, 2, NULL);
//    xTaskCreate(vTaskSendNewParams, "Send new params", 100, NULL, 2, NULL);
    xTaskCreate(vTaskSendToFPGA, "Send message to FPGA", 100, NULL, 3, NULL);
//    xTaskCreate(vTaskAlarmHandler, "alarm handling", 100, NULL, 1, NULL); //highest priority
    
    
    /*Initialize test tasks*/
    #if COMMUNICATIONTEST == 1
        vTestTaskComsInit();
    #endif
    
    }


  

/* This function consists of these steps: 
   - check if there has been sent an alarm, and if so, wait for ACK from FPGA before doing anything else
       - when ACK recieved, send to xQueueAlarmACK and resume normal function
   - wait for not empty Rx fifo
   - if first recieved byte is 0 (data request): 
       - Datarequest is set to true, and sent in xQueueRecievedDataRequest
   - if first recieved byte is 1 (new params):
       - save second recieved byte as sensor ID 
       - save third recieved byte as new value
       - send these to vTaskSendNewParams()
*/
//I dont really get the logic behind this module
void vTaskRecieveFromFPGA(){
    
    const TickType_t xDelayms = pdMS_TO_TICKS( 200 );
    uint16 i = 0;
    _Bool RecievedACK;
    _Bool SentAlarm;
    _Bool xStatus;
    struct Params RecievedParams; //for internally sending recieved params 
    struct Message ParamsForMess; //for sending param to NewParam task
    
    for(;;){
        
        vTaskDelay(xDelayms);
        xStatus = xQueueReceive(xQueueWaitForACK, &SentAlarm, 0); 
        
        if(xStatus == pdTRUE)
        {
            // UART_PutString("Recieve from FPGA: waiting for alarm ACK \n\n"); USED FOR TEST
            while(UART_GetRxBufferSize() != 1){
                //wait for ACK
            }    
            RecievedACK = UART_GetByte();
            //UART_PutString("Recieved ACK: "); USED FOR TEST
            //UART_PutChar(RecievedACK); USED FOR TEST
            //UART_PutString("\n\n"); USED FOR TEST
            xQueueSendToBack(xQueueAlarmACK, &RecievedACK, portMAX_DELAY);
        }    
        
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
        
                    
                        xQueueSendToBack(xQueueCentralrequest, &DataRequest, portMAX_DELAY);
                        
                        DataRequest = pdFALSE;
                        
                        break;
                    
                    case '1': //case with new param
                    
                        //UART_PutString("New param case \n"); //USED FOR TEST
                        RecievedParams.cID = UART_GetChar(); 
                        
                        while(i < 2){
                            RecievedParams.Value[i] = UART_GetChar();
                            i++;
                        }    
                        i = 0;
                        
                        ParamsForMess.cID = RecievedParams.cID;
                        ParamsForMess.iMessage = vBitShiftForMessage(RecievedParams.Value);
                        
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
                        
                        xQueueSendToBack(xQueueSendNewParams, &ParamsForMess, portMAX_DELAY);
                        
                        break;
                    
                }
                
            }
            
    }
    
}




/*
void vTaskSendDataRequest(){
    //extern QueueHandle_t xQueueCentralrequest; //only works after merge with data storage
    BaseType_t SendDataRequest;
    //struct Data Send;
    const TickType_t xDelayms = pdMS_TO_TICKS( 100 );
    
    for(;;)
    {
        xQueueReceive(xQueueRecievedDataRequest, &SendDataRequest, portMAX_DELAY);
        
        if(SendDataRequest == pdTRUE)
        {
            //xQueueSendToBack(xQueueCentralrequest, &SendDataRequest, portMAX_DELAY); //only works after merge with data storage
            
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
            
            xQueueSendToBack(xQueueTestData, (void *) &Send, portMAX_DELAY);
            
            
        }    
         USED FOR TEST
        else{    
             
            UART_PutString("\n");
            UART_PutString("Data request: FALSE \n");
        }        
    }    
    
}  

void vTaskSendNewParams(){
    struct Params SendParams;
    
    for(;;)
    {
        xQueueReceive(xQueueRecievedNewParams, &(SendParams), portMAX_DELAY);
        
         only works after merge with new params 
        //xQueueSendToBack(xQueueSendNewParams, (void *) &SendParams, portMAX_DELAY);
        
        
         USED FOR TEST
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
        
    }    
    
}
*/




void vTaskSendToFPGA(){
    const TickType_t bigDelay = pdMS_TO_TICKS( 500 ); 
    const TickType_t smallDelay = pdMS_TO_TICKS( 100 ); 
    struct Message SendAlarm;
    struct Data SendToCentral; 
    _Bool xStatus;
    _Bool yStatus;
    
    for(;;)
    {
        xStatus = xQueueReceive(xQueueAlarmForFPGA, &SendAlarm, portMAX_DELAY);
        if(xStatus == pdTRUE)
        {
            //UART_PutString("Send to FPGA: Alarm recieved \n"); USED FOR TEST
            //UART_PutString("ID: "); USED FOR TEST
            UART_PutChar(SendAlarm.cID);
            //UART_PutString(", message: "); USED FOR TEST
            vBitShifterUART(SendAlarm.iMessage);
            //UART_PutString("\n\n"); USED FOR TEST
            xQueueSendToBack(xQueueAlarmFromFPGA, &xStatus, portMAX_DELAY);
        }    
        
        //xQueueReceive(xQueueCentralData, &(SendToCentral), portMAX_DELAY); this queue is for real
        //xQueueReceive(xQueueTestData, &SendToCentral, portMAX_DELAY); //this Queue is for test
        
        //UART_PutString("Data recieved from data storage: \n"); //USED FOR TEST
        
        yStatus = xQueueReceive(xQueueCentralData, &SendToCentral, smallDelay);
        if (yStatus == pdTRUE)
        {
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
} 




/*
void vTaskAlarmHandler(){
    //extern QueueHandle_t xQueueAlarmForFPGA; //only works after merge with alarm task
    //extern QueueHandle_t xQueueAlarmFromFPGA; //only works after merge with alarm task
    
    struct AlarmMessage MessageForFPGA; 
    _Bool RecievedACK;
    _Bool xStatus;
    
    for(;;)
    {
        //xStatus = xQueueReceive(xQueueAlarmForFPGA, &MessageForFPGA, portMAX_DELAY);//only works after merge with alarm task
        //xStatus = xQueueReceive(xQueueTestAlarm, &MessageForFPGA, portMAX_DELAY); USED FOR TEST
        if(xStatus == pdTRUE)
        {
            //UART_PutString("Alarm Handler: Alarm recieved \n\n"); USED FOR TEST
            xQueueSendToBack(xQueueRecievedAlarm, &MessageForFPGA, portMAX_DELAY);
            xQueueReceive(xQueueAlarmACK, &RecievedACK, portMAX_DELAY);
            //UART_PutString("Alarm Handler: ACK recieved \n\n"); USED FOR TEST
            //xQueueSendToBack(xQueueAlarmFromFPGA, &RecievedACK, portMAX_DELAY);//only works after merge with alarm task
            
        }    
        
    }    
    
}    
*/



//////////////////////////////// Test Task ///////////////////////////////


void vTestTaskComsInit(){
    /* Alarm test task */
 //   xTaskCreate(vTaskTestComAlarm, "alarm test", 100, NULL, 2, NULL);
    
}    

void vTaskTestComAlarm(){
    const TickType_t xDelayms = pdMS_TO_TICKS( 10000 );
    
    struct Message TestAlarm;
    TestAlarm.cID = 't';
    TestAlarm.iMessage = 50;
    
    for(;;)
    {
        UART_PutString("Test alarm: \n");
        UART_PutString("ID: ");
        UART_PutChar(TestAlarm.cID);
        UART_PutString(", message: ");
        vBitShifterUART(TestAlarm.iMessage);
        UART_PutString("\n\n");
        //xQueueSendToBack(xQueueTestAlarm, &TestAlarm, portMAX_DELAY);
        vTaskDelay(xDelayms);
        
        
    }    
    
}    