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
            UART_PutString("Initializing..");
             UART_PutString("\n");
UART_Start();    
I2C_Start();
SCD30_initialize();


    for(;;)
    {
        UART_PutString("Initializing..");
         UART_PutString("\n");
    float resultat[3] = {0};

    if (SCD30_isAvailable()) 
        {
            UART_PutString("SCD30 available");
        SCD30_getCarbonDioxideConcentration(resultat);
        UART_PutString("CO2: ");
        UART_PutHexInt(resultat[0]);
        UART_PutString("\n");
        UART_PutString("TEMP: ");
        UART_PutHexInt(resultat[1]);
        UART_PutString("\n");
        UART_PutString("RH: ");
        UART_PutHexInt(resultat[2]);
        UART_PutString("\n");
        }
    else {
    UART_PutString("ERROR");
    }
    }
}

/* [] END OF FILE */
