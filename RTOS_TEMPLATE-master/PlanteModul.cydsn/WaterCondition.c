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
#include "FreeRTOS.h"
#include "WaterCondition.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

SemaphoreHandle_t waterSemph;
QueueHandle_t WaterPumpQueue;

_Bool WaterState;



void initWaterCondition(){    
    xTaskCreate(vTaskFlowWater , "Flow water", 100 , NULL , 2 , NULL);
    xTaskCreate(vTaskWaterPump , "Water pump", 100 , NULL , 2 , NULL);
    WaterPumpQueue = xQueueCreate(1 , sizeof(_Bool));
    waterSemph = xSemaphoreCreateBinary();
}


void vTaskFlowWater(){
    const TickType_t xDelayms = pdMS_TO_TICKS( 2000 ); // Sets the measurement resolution.
    uint8 flowWaterResult;
    _Bool state = 0; //state for detecting 
    for(;;){
        if(state == 0 ){
            ADC_Flow_StartConvert();
            state = 1;
        }
        else{
            flowWaterResult = ADC_Flow_GetResult8(); 
            SW_UART_TEST_USB_PutHexByte(flowWaterResult);
            if(flowWaterResult < 50){ //No water 
                WaterState = 0;
                
            }
            else{
                WaterState = 1;   
            }
            state = 0;
            xSemaphoreGive(waterSemph); //gives semaphore as WaterState has updated and controll task should run
            vTaskDelay(xDelayms);
        }
    }
}

void vTaskWaterPump(){
    _Bool bPumpState = 1;
    for(;;){
        xQueueReceive( WaterPumpQueue , &bPumpState , portMAX_DELAY); //sends task to blocked state until new Waterpump setting is comming
        if(bPumpState == 1){
            Pin_WaterPump_Write(1); //sets output pin for waterpump to high
        }
        else if(bPumpState == 0){
            Pin_WaterPump_Write(0); //sets output pin for waterpump to low
        }
    }
}

void vTaskWaterConditionController(){
    for(;;){
        
        xSemaphoreTake(waterSemph , portMAX_DELAY);
        
        if(WaterState == 1){
            //generate package to datalager
            //do nothing everything is fine
        }
        else{
            //Something wrong no water in pipes
            _Bool bNewPumpState = 0;
            xQueueSend(WaterPumpQueue, &bNewPumpState , portMAX_DELAY);
            //make alarm to alarm handler task
        }
    }
}
