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


/*--------------------------*/

/*This handler is entirely for testing purposes*/
QueueHandle_t xQueueTestNewParam;

void vNewparamInit(){
    
    //Declare Queues here here:

    xQueueLightHandler = xQueueCreate( 1, sizeof( uint8 ) );
    xQueueNutrientsHandler = xQueueCreate(1, sizeof ( uint16 ) );
    /*--------------------------*/
    
    /*This queue is entirely for testing purposes*/
    xQueueTestNewParam = xQueueCreate(3, sizeof (uint16) );
    
    
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
    uint16 uTestParams[3] = {2, 8, 16};
    xQueueSendToBack(xQueueTestNewParam, uTestParams, portMAX_DELAY);

}
/* This task reads the incoming new params and determines which control task the new params should be sent to */
void vTaskNewParam(){
    uint16 uNewParams[3];               
    uint8 uNewLightParams[2];           // uNewLightParams[0] = start time, uNewLightParams[1] = stop time 
    uint16 uNewNutrientParams[2];
    xQueueReceive( xQueueTestNewParam, uNewParams, portMAX_DELAY);
    
    switch(uNewParams[0])
    {
        case 1:
            uNewNutrientParams[0] = uNewParams[1];
            uNewNutrientParams[1] = uNewParams[2];
            xQueueSendToBack(xQueueNutrientsHandler, uNewNutrientParams, portMAX_DELAY);
            
            break;
        
        case 2:
            uNewLightParams[0] = uNewParams[1];
            uNewLightParams[1] = uNewParams[2];
            xQueueSendToBack(xQueueLightHandler, uNewLightParams, portMAX_DELAY);
            
            break;
        
        
    }
    
}


