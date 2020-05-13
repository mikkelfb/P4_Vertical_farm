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

/*  The LED and light sensor are controlled like so: 

        vTaskLightMeasure():
        - The output value of the light sensor is read periodically every 10 s
        - The value is sent to the queue xQueueLightValue
        
        vTaskLightController();
        - The struct LightCycle contains two hour values, which are the start and stop of the interval
          where the LED should be on. These values are recieved from a queue from newparam.c
        - The current hour, minute, sec is read from I2C and stored in the struct CurrentTime
        - If CurrentTime.Hour is inbetween the two values in LightCycle, bLEDcmd is set HIGH 
          and sent to the queue xQueueLEDCmd
          Then after a short delay, the light sensor value is read from xQueueLightValue
          If the light value is 1 (not measuring enough light), bAlarmState is set to HIGH
        - If CurrentTime.Hour is NOT inbetween the two values in LightCycle, bLEDcmd is set LOW 
          and sent to the queue xQueueLEDCmd
        - Delay of 10 s
        
        vTaskLEDcontrol():
        - Recieve values from the queues xQueueLightValue and xQueueLEDCmd
        - If bLEDcmd is HIGH and the light value is 1, the LED are turned on
        - If bLEDcmd is LOW, the LED are turned off

*/

#include "FreeRTOS.h"
#include "light.h"
#include "task.h"
#include "queue.h"

uint8 Light;                       // Variable contains measured light value //*** Se overvejelser med hensyn til denne variabel
_Bool bLEDcmd;                     // Variable describing if the LED should be on/off
_Bool bAlarmState = 0;             // Temporary values until Alarm task is set up
_Bool bAlarmACK = 0;               // Temporary values until Alarm task is set up
uint8 ClockAddr = 9;               // Address for Arduino clock

/* Create a struct for storing the light cycle */
struct TimeInterval{
    uint8 Start;
    uint8 Stop;
};
struct TimeInterval LightCycle;
    
/* Create a struct for storing the current time */
struct Clock{
    uint8 Sec;
    uint8 Min;
    uint8 Hour;
    uint8 DayOfMonth;
    uint8 Month;
    uint16 Year;
};

struct Clock CurrentTime;

/* Create a queue for sending the light value through UART */
QueueHandle_t xQueueLightValue;
    
/* Create a queue for sending the on/off command to LED */
QueueHandle_t xQueueLEDCmd;

/* Create a queue for checking the light cycle */
QueueHandle_t xQueueCurrentTime[3];

void vLightInit(){
           
    /*  The queue is created to hold a maximum of 1 value, each of which is
        large enough to hold a variable at the size of uint8. */
    xQueueLightValue = xQueueCreate(1, sizeof(uint8));
    
    /*  The queue is created to hold a maximum of 1 value at the size of a Bool. */
    xQueueLEDCmd = xQueueCreate(1, sizeof(_Bool));
    
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
    
    /*  Create the LED task to turn on and off the LED */
    xTaskCreate(vTaskLEDcontrol, "LED controller", 100, NULL, 2, NULL);
    
    /*Initialize test tasks*/
    #if LIGHTTEST == 1
        //vTestLightTaskInit();
    #endif
}

/*  This function turns on the light sensor, periodically reads the sensor 
    and sends the measured value to the created queue for UART */
void vTaskLightMeasure(){
    const TickType_t xDelayms = pdMS_TO_TICKS( 10000 ); // Sets the measurement resolution.
    //*** uint8 LightRead;
    for(;;){
        Light = Pin_LIGHT_in_Read();
        vTaskDelay(xDelayms); 
    }
}


/*  This function recieves info about which time interval there should be light, 
    turns on/off LED lights and periodically checks if the lights are on */
