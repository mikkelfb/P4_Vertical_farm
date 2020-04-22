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

const int iLightArrayle = 5;
uint8 Light[5]; // variable contains measured light value

QueueHandle_t xQueueLightValue;

void vLightInit(){
    
    xQueueLightValue = xQueueCreate(1, sizeof(uint8));
    
    xTaskCreate(vTaskLightMeasure, "Light", 1000 , NULL , 2 , NULL);
    
    /*Initialize test tasks*/
    #if LIGHTTEST == 1
        vTestLightTaskInit();
    #endif
}


void vTaskLightMeasure(){
    Pin_5V_out_Write(1u);
    const TickType_t xDelayms = pdMS_TO_TICKS( 10000 ); // Sets the measurement resolution.
    
    for(;;){
        for(int i = 0;i < iLightArrayle;i++){
            uint8 LightRead = Pin_LIGHT_in_Read();
            Light[i] = LightRead;
            xQueueSendToBack(xQueueLightValue, &Light[i], portMAX_DELAY);
            vTaskDelay(xDelayms);
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