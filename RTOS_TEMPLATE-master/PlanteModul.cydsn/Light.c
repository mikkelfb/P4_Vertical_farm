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
#include "light.h"
#include "task.h"
#include "queue.h"

uint8 Light; // variable contains measured light value

QueueHandle_t xQueueLightValue;

void vLightInit(){
    
    xQueueLightValue = xQueueCreate(1, sizeof(uint8));
    
    xTaskCreate(vTaskLightMeasure, "Light", 1000 , NULL , 2 , NULL);
    xTaskCreate(vTaskLightController, "Light controller", 100, NULL, 2, NULL);
    
    /*Initialize test tasks*/
    #if LIGHTTEST == 1
        vTestLightTaskInit();
    #endif
}


void vTaskLightMeasure(){
    Pin_5V_out_Write(1u);
    const TickType_t xDelayms = pdMS_TO_TICKS( 10000 ); // Sets the measurement resolution.
    
    for(;;){
       
        uint8 LightRead = Pin_LIGHT_in_Read();
        Light = LightRead;
        xQueueSendToBack(xQueueLightValue, &Light, portMAX_DELAY);
        vTaskDelay(xDelayms); 
    }
}

// this function recieves info about which time interval there should be light, turns on/off LED lights and 
// periodically checks if the lights are on
void vTaskLightController(){
    uint8 LightCycle[] = {8, 16}; //input parameters with start and stop time for the light cycle
    const TickType_t xDelayms = pdMS_TO_TICKS( 500 ); // Sets the measurement resolution.
    
    uint8 CurrentHour = RTC_ReadHour();
    
    if((CurrentHour >= LightCycle[0]) && (CurrentHour <= LightCycle[1])){
        // some code that turns on the LED
        
        vTaskDelay(xDelayms);
        if(Light == 0){
            // all is good
        }
        else if(Light == 1){
            //all is not good
            //force turn on LED and send alarm
        }
        else{
            //panic
        }    
    }
    
}


void vTestLightTaskInit(){
    xTaskCreate(vTestLightTask, "TestLight", 1000, NULL, 2, NULL);
}    

void vTestLightTask(){
    uint8 TestLightValue;
    for(;;){
        xQueueReceive(xQueueLightValue, &TestLightValue, portMAX_DELAY);
        SW_UART_TEST_USB_PutString("Light Value: ");
        SW_UART_TEST_USB_PutHexInt(TestLightValue);
        SW_UART_TEST_USB_PutString("\n");
    }    
}