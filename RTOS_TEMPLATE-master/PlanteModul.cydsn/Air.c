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
#include "Air.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "Shared_resources.c"


struct AirCondition {   // Struct for holding values from the sensor 
    uint16 iAirTemp;
    uint16 iRH;
    uint16 iCO2;
};

struct TestData{        // Struct for testing the air controller 
    char identifier;
    uint16 message;
};

const int iSizeOfAir = 40; // Should be the same as the value of the array below
struct AirCondition currentAir[40]; //array of structs for calculating mean values 
int iAirTIndex = 0; // Use this index to save AirT measurements into the struct array
int iRHIndex = 0; // Use this index to save RH measurements into the struct array
int iCO2Index = 0; // Use this index to save CO2 measurements into the struct array


QueueHandle_t xQueueAirConditions;
QueueHandle_t xQueueAirTestValues;



 // initializing and creating task and queues
void vAirInit()
{
    xTaskCreate(vTaskAirController, "Air controller", 1000, NULL, 2, NULL);

    xQueueAirConditions = xQueueCreate(5, sizeof(struct TestData));
    xQueueAirTestValues = xQueueCreate(5, sizeof(struct TestData));
    
    #if AIRTEST ==1
        vTestTaskAirInit();
    #endif
}
    
    
    
void vTaskAirController()
{
    const TickType_t xDelayms = pdMS_TO_TICKS(1000);    
    struct TestData AirTMessage;                    //Struct works as buffer for the air temp values, when calcing mean.
    struct TestData RHMessage;                    //Struct works as buffer for the RH values, when calcing mean.
    struct TestData CO2Message;                 //Struct works as buffer for the CO2 values, when calculating mean
    struct TestData TestingParam;                 //Struct for recieving new paramsettings
    TestingParam.message =0;
    AirTMessage.identifier  = 'a';
    RHMessage.identifier    = 'r';
    CO2Message.identifier   = 'c';
    int RHParameter = 50;                          //Initial paramsettings
    _Bool bstate;
    _Bool alarmAck;
    for(;;)
    {
        vTaskDelay(xDelayms);
        AirTMessage.message = 0;
        RHMessage.message   = 0;
        CO2Message.message  = 0;
        for(int i = 0; i<= iSizeOfAir; i++)
        {                                               //Calculating meanvalues in the buffers
            AirTMessage.message = AirTMessage.message + currentAir[i].iAirTemp;
            RHMessage.message = RHMessage.message + currentAir[i].iRH;
            CO2Message.message = CO2Message.message + currentAir[i].iCO2;
        }
        AirTMessage.message = AirTMessage.message/iSizeOfAir;
        RHMessage.message = RHMessage.message/iSizeOfAir;
        CO2Message.message = CO2Message.message/iSizeOfAir; 
                                                        //Sending meanval to testtask or data_task
        xQueueSendToBack(xQueueControllerData, &AirTMessage, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &RHMessage, portMAX_DELAY);
        xQueueSendToBack(xQueueControllerData, &CO2Message, portMAX_DELAY);

        if(AirTMessage.message > 40 || AirTMessage.message < 5)      //Alarm for air temp
        {
            xQueueSendToBack(xQueueAlarmFromController, &AirTMessage, portMAX_DELAY);
            xQueueReceive(xQueueAlarmForController, &alarmAck, portMAX_DELAY);
            //SW_UART_TEST_USB_PutString("ALARM AirTemp");  // Alarm statement, TBD
            //SW_UART_TEST_USB_PutString("\n");
        }
         
        if(RHMessage.message > (RHParameter*1.25))      //Controlling fan
        {
            bstate = 1; // bstate high turning on fan
            //SW_UART_TEST_USB_PutString("Fan on");  
            //SW_UART_TEST_USB_PutString("\n");
        }
        if(RHMessage.message < (RHParameter*0.75))      //Controlling fan
        {
            bstate = 0; // bstate high turning off fan
            //SW_UART_TEST_USB_PutString("Fan off");  
            //SW_UART_TEST_USB_PutString("\n");
        }
        if(RHMessage.message < (RHParameter*0.5) || RHMessage.message > (RHParameter*1.5))
        {
            xQueueSendToBack(xQueueAlarmFromController, &RHMessage, portMAX_DELAY);
            xQueueReceive(xQueueAlarmForController, &alarmAck, portMAX_DELAY);
            //SW_UART_TEST_USB_PutString("ALARM RH");     // Alarm statement, TBD
            //SW_UART_TEST_USB_PutString("\n");
        }       
        if(CO2Message.message > 2000 || CO2Message.message < 1000)
        {
             xQueueSendToBack(xQueueAlarmFromController, &CO2Message, portMAX_DELAY);
            xQueueReceive(xQueueAlarmForController, &alarmAck, portMAX_DELAY);
            //SW_UART_TEST_USB_PutString("ALARM CO2");  // Alarm statement, TBD
            //SW_UART_TEST_USB_PutString("\n");
        }
        BaseType_t xStatus = xQueueReceive(xQueueAirTestValues, &TestingParam, portMAX_DELAY);
        if(xStatus == pdPASS)
        {                                               // Setting new params for RH
            switch(TestingParam.identifier)
            {
                case 'r':
                    RHParameter = TestingParam.message;
                    //SW_UART_TEST_USB_PutString("new RH value: ");
                    //SW_UART_TEST_USB_PutHexInt(RHParameter);
                    //SW_UART_TEST_USB_PutString("\n");
                    break;
            }
        }
    }
}   

        
                              ////// Test taks //////

