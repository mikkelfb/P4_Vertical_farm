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
#include "Alarm.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


struct alarmMessage{
    char identifier;
    uint16 message;
};


QueueHandle_t xQueueAlarmFromController;
QueueHandle_t xQueueAlarmForController;
QueueHandle_t xQueueAlarmForFPGA;
QueueHandle_t xQueueAlarmFromFPGA;

void vAlarmInit()
{
    xTaskCreate(vTaskAlarmHandling, "Alarm", 1000, NULL, 4, NULL);
    
    xQueueAlarmFromController   = xQueueCreate(1, sizeof(struct alarmMessage));
    xQueueAlarmForController    = xQueueCreate(1, sizeof(_Bool));
    xQueueAlarmForFPGA          = xQueueCreate(1, sizeof(struct alarmMessage));
    xQueueAlarmFromFPGA         = xQueueCreate(1, sizeof(_Bool));
    
    #if ALARMTEST == 1
        vTestTaskAlarmInit();
    #endif
}

void vTaskAlarmHandling()
{
    struct alarmMessage messForFPGA;
    _Bool recievedMess;
    _Bool xStatus;
    
    for(;;)
    {
        xStatus = xQueueReceive(xQueueAlarmFromController, &messForFPGA, portMAX_DELAY);
        if (xStatus == pdTRUE)
        {
            xQueueSendToBack(xQueueAlarmForFPGA, &messForFPGA, portMAX_DELAY);
            xQueueReceive(xQueueAlarmFromFPGA, &recievedMess, portMAX_DELAY);
            xQueueSendToBack(xQueueAlarmForController, &recievedMess, portMAX_DELAY);
        }       
    }
}



////////////////////// testing section ////////////////////////////////////

void vTestTaskAlarmInit()
{
    xTaskCreate(vTaskTestAlarm, "Alarm Test", 1000, NULL, 4, NULL);
}

void vTaskTestAlarm()
{
    const TickType_t xDelayms = pdMS_TO_TICKS( 1000 );
    struct alarmMessage alarm;
    alarm.identifier = 't';
    alarm.message = 100;
    struct alarmMessage alarmOnFPGA;
    _Bool xStatusFPGA;
    _Bool xStatusController = 0;
    
    for(;;)
    {
        xQueueSendToBack(xQueueAlarmFromController, &alarm, portMAX_DELAY);
        xStatusFPGA = xQueueReceive(xQueueAlarmForFPGA, &alarmOnFPGA, portMAX_DELAY);
        
        if(xStatusFPGA == pdTRUE)
        {
            SW_UART_TEST_USB_PutString("alarm recieved in FPGA task: ");
            SW_UART_TEST_USB_PutString("\n");
            SW_UART_TEST_USB_PutChar(alarmOnFPGA.identifier);
            SW_UART_TEST_USB_PutString("\n");
            SW_UART_TEST_USB_PutHexInt(alarmOnFPGA.message);
            SW_UART_TEST_USB_PutString("\n");
            xQueueSendToBack(xQueueAlarmFromFPGA, &xStatusFPGA, portMAX_DELAY);
            
            xQueueReceive(xQueueAlarmForController, &xStatusController, portMAX_DELAY);
            if(xStatusController == pdTRUE)
            {
            SW_UART_TEST_USB_PutString("check recieved from alarmtask in controllertask"); 
            SW_UART_TEST_USB_PutString("\n"); 
            }
        }
        vTaskDelay(xDelayms);
    }
}



/* [] END OF FILE */
