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
///////////////// File for Queuehandles used to send between different files //////////////////////


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

QueueHandle_t xQueueControllerData;         // Incoming data from controller tasks
QueueHandle_t xQueueCentralrequest;         // Requests from central task
QueueHandle_t xQueueCentralData;            // Data sent to central
QueueHandle_t xQueueStorageReady;           // Struct containing new data from all sensors sent to DatatStorage task


QueueHandle_t xQueueAlarmFromController;    // incoming alarm from controller tasks
QueueHandle_t xQueueAlarmForController;     // ack sending to controller tasks
QueueHandle_t xQueueAlarmForFPGA;           // sening alarm to UART task
QueueHandle_t xQueueAlarmFromFPGA;          // Receiving ack from UART task



/* [] END OF FILE */
