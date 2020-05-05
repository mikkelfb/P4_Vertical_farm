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

/* 
    OBS OBS

    It is assumed that every time a nutrient param is changed and sent from the FPGA, all of the
    nutrient params are sent. 
    
    Eg:
    In case of the current params:
    EC: 500
    pH: 8
    
    The user wishes to change the pH param to 7, so the following params are sent:
    EC: 500
    pH: 7

*/

/* [] END OF FILE */

#include "FreeRTOS.h"
#include "Nutrients.h"
#include "task.h"
#include "queue.h"
#include "newparam.h"
#include <stdio.h>
#include <stdlib.h>

//Declare handlers here:
QueueHandle_t xQueueLightHandler;
QueueHandle_t xQueueNutrientsHandler;

uint16 uRecievedNewParams[3] = {0, 0, 0};               
uint8 uNewLightParams[2] = {1, 1};           // uNewLightParams[0] = start time, uNewLightParams[1] = stop time 
uint16 uNewNutrientParams[2] = {0, 0};

struct NewParameters{
    char cID;                   // 'e' = EC, 'p' = pH, 't' = time
    float fpHValue;
    float fECValue;
    uint8 uStartTime;
    uint8 uStopTime;
};
struct NewParameters TestFromCom;

/*--------------------------*/

/*This handler is entirely for testing purposes*/
QueueHandle_t xQueueTestNewParam;

void vNewparamInit(){
    //SW_UART_TEST_USB_PutString("NewParamInit \n");
    //Declare Queues here here:

    xQueueLightHandler = xQueueCreate( 1, sizeof( uint8 ) );
    //xQueueLightHandler[1] = xQueueCreate( 1, sizeof( uint8 ) );
    xQueueNutrientsHandler = xQueueCreate(1, sizeof ( float ) );
    /*--------------------------*/
    
    /*This queue is entirely for testing purposes*/
    xQueueTestNewParam = xQueueCreate(3, sizeof (struct TestFromCom *) );
    //xQueueTestNewParam[1] = xQueueCreate(3, sizeof (uint16) );
    //xQueueTestNewParam[2] = xQueueCreate(3, sizeof (uint16) );
    
    
    xTaskCreate(vComsFromOtherTask, "coms from other task", 100, NULL, 2, NULL);
    xTaskCreate(vTaskNewParam, "new param task", 100, NULL, 2, NULL);
    
    
    #if NUTRIENTSTEST == 1
        vTestTaskInit();
    #endif
}

/*This task is entirely for testing purposes. It just sends new parameters, in much the same way that the 
communication task eventually will: As an array, where the index0 is an identifyer
In this task it will be assumed that the identifyer makes 1 = pH/EC, 2 = light*/
void vComsFromOtherTask(){
    const TickType_t xShortDelayms = pdMS_TO_TICKS( 100 );
    
    //struct NewParameters TestFromCom;
    
    TestFromCom.cID = 't';
    TestFromCom.fECValue = 50;
    TestFromCom.fpHValue = 7;
    TestFromCom.uStartTime = 8;
    TestFromCom.uStopTime = 16;
    
    SW_UART_TEST_USB_PutChar(TestFromCom.cID);
    SW_UART_TEST_USB_PutString("\n ");
    SW_UART_TEST_USB_PutHexByte(TestFromCom.fECValue);
    SW_UART_TEST_USB_PutString("\n ");
    SW_UART_TEST_USB_PutHexByte(TestFromCom.fpHValue);
    SW_UART_TEST_USB_PutString("\n ");
    SW_UART_TEST_USB_PutHexByte(TestFromCom.uStartTime);
    SW_UART_TEST_USB_PutString("\n ");
    SW_UART_TEST_USB_PutHexByte(TestFromCom.uStopTime);
    SW_UART_TEST_USB_PutString("\n ");
        
    
    xQueueSendToBack(xQueueTestNewParam, &TestFromCom, portMAX_DELAY);
    
    /*uint16 uTestParams[3] = {2, 8, 16};
    xQueueSendToBack(xQueueTestNewParam[0], &uTestParams[0], portMAX_DELAY);
    xQueueSendToBack(xQueueTestNewParam[1], &uTestParams[1], portMAX_DELAY);
    xQueueSendToBack(xQueueTestNewParam[2], &uTestParams[2], portMAX_DELAY);*/
    vTaskDelay(xShortDelayms);
    //SW_UART_TEST_USB_PutString("test of task \n");
    vTaskDelay(xShortDelayms);

}
/* This task reads the incoming new params and determines which control task the new params should be sent to */
void vTaskNewParam(){
    //const TickType_t xShortDelayms = pdMS_TO_TICKS( 100 );
    //SW_UART_TEST_USB_PutString("test of TaskNewParam \n");
    //vTaskDelay(xShortDelayms);
    
    for(;;)
    {
        struct NewParameters Temp;
        xQueueReceive(xQueueTestNewParam, &Temp, portMAX_DELAY);
        //xQueueReceive(xQueueTestNewParam[1], &uRecievedNewParams[1], portMAX_DELAY);
        //xQueueReceive(xQueueTestNewParam[2], &uRecievedNewParams[2], portMAX_DELAY);
        //SW_UART_TEST_USB_PutString("new params recieved \n");
        
        /*SW_UART_TEST_USB_PutChar(Temp.cID);
        SW_UART_TEST_USB_PutString("\n ");
        SW_UART_TEST_USB_PutHexByte(Temp.fECValue);
        SW_UART_TEST_USB_PutString("\n ");
        SW_UART_TEST_USB_PutHexByte(Temp.fpHValue);
        SW_UART_TEST_USB_PutString("\n ");
        SW_UART_TEST_USB_PutHexByte(Temp.uStartTime);
        SW_UART_TEST_USB_PutString("\n ");
        SW_UART_TEST_USB_PutHexByte(Temp.uStopTime);
        SW_UART_TEST_USB_PutString("\n ");*/
        
        switch(Temp.cID)
        {
            case 1:
                uNewNutrientParams[0] = uRecievedNewParams[1];
                uNewNutrientParams[1] = uRecievedNewParams[2];
                xQueueSendToBack(xQueueNutrientsHandler, uNewNutrientParams, portMAX_DELAY);
            
                break;
        
            case 2:
                uNewLightParams[0] = uRecievedNewParams[1];
                uNewLightParams[1] = uRecievedNewParams[2];
                xQueueSendToBack(xQueueLightHandler, &uNewLightParams[0], portMAX_DELAY);
                xQueueSendToBack(xQueueLightHandler, &uNewLightParams[1], portMAX_DELAY);
                //SW_UART_TEST_USB_PutString("new params sent \n");
                /*SW_UART_TEST_USB_PutHexByte(uNewLightParams[0]);
                SW_UART_TEST_USB_PutString(" ");
                SW_UART_TEST_USB_PutHexByte(uNewLightParams[1]);
                SW_UART_TEST_USB_PutString("\n ");*/
                break;
        }
        
    }
    
}


