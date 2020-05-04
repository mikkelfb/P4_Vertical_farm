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
#include <stdio.h>
#include <stdlib.h>

//Declare handlers here:
QueueHandle_t xQueuetesthandler;


/*--------------------------*/

void vNewparamInit(){
    xQueuetesthandler = xQueueCreate( 1, sizeof( int ) );
    
    
    
    #if NUTRIENTSTEST == 1
        vTestTaskInit();
    #endif
}




void vTaskTestHandlers(){
    int x = 7;
    const TickType_t xDelayms = pdMS_TO_TICKS( 7000 );
    for(;;){
        xQueueSend ( xQueuetesthandler, &x, portMAX_DELAY ); 
        vTaskDelay(xDelayms);
    }
}