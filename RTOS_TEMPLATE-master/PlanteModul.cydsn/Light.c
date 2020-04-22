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

uint8 Light; // Variable contains measured light value

/* Create a queue for sending the light value through UART */
QueueHandle_t xQueueLightValue;

void vLightInit(){
    /*  The queue is created to hold a maximum of 1 value, each of which is
        large enough to hold a variable at the size of uint8. */
    xQueueLightValue = xQueueCreate(1, sizeof(uint8));
    
    
    /*  Create the task that will control the light sensor. The task is created with
        priority 2. */
    xTaskCreate(vTaskLightMeasure, "Light", 1000 , NULL , 2 , NULL);
    
    /*  Create the controller task that will recieve external commands 
        and regulate the LED based on the sensor value. 
        The task is created with priority 2. */
    xTaskCreate(vTaskLightController, "Light controller", 100, NULL, 2, NULL);
    
    /*Initialize test tasks*/
    #if LIGHTTEST == 1
        vTestLightTaskInit();
    #endif
}

/*  This function turns on the light sensor, periodically reads the sensor 
    and sends the measured value to the created queue for UART */
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

/*  This function recieves info about which time interval there should be light, 
    turns on/off LED lights and periodically checks if the lights are on */
void vTaskLightController(){
    uint8 LightCycle[] = {8, 16}; // Input parameters with start and stop time for the light cycle
    const TickType_t xDelayms = pdMS_TO_TICKS( 500 ); // Sets the measurement resolution.
    
    uint8 CurrentHour = RTC_ReadHour(); // Reads the current hour from the internal RTC
    
    /*  This if-statement checks if the current hour is within the on-interval of the light cycle.
        If so, the LED is turned on. 
        After a short delay, the light sensor is read to make sure that the LED is on */
    if((CurrentHour >= LightCycle[0]) && (CurrentHour <= LightCycle[1])){
        
        // some code that turns on the LED
        
        vTaskDelay(xDelayms);
        if(Light == 0){
            // The LED are on, all is good
        }
        else if(Light == 1){
            // The LED are not on, all is not good
            
            // some code to force turn on LED and send alarm
        }
        else{
            //panic
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

/*Initialize test tasks*/
void vTestLightTaskInit(){
    xTaskCreate(vTestLightTask, "TestLight", 1000, NULL, 2, NULL);
}    

/* Function tests vTaskLightMeasure() by reading from the queue and sending to serial monitor */
void vTestLightTask(){
    uint8 TestLightValue;
    for(;;){
        xQueueReceive(xQueueLightValue, &TestLightValue, portMAX_DELAY);
        SW_UART_TEST_USB_PutString("Light Value: ");
        SW_UART_TEST_USB_PutHexInt(TestLightValue);
        SW_UART_TEST_USB_PutString("\n");
    }    
}