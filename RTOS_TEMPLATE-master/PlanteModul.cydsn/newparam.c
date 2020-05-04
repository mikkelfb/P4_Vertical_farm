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


/*--------------------------*/

/*This handler is entirely for testing purposes*/
QueueHandle_t xQueueTestNewParam;

void vNewparamInit(){
    
    //Declare Queues here here:

    xQueueLightHandler = xQueueCreate( 1, sizeof( int ) );
    xQueueNutrientsHandler = xQueueCreate(1, sizeof ( int ) );
    /*--------------------------*/
    
    /*This queue is entirely for testing purposes*/
    xQueueTestNewParam = xQueueCreate(3, sizeof (uint16) );
    
    
    #if NUTRIENTSTEST == 1
        vTestTaskInit();
    #endif
}

/*This task is entirely for testing purposes. It just sends new parameters, in much the same way that the 
communication task eventually will: As an array, where the index0 is an identifyer
In this task it will be assumed that the identifyer makes 1 = pH, 2 = light and 3 = EC*/
void vComsFromOtherTask(){
    uint16 uTestParams[10] = {2, 8, 16};
    xQueueSendToBack(xQueueTestNewParam, uTestParams, portMAX_DELAY);

}

void vTaskNewParam(){
    uint16 uNewParams[10];
    xQueueReceive( xQueueTestNewParam, uNewParams, portMAX_DELAY);
    
    
    
}