void vTaskLightController( void *pvParameters ){
    extern QueueHandle_t xQueueLightHandler;
    
    const TickType_t xDelayms = pdMS_TO_TICKS( 10000 ); // Sets the measurement resolution.
    const TickType_t xShortDelayms = pdMS_TO_TICKS( 100 );
    
    /*
    SW_UART_TEST_USB_PutString("\n ");
    SW_UART_TEST_USB_PutString("Current light cycle values: ");
    SW_UART_TEST_USB_PutHexByte(LightCycle.Start);
    SW_UART_TEST_USB_PutString(" ");
    SW_UART_TEST_USB_PutHexByte(LightCycle.Stop);
    SW_UART_TEST_USB_PutString("\n ");
    */
    xQueueReceive(xQueueLightHandler, &(LightCycle), 0); // If queue is empty, return immediately
    /*
    SW_UART_TEST_USB_PutString("\n ");
    SW_UART_TEST_USB_PutString("Updated light cycle values: ");
    SW_UART_TEST_USB_PutHexByte(LightCycle.Start);
    SW_UART_TEST_USB_PutString(" ");
    SW_UART_TEST_USB_PutHexByte(LightCycle.Stop);
    SW_UART_TEST_USB_PutString("\n ");
    */    
    char ArduinoClock[3]; // Array to store clock value from Arduino
    
    for(;;){
              
        /* Read current time from Arduino clock and store in array */
        I2C_MasterReadBuf( ClockAddr, (uint8*) ArduinoClock, 3, I2C_MODE_COMPLETE_XFER);
        
        /* Save values from array in CurrentTime struct */
        CurrentTime.Hour = ArduinoClock[0];
        CurrentTime.Min = ArduinoClock[1];
        CurrentTime.Sec = ArduinoClock[2];
        
        /* Sends CurrentTime values to serial via UART, used for test 
        xQueueSendToBack(xQueueCurrentTime[0], &CurrentTime.Hour, portMAX_DELAY);
        xQueueSendToBack(xQueueCurrentTime[1], &CurrentTime.Min, portMAX_DELAY);
        xQueueSendToBack(xQueueCurrentTime[2], &CurrentTime.Sec, portMAX_DELAY);*/
        vTaskDelay(xShortDelayms);
                
        /*  This if-statement checks if the current hour is within the on-interval of the light cycle.
            If so, bLEDcmd is set to 1, commanding the LED to turned on. 
            After a short delay, the light sensor is read to make sure that the LED is on */
        if((CurrentTime.Hour >= LightCycle.Start) && (CurrentTime.Hour < LightCycle.Stop)){
            
            bLEDcmd = 1;
            xQueueSendToBack(xQueueLEDCmd, &bLEDcmd, portMAX_DELAY);
            //SW_UART_TEST_USB_PutString("Within active hours of light cycle: TRUE \n"); // Used for test
            vTaskDelay(xShortDelayms);
            
            if(Light == 0){
                // The LED are on, all is good
                //SW_UART_TEST_USB_PutString("Enough light: TRUE \n \n"); // Used for test
            }
            else if(Light == 1){
                // The LED are not on, all is not good, send an alarm
                //SW_UART_TEST_USB_PutString("Enough light: FALSE \n \n"); // Used for test
                bAlarmState = 1;
            }  
        }
        else{
            /* The LED should not be on, sending command to vTaskLEDcontrol to turn them off */
            bLEDcmd = 0;
            xQueueSendToBack(xQueueLEDCmd, &bLEDcmd, portMAX_DELAY);
                        
            //SW_UART_TEST_USB_PutString("Within active hours of light cycle: FALSE \n \n"); // Used for test
        }    
    vTaskDelay(xDelayms); 
    }
}


void vTaskLEDcontrol(){
    _Bool bLEDcontrolcmd;
    
    for(;;){
        /* Recieve on/off command and measured value */
        //*** vi skal lige snakke om hvad der foregår her
        xQueueReceive(xQueueLEDCmd, &bLEDcontrolcmd, portMAX_DELAY);
        /* USED FOR TEST
        SW_UART_TEST_USB_PutString("LED CMD: ");
        SW_UART_TEST_USB_PutHexInt(bLEDcontrolcmd);
        SW_UART_TEST_USB_PutString("\n");*/
        
        /* If the LED should be on and aren't on, turn them on. */
        if ( bLEDcontrolcmd == 1 && (Pin_LightTestLED_Read() == 0)){
            Pin_LightTestLED_Write( 1u ); // Turns on LED
        }
        /* If the LED shouldn't be on, turn them off. */
        else if (bLEDcontrolcmd == 0 && (Pin_LightTestLED_Read() == 1)){
            Pin_LightTestLED_Write( 0u ); // Turns off LED
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


/*  Initialize test tasks
void vTestLightTaskInit(){
 //   xTaskCreate(vTestLightTask, "TestLight", 1000, NULL, 2, NULL);
}    
*/
/*  Function tests vTaskLightMeasure() and vTaskLightController() 
    by reading from the queue and sending to serial monitor 
void vTestLightTask(){
    uint8 TestLightValue;
    uint8 TestCurrentHour;
    uint8 TestCurrentMin;
    uint8 TestCurrentSec;
    for(;;){
        xQueueReceive(xQueueLightValue, &TestLightValue, portMAX_DELAY);
        xQueueReceive(xQueueCurrentTime[0], &TestCurrentHour, portMAX_DELAY);
        xQueueReceive(xQueueCurrentTime[1], &TestCurrentMin, portMAX_DELAY);
        xQueueReceive(xQueueCurrentTime[2], &TestCurrentSec, portMAX_DELAY);
        
        // Prints current time 
        SW_UART_TEST_USB_PutString("Current time: ");
        SW_UART_TEST_USB_PutHexByte(TestCurrentHour);
        SW_UART_TEST_USB_PutString(":");
        SW_UART_TEST_USB_PutHexByte(TestCurrentMin);
        SW_UART_TEST_USB_PutString(":");
        SW_UART_TEST_USB_PutHexByte(TestCurrentSec);
        SW_UART_TEST_USB_PutString("\n");
        
        // Prints light value 
        SW_UART_TEST_USB_PutString("Light Value: ");
        SW_UART_TEST_USB_PutHexInt(TestLightValue);
        SW_UART_TEST_USB_PutString("\n");
        
    }    
}*/