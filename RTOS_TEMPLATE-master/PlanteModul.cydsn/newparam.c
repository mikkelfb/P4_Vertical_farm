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
#include "Shared_resources.c"



/*
uint16 uRecievedNewParams[3] = {0, 0, 0};               
uint8 uNewLightParams[2] = {1, 1};           // uNewLightParams[0] = start time, uNewLightParams[1] = stop time 
uint16 uNewNutrientParams[2] = {0, 0};
*/

// 'e' = EC, 'p' = pH, 't' = time
struct NewParameters{
    char cID;                   
    uint16_t fpHValue;
    uint16_t fECValue;
    uint8 uStartTime;
    uint8 uStopTime;
};

struct LightPackage{
    uint8 uStartTime;
    uint8 uStopTime;
};    

struct NutrientsPackage{
    char cID;
    uint16_t iNewValue;
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
    
    //xTaskCreate(vComsFromOtherTask, "coms from other task", 100, NULL, 2, NULL);
    xTaskCreate(vTaskNewParam, "new param task", 100, NULL, 3, NULL);
    
    
    #if NEWPARAMTEST == 1
        vInitTestParam();
    #endif
}




/* This task reads the incoming new params and determines which control task the new params should be sent to */
void vTaskNewParam(){
    struct NutrientsPackage input;
    struct LightPackage Light;
    struct NutrientsPackage Nutrients;
    
    for(;;)
    {
        xQueueReceive(xQueueSendNewParams, &input, portMAX_DELAY);
    
        switch(input.cID)
        {
            // If EC value
            case 'e':
                
                Nutrients.cID = input.cID;
                Nutrients.iNewValue = input.iNewValue;
                
                /*
                SW_UART_TEST_USB_PutString("\n ID sent: ");
                SW_UART_TEST_USB_PutChar(Nutrients.cID);
                SW_UART_TEST_USB_PutString("\n New EC value: ");
                SW_UART_TEST_USB_PutHexByte(Nutrients.fNewValue);
                SW_UART_TEST_USB_PutString("\n ");
                */    
                xQueueSendToBack(xQueueNutrientsHandler, &Nutrients, portMAX_DELAY);
           
                break;
        
            // If light cycle value
            case 't':
                
                Light.uStartTime = (input.iNewValue >> 8);
                Light.uStopTime  = input.iNewValue;
                /*
                SW_UART_TEST_USB_PutString("\n New start time: ");
                SW_UART_TEST_USB_PutHexByte(Light.uStartTime);
                SW_UART_TEST_USB_PutString("\n New stop time: ");
                SW_UART_TEST_USB_PutHexByte(Light.uStopTime);
                SW_UART_TEST_USB_PutString("\n ");
                */
                xQueueSendToBack(xQueueLightHandler, &Light, portMAX_DELAY);
                break;
                
            // If pH value
            case 'p':
                
                Nutrients.cID = input.cID;
                Nutrients.iNewValue = input.iNewValue;
                /*
                SW_UART_TEST_USB_PutString("\n ID sent: ");
                SW_UART_TEST_USB_PutChar(Nutrients.cID);
                SW_UART_TEST_USB_PutString("\n New pH value: ");
                SW_UART_TEST_USB_PutHexByte(Nutrients.fNewValue);
                SW_UART_TEST_USB_PutString("\n ");
                */
                
                xQueueSendToBack(xQueueNutrientsHandler, &Nutrients, portMAX_DELAY);
           
                break;
        
        }
        
    }
    
}




///////////////// Test tasks ////////////////////////////


void vInitTestParam()
{
//    vComsFromOtherTask();
}

    //This task is entirely for testing purposes. It just sends new parameters, in much the same way that the 
    //communication task eventually will: As a struct, where the first element is an identifier in the form of a character.
    //In this task it will be assumed that the identifier makes 'e' = EC, 't' = light, 'p' = pH

void vComsFromOtherTask(){
    const TickType_t xShortDelayms = pdMS_TO_TICKS( 100 );
    struct NewParameters TestFromCom;
    
    TestFromCom.cID = 't';
    TestFromCom.fECValue = 50;
    TestFromCom.fpHValue = 7;
    TestFromCom.uStartTime = 8;
    TestFromCom.uStopTime = 16;
    xQueueSendToBack(xQueueTestNewParam, (void *) &TestFromCom, portMAX_DELAY);
}


