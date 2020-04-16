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
    xTaskCreate(vTaskNutrientPump, "Pump 1", 100, NULL, 2, NULL); 
    
    /*Initialize test tasks*/
    #if NUTRIENTSTEST == 1
        vTestTaskInit();
    #endif
    
    
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





/* --- TEST TASK --- */

/*
    Task for testing existing implementations.

    DO NOT delete this functions/tasks, but comment them out if they arn't nexessary 
    for your current implementation/testing
    
    All test task/functions, must start with (type)Test(Task/Function/Var)(Name)

    
*/

void vTestTaskInit(){
    xTaskCreate(vTestTaskNutrientPump, "Test Pump 1", 100, NULL, 1, NULL); 
}


void vTestTaskNutrientPump() {
    _Bool bTestState = 1;
    const TickType_t xDelay500ms = pdMS_TO_TICKS( 500 );
    
    for(;;) {
        if(bTestState == 1)
            bTestState = 0;
        else
            bTestState = 1;
        
        xQueueSendToBack(xQueueNutrientPump, &bTestState, portMAX_DELAY);
        
        vTaskDelay(xDelay500ms);
    }
    

}
