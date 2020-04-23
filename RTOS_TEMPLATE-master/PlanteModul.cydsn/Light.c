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
uint8 Time;
_Bool bLEDState;
_Bool bAlarmState = 0;

/* Create a queue for sending the light value through UART */
QueueHandle_t xQueueLightValue;

/* Create a queue for checking the light cycle */
QueueHandle_t xQueueCurrentHour;

void vLightInit(){
    
    struct Start
    {
        uint8 Sec;
        uint8 Min;
        uint8 Hour;
        uint8 DayOfMonth;
        uint8 Month;
        uint16 Year;
        
    };   
    
    RTC_TIME_DATE Start;
    
    /* Fill struct with date and time */
    Start.Sec = 55u;
    Start.Min = 59u;
    Start.Hour = 14u;
    Start.DayOfMonth = 31u;
    Start.Month = 12u;
    Start.Year = 2007u;
    
    RTC_WriteTime(&Start);
    
    RTC_Start();
    
    
    /*  The queue is created to hold a maximum of 1 value, each of which is
        large enough to hold a variable at the size of uint8. */
    xQueueLightValue = xQueueCreate(1, sizeof(uint8));
    
    xQueueCurrentHour = xQueueCreate(1, sizeof(uint8));
    
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
        //xQueueSendToBack(xQueueLightValue, &Light, portMAX_DELAY);
        vTaskDelay(xDelayms); 
    }
}


/*  This function recieves info about which time interval there should be light, 
    turns on/off LED lights and periodically checks if the lights are on */
void vTaskLightController(){
    uint8 LightCycle[] = {8, 16}; // Input parameters with start and stop time for the light cycle
    const TickType_t xDelayms = pdMS_TO_TICKS( 10000 ); // Sets the measurement resolution.
    const TickType_t xShortDelayms = pdMS_TO_TICKS( 500 );
    SW_UART_TEST_USB_PutString("before time read");
    SW_UART_TEST_USB_PutString("\n");
    
    for(;;){
        uint8 CurrentHour = RTC_currentTimeDate.Hour; // Reads the current hour from the internal RTC
        SW_UART_TEST_USB_PutString("time read");
        SW_UART_TEST_USB_PutString("\n");
        Time = CurrentHour;
        xQueueSendToBack(xQueueCurrentHour, &Time, portMAX_DELAY);
        vTaskDelay(xShortDelayms);
                
        /* This if-statement checks if the current hour is within the on-interval of the light cycle.
            If so, the LED is turned on. 
            After a short delay, the light sensor is read to make sure that the LED is on */
        if((CurrentHour >= LightCycle[0]) && (CurrentHour <= LightCycle[1])){
            
            // some code that turns on the LED
            bLEDState = 1;
            SW_UART_TEST_USB_PutString("time to light up! \n");
            vTaskDelay(xShortDelayms);
            
            if(Light == 0){
                // The LED are on, all is good
                SW_UART_TEST_USB_PutString("there is light, all is well! \n");
            }
            else if(Light == 1){
                // The LED are not on, all is not good
                SW_UART_TEST_USB_PutString("there is not light, all is not well! \n");
                // some code to force turn on LED and send alarm
            }
            else{
                //panic
            }    
        }
        else{
            // The LED should not be on
            bLEDState = 0;
            // some code to turn off LED
            
            SW_UART_TEST_USB_PutString("time to light down! \n");
        }    
    vTaskDelay(xDelayms); 
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
    //uint8 TestLightValue;
    uint8 TestCurrentHour;
    for(;;){
        //xQueueReceive(xQueueLightValue, &TestLightValue, portMAX_DELAY);
        xQueueReceive(xQueueCurrentHour, &TestCurrentHour, portMAX_DELAY);
        //SW_UART_TEST_USB_PutString("Light Value: ");
        //SW_UART_TEST_USB_PutHexInt(TestLightValue);
        SW_UART_TEST_USB_PutString("Current Hour: ");
        SW_UART_TEST_USB_PutHexByte(TestCurrentHour);
        SW_UART_TEST_USB_PutString("\n");
    }    
}