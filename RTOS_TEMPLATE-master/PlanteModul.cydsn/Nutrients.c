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



QueueHandle_t xQueueNutrientPump[3];
QueueHandle_t xQueuePHValue;


const uint8 MAX_SPEED = 6;
const uint8 STOP_SPEED = 19;
static const uint8 pcTextForNutrientPump[3] = {0, 1, 2};


void vNutrientsInit() {
    /*  The queue is created to hold a maximum of 1 value, each of which is
        large enough to hold a variable at the size of Bool. */
    xQueueNutrientPump[0] = xQueueCreate( 1, sizeof( _Bool ) );
    xQueueNutrientPump[1] = xQueueCreate( 1, sizeof( _Bool ) );
    xQueueNutrientPump[2] = xQueueCreate( 1, sizeof( _Bool ) );
    
    xQueuePHValue = xQueueCreate(1 , sizeof(uint16));
    
    
    /*  Create the task that will control one nutrient pump. The task is created with
        priority 1. */
    xTaskCreate(vTaskNutrientPump, "Pump 1", 100, (void*)pcTextForNutrientPump, 2, NULL);
    xTaskCreate(vTaskNutrientPump, "Pump 2", 100, (void*)pcTextForNutrientPump + 1, 2, NULL);
    xTaskCreate(vTaskNutrientPump, "Pump 2", 100, (void*)pcTextForNutrientPump + 2, 2, NULL);
    
    xTaskCreate(vTaskMeasurePH, "PH", 100 , NULL , 2 , NULL);
    
    /*Initialize test tasks*/
    #if NUTRIENTSTEST == 1
        vTestTaskInit();
    #endif
    
    
}




void vTaskNutrientPump( void *pvParameters ) {
    _Bool bState;
    BaseType_t xStatus;
    uint8 *piNutrientPump;
    
    piNutrientPump = ( uint8 * ) pvParameters;
    
    for(;;) {
        switch (*piNutrientPump) {
            case 0:
                xStatus = xQueueReceive( xQueueNutrientPump[0], &bState, portMAX_DELAY );
        
                if (xStatus == pdPASS) {
                    if (bState) {
                        PWM_PERISTALTISK_1_WriteCompare1(MAX_SPEED);
                    } else {
                        PWM_PERISTALTISK_1_WriteCompare1(STOP_SPEED);
                    }
                }
                break;
            
            case 1:
                xStatus = xQueueReceive( xQueueNutrientPump[1], &bState, portMAX_DELAY );
        
                if (xStatus == pdPASS) {
                    if (bState) {
                        PWM_PERISTALTISK_1_WriteCompare2(MAX_SPEED);
                    } else {
                        PWM_PERISTALTISK_1_WriteCompare2(STOP_SPEED);
                    }
                }
                break;
                
            case 2:
                xStatus = xQueueReceive( xQueueNutrientPump[2], &bState, portMAX_DELAY );
        
                if (xStatus == pdPASS) {
                    if (bState) {
                        PWM_PERISTALTISK_2_WriteCompare(MAX_SPEED);
                    } else {
                        PWM_PERISTALTISK_2_WriteCompare(STOP_SPEED);
                    }
                }
                break;
        }
    }
}

void vTaskMeasurePH(){
    uint16 iPHValue;
    const TickType_t xDelayms = pdMS_TO_TICKS( 200 );
    _Bool bState = 0;
    
    
    for(;;){
        if(bState == 0){
            ADC_PH_StartConvert();
            bState = 1;
        }
        else if( (ADC_PH_IsEndConversion(ADC_PH_RETURN_STATUS) != 0) && (bState == 1) ){
            iPHValue = ADC_PH_GetResult16();
            xQueueSendToBack(xQueuePHValue , &iPHValue , portMAX_DELAY);
            bState = 0;
            vTaskDelay(xDelayms);
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
    xTaskCreate(vTestTaskUARTDataTransmit, "Test PH print", 100, NULL, 2, NULL); 
}


void vTestTaskUARTDataTransmit(){
    uint16 iPHValueForPrint;
    for(;;){
        xQueueReceive( xQueuePHValue, &iPHValueForPrint, portMAX_DELAY );
        SW_UART_TEST_USB_PutHexInt(iPHValueForPrint);
    }
}



void vTestTaskNutrientPump() {
    _Bool bTestState1 = 1;
    _Bool bTestState2 = 0;
    const TickType_t xDelayms = pdMS_TO_TICKS( 200 );
    
    for(;;) {
        if(bTestState1 == 1) {
            bTestState1 = 0;
            bTestState2 = 1;
        }
        else {
            bTestState1 = 1;
            bTestState2 = 0;
        }
        
        xQueueSendToBack(xQueueNutrientPump[0], &bTestState1, portMAX_DELAY);
        xQueueSendToBack(xQueueNutrientPump[1], &bTestState2, portMAX_DELAY);
        xQueueSendToBack(xQueueNutrientPump[2], &bTestState1, portMAX_DELAY);
        
        vTaskDelay(xDelayms);
    }
    

}