void vTestTaskAirInit()
{
   // xTaskCreate(vTestTaskAirController, "Test task Air controller", 100, NULL, 3, NULL);
}


void vTestTaskAirController()
{
    const TickType_t xDelayms = pdMS_TO_TICKS(3000);
    struct TestData Receivedtest;
    struct TestData newParam;
    newParam.identifier = 'r';                      // Testinginstance for new RH paramsetting
    newParam.message    = 15;
    for(;;)
    {
        for(int i = 0; i<= iSizeOfAir; i++)   // Generating sensordata
        {
            currentAir[i].iAirTemp = i;
            currentAir[i].iRH = i;
            currentAir[i].iCO2 = i;  
        }
        vTaskDelay(xDelayms);                       // Recieveing and printing mean values
        xQueueReceive(xQueueAirConditions, &Receivedtest, portMAX_DELAY);
        SW_UART_TEST_USB_PutString("identifier: ");
        SW_UART_TEST_USB_PutChar(Receivedtest.identifier);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutString("value: ");
        SW_UART_TEST_USB_PutHexInt(Receivedtest.message);
        SW_UART_TEST_USB_PutString("\n");
        
        xQueueReceive(xQueueAirConditions, &Receivedtest, portMAX_DELAY);
        SW_UART_TEST_USB_PutString("identifier: ");
        SW_UART_TEST_USB_PutChar(Receivedtest.identifier);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutString("value: ");
        SW_UART_TEST_USB_PutHexInt(Receivedtest.message);
        SW_UART_TEST_USB_PutString("\n");
        
         xQueueReceive(xQueueAirConditions, &Receivedtest, portMAX_DELAY);
        SW_UART_TEST_USB_PutString("identifier: ");
        SW_UART_TEST_USB_PutChar(Receivedtest.identifier);
        SW_UART_TEST_USB_PutString("\n");
        SW_UART_TEST_USB_PutString("value: ");
        SW_UART_TEST_USB_PutHexInt(Receivedtest.message);
        SW_UART_TEST_USB_PutString("\n");
                                                    // sending new RH paramsetting to ControllerTask
        xQueueSendToBack(xQueueAirTestValues, &newParam, portMAX_DELAY);

    }
}  

/* [] END OF FILE */
