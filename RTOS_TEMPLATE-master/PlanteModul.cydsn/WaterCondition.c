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

_Bool WaterState; //Variable to determine wheter or not there is water in the pibes. Is the value which should be send to datalager

void vInitWaterCondition(){    
    //Craete the 3 tasks
    xTaskCreate(vTaskFlowWater , "Flow water", 50 , NULL , 2 , NULL);
    xTaskCreate(vTaskWaterPump , "Water pump", 50 , NULL , 2 , NULL);
    xTaskCreate(vTaskWaterConditionController , "Water Condition controller", 50 , NULL , 2 , NULL);
    
    //Create queue for controller task to send Waterpump task a message wether it should run or not.
    WaterPumpQueue = xQueueCreate(1 , sizeof(_Bool));
    
    //Create semphore to lock controller task until FlowWater have made a measurement
    waterSemph = xSemaphoreCreateBinary();
}


void vTaskFlowWater(){
    const TickType_t xDelayms = pdMS_TO_TICKS( 2000 ); // Sets the amount of measure ments per second
    uint8 flowWaterResult;
    _Bool state = 0; //state for detecting 
    for(;;){
        if(state == 0 ){
            ADC_Flow_StartConvert(); //Start ACD convertion
            state = 1;
        }
        else if(ADC_Flow_IsEndConversion(ADC_Flow_RETURN_STATUS) && state == 1){ //Test if ADC convertion is done
            flowWaterResult = ADC_Flow_GetResult8(); //Get ADC result
            //SW_UART_TEST_USB_PutHexByte(flowWaterResult); //test to see what sensor show should be commented out in realese
            if(flowWaterResult < 50){ //If sensor gives a value under 50 = no water in pipes
                WaterState = 0; //Set state 0 as th ere is no water in pipes
            }
            else{
                WaterState = 1; //set state 1 as there is water in pipes
            }
            state = 0; //reset intern state so task is ready to make new measurement
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
        SW_UART_TEST_USB_PutString("C task");
        
        if(WaterState == 1){
            //generate package to datalager
            //do nothing everything is fine
            _Bool bNewPumpState = 1;
            xQueueSend(WaterPumpQueue, &bNewPumpState , portMAX_DELAY);
        }
        else{
            //Something wrong no water in pipes
            _Bool bNewPumpState = 0;
            xQueueSend(WaterPumpQueue, &bNewPumpState , portMAX_DELAY);
            //make alarm to alarm handler task
        }
    }
}
