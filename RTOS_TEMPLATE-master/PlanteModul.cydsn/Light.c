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
uint8 ClockAddr = 9; // Address for Arduino clock


struct Clock
    {
        uint8 Sec;
        uint8 Min;
        uint8 Hour;
        uint8 DayOfMonth;
        uint8 Month;
        uint16 Year;
        
    };
struct Clock Start;
struct Clock CurrentTime;

/* Create a queue for sending the light value through UART */
QueueHandle_t xQueueLightValue;

/* Create a queue for checking the light cycle */
QueueHandle_t xQueueCurrentTime[3];

void vLightInit(){
       
    
    /*  The queue is created to hold a maximum of 1 value, each of which is
        large enough to hold a variable at the size of uint8. */
    xQueueLightValue = xQueueCreate(1, sizeof(uint8));
    
    /*  Queue with current time: hour, min, sec */
    xQueueCurrentTime[0] = xQueueCreate(1, sizeof(uint8));
    xQueueCurrentTime[1] = xQueueCreate(1, sizeof(uint8));
    xQueueCurrentTime[2] = xQueueCreate(1, sizeof(uint8));
    
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
    uint8 LightCycle[] = {8, 13}; // Input parameters with start and stop time for the light cycle
    const TickType_t xDelayms = pdMS_TO_TICKS( 10000 ); // Sets the measurement resolution.
    const TickType_t xShortDelayms = pdMS_TO_TICKS( 100 );
    char ArduinoClock[3]; // Array to store clock value from Arduino
    
    for(;;){
              
        /* Read current time from Arduino clock and store in array */
        I2C_MasterReadBuf( ClockAddr, (uint8*) ArduinoClock, 3, I2C_MODE_COMPLETE_XFER);
        
        /* Save values from array in CurrentTime struct */
        CurrentTime.Hour = ArduinoClock[0];
        CurrentTime.Min = ArduinoClock[1];
        CurrentTime.Sec = ArduinoClock[2];
        
        /* Sends CurrentTime values to serial via UART, used for test */
        xQueueSendToBack(xQueueCurrentTime[0], &CurrentTime.Hour, portMAX_DELAY);
        xQueueSendToBack(xQueueCurrentTime[1], &CurrentTime.Min, portMAX_DELAY);
        xQueueSendToBack(xQueueCurrentTime[2], &CurrentTime.Sec, portMAX_DELAY);
        vTaskDelay(xShortDelayms);
                
        /*  This if-statement checks if the current hour is within the on-interval of the light cycle.
            If so, the LED is turned on. 
            After a short delay, the light sensor is read to make sure that the LED is on */
        if((CurrentTime.Hour >= LightCycle[0]) && (CurrentTime.Hour < LightCycle[1])){
            
            // some code that turns on the LED
            bLEDState = 1;
            SW_UART_TEST_USB_PutString("Within active hours of light cycle: TRUE \n");
            vTaskDelay(xShortDelayms);
            
            if(Light == 0){
                // The LED are on, all is good
                SW_UART_TEST_USB_PutString("Enough light: TRUE \n \n");
            }
            else if(Light == 1){
                // The LED are not on, all is not good
                SW_UART_TEST_USB_PutString("Enough light: FALSE \n \n");
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
            
            SW_UART_TEST_USB_PutString("Within active hours of light cycle: FALSE \n \n");
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

/* Initialize test tasks*/
void vTestLightTaskInit(){
    xTaskCreate(vTestLightTask, "TestLight", 1000, NULL, 2, NULL);
}    

/* Function tests vTaskLightMeasure() by reading from the queue and sending to serial monitor */
void vTestLightTask(){
    //uint8 TestLightValue;
    uint8 TestCurrentHour;
    uint8 TestCurrentMin;
    uint8 TestCurrentSec;
    for(;;){
        //xQueueReceive(xQueueLightValue, &TestLightValue, portMAX_DELAY);
        xQueueReceive(xQueueCurrentTime[0], &TestCurrentHour, portMAX_DELAY);
        xQueueReceive(xQueueCurrentTime[1], &TestCurrentMin, portMAX_DELAY);
        xQueueReceive(xQueueCurrentTime[2], &TestCurrentSec, portMAX_DELAY);
        //SW_UART_TEST_USB_PutString("Light Value: ");
        //SW_UART_TEST_USB_PutHexInt(TestLightValue);
        //SW_UART_TEST_USB_PutString("Current Hour: ");
        //SW_UART_TEST_USB_PutHexByte(TestCurrentHour);
        //SW_UART_TEST_USB_PutString("\n");
        
        SW_UART_TEST_USB_PutString("Current time: ");
        SW_UART_TEST_USB_PutHexByte(TestCurrentHour);
        SW_UART_TEST_USB_PutString(":");
        SW_UART_TEST_USB_PutHexByte(TestCurrentMin);
        SW_UART_TEST_USB_PutString(":");
        SW_UART_TEST_USB_PutHexByte(TestCurrentSec);
        SW_UART_TEST_USB_PutString("\n");
    }    
}