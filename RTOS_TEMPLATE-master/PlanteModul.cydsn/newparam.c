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
#include "Nutrients.h"
#include "task.h"
#include "queue.h"
#include "newparam.h"
#include <stdio.h>
#include <stdlib.h>

//Declare handlers here:
QueueHandle_t xQueueLightHandler;
QueueHandle_t xQueueNutrientsHandler;

/*
uint16 uRecievedNewParams[3] = {0, 0, 0};               
uint8 uNewLightParams[2] = {1, 1};           // uNewLightParams[0] = start time, uNewLightParams[1] = stop time 
uint16 uNewNutrientParams[2] = {0, 0};
*/

// 'e' = EC, 'p' = pH, 't' = time
struct NewParameters{
    char cID;                   
    float fpHValue;
    float fECValue;
    uint8 uStartTime;
    uint8 uStopTime;
};

struct LightPackage{
    uint8 uStartTime;
    uint8 uStopTime;
};    

struct NutrientsPackage{
    char cID;
    float fNewValue;
};    

/*--------------------------*/

/*This handler is entirely for testing purposes*/
QueueHandle_t xQueueTestNewParam;

void vNewparamInit(){
    //SW_UART_TEST_USB_PutString("NewParamInit \n");
    //Declare Queues here here:

    xQueueLightHandler = xQueueCreate( 2, sizeof( struct LightPackage ) );
    xQueueNutrientsHandler = xQueueCreate(2, sizeof ( struct NutrientsPackage ) );
    /*--------------------------*/
    
    /*This queue is entirely for testing purposes*/
    xQueueTestNewParam = xQueueCreate(5, sizeof (struct NewParameters) );
    
    xTaskCreate(vComsFromOtherTask, "coms from other task", 100, NULL, 2, NULL);
    xTaskCreate(vTaskNewParam, "new param task", 100, NULL, 2, NULL);
    
    
    #if NUTRIENTSTEST == 1
        vTestTaskInit();
    #endif
}

/*This task is entirely for testing purposes. It just sends new parameters, in much the same way that the 
communication task eventually will: As a struct, where the first element is an identifier in the form of a character.
In this task it will be assumed that the identifier makes 'e' = EC, 't' = light, 'p' = pH
void vComsFromOtherTask( void *pvParameters){
    const TickType_t xShortDelayms = pdMS_TO_TICKS( 100 );
    struct NewParameters TestFromCom;
    
    TestFromCom.cID = 't';
    TestFromCom.fECValue = 50;
    TestFromCom.fpHValue = 7;
    TestFromCom.uStartTime = 8;
    TestFromCom.uStopTime = 16;
    xQueueSendToBack(xQueueTestNewParam, (void *) &TestFromCom, portMAX_DELAY);
}

*/
/* This task reads the incoming new params and determines which control task the new params should be sent to */
void vTaskNewParam( void *pvParameters ){
    struct NewParameters Temp;
    struct LightPackage Light;
    struct NutrientsPackage Nutrients;
    
    for(;;)
    {
        xQueueReceive(xQueueTestNewParam, &(Temp), portMAX_DELAY);
    
        switch(Temp.cID)
        {
            // If EC value
            case 'e':
                
                Nutrients.cID = Temp.cID;
                Nutrients.fNewValue = Temp.fECValue;
                
                /*
                SW_UART_TEST_USB_PutString("\n ID sent: ");
                SW_UART_TEST_USB_PutChar(Nutrients.cID);
                SW_UART_TEST_USB_PutString("\n New EC value: ");
                SW_UART_TEST_USB_PutHexByte(Nutrients.fNewValue);
                SW_UART_TEST_USB_PutString("\n ");
                */    
                xQueueSendToBack(xQueueNutrientsHandler, (void *) &Nutrients, portMAX_DELAY);
           
                break;
        
            // If light cycle value
            case 't':
                
                Light.uStartTime = Temp.uStartTime;
                Light.uStopTime = Temp.uStopTime;
                /*
                SW_UART_TEST_USB_PutString("\n New start time: ");
                SW_UART_TEST_USB_PutHexByte(Light.uStartTime);
                SW_UART_TEST_USB_PutString("\n New stop time: ");
                SW_UART_TEST_USB_PutHexByte(Light.uStopTime);
                SW_UART_TEST_USB_PutString("\n ");
                */
                xQueueSendToBack(xQueueLightHandler, (void *) &Light, portMAX_DELAY);
                break;
                
            // If pH value
            case 'p':
                
                Nutrients.cID = Temp.cID;
                Nutrients.fNewValue = Temp.fpHValue;
                /*
                SW_UART_TEST_USB_PutString("\n ID sent: ");
                SW_UART_TEST_USB_PutChar(Nutrients.cID);
                SW_UART_TEST_USB_PutString("\n New pH value: ");
                SW_UART_TEST_USB_PutHexByte(Nutrients.fNewValue);
                SW_UART_TEST_USB_PutString("\n ");
                */
                
                xQueueSendToBack(xQueueNutrientsHandler, (void *) &Nutrients, portMAX_DELAY);
           
                break;
        
        }
        
    }
    
}


