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

QueueHandle_t xQueueNutrientPump;

const uint8 MAX_SPEED = 6;
const uint8 STOP_SPEED = 19;


void vNutrientsInit() {
    /*  The queue is created to hold a maximum of 1 value, each of which is
        large enough to hold a variable at the size of Bool. */
    xQueueNutrientPump = xQueueCreate( 1, sizeof( _Bool ) );
    
    /*  Create the task that will control one nutrient pump. The task is created with
        priority 1. */
    xTaskCreate(vTaskNutrientPump, "Pump 1", 100, NULL, 1, NULL); 
    
}




void vTaskNutrientPump() {
    _Bool bState;
    BaseType_t xStatus;
    
    for(;;) {
        xStatus = xQueueReceive( xQueueNutrientPump, &bState, portMAX_DELAY );
        
        if (xStatus == pdPASS) {
            if (bState) {
                PWM_PERISTALTISK_WriteCompare1(MAX_SPEED);
            } else {
                PWM_PERISTALTISK_WriteCompare1(STOP_SPEED);
            }
        }
        
    }
   
    
}

void vTestTaskNutrientPump() {
    

}
