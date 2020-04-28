/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */



#include <device.h>

/* RTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Plantemodul includes */
#include "Nutrients.h"
#include "Air.h"


/* The time between cycles of the 'check' functionality (defined within the
tick hook. */
#define mainCHECK_DELAY						( ( TickType_t ) 5000 / portTICK_PERIOD_MS )
#define mainCOM_LED							( 3 )

/* The number of nano seconds between each processor clock. */
#define mainNS_PER_CLOCK ( ( unsigned long ) ( ( 1.0 / ( double ) configCPU_CLOCK_HZ ) * 1000000000.0 ) )

/*
 * Installs the RTOS interrupt handlers and starts the peripherals.
 */
static void prvHardwareSetup( void );
/*---------------------------------------------------------------------------*/


int main( void )
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	prvHardwareSetup();
    
 //   vNutrientsInit(); //initialize Nutrients control task and parameteres
    
    vAirInit();
     
    
    
	/* Will only get here if there was insufficient memory to create the idle
    task.  The idle task is created within vTaskStartScheduler(). */
	vTaskStartScheduler();

    
    
	/* Should never reach here as the kernel will now be running.  If
	vTaskStartScheduler() does return then it is very likely that there was
	insufficient (FreeRTOS) heap space available to create all the tasks,
	including the idle task that is created within vTaskStartScheduler() itself. */
	for( ;; );
}
/*---------------------------------------------------------------------------*/





void prvHardwareSetup( void )
{
/* Port layer functions that need to be copied into the vector table. */
extern void xPortPendSVHandler( void );
extern void xPortSysTickHandler( void );
extern void vPortSVCHandler( void );
extern cyisraddress CyRamVectors[];



	/* Install the OS Interrupt Handlers. */
	CyRamVectors[ 11 ] = ( cyisraddress ) vPortSVCHandler;
	CyRamVectors[ 14 ] = ( cyisraddress ) xPortPendSVHandler;
	CyRamVectors[ 15 ] = ( cyisraddress ) xPortSysTickHandler;
    
    
    /* Startup One_Wire and I2C. */
    SW_UART_TEST_USB_Start();
    I2C_1_Start();

    
	/* Start-up the peripherals. */
        
    PWM_PERISTALTISK_1_Start(); //start PWM module 1
    PWM_PERISTALTISK_2_Start(); //start PWM module 2
    ADC_PH_Start(); //start ADC_PH module
    DS18x8_Start(); // Starting DS18x8 module
    
    
    
    
}
/*---------------------------------------------------------------------------*/


void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* The stack space has been execeeded for a task, considering allocating more. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*---------------------------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* The heap space has been execeeded. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*---------------------------------------------------------------------------*/
