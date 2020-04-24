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
#include "project.h"
#include "SCD30.h"


int main(void)
{
    CyGlobalIntEnable; 
UART_Start(); 
UART_PutString("Hello World!");
UART_PutString("\n");
I2C_Start();

SCD30_initialize();


    for(;;)
    {
        CyDelay(5000);
    float result[3] = {0};
    if (SCD30_isAvailable()) 
        {
        UART_PutString("SCD30 available");
        UART_PutString("\n");
        SCD30_getCarbonDioxideConcentration(result);
        UART_PutString("CO2: ");
        UART_PutHexInt(result[0]);
        UART_PutString("\n");
        UART_PutString("TEMP: ");
        UART_PutHexInt(result[1]);
        UART_PutString("\n");
        UART_PutString("RH: ");
        UART_PutHexInt(result[2]);
        UART_PutString("\n");
        UART_PutString("\n");
        UART_PutString("\n");
        }
    else {
    UART_PutString("ERROR");
    }
    }
}

/* [] END OF FILE */
