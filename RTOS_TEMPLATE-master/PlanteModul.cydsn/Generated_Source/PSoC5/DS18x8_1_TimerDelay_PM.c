/*******************************************************************************
* File Name: DS18x8_1_TimerDelay_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "DS18x8_1_TimerDelay.h"

static DS18x8_1_TimerDelay_backupStruct DS18x8_1_TimerDelay_backup;


/*******************************************************************************
* Function Name: DS18x8_1_TimerDelay_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  DS18x8_1_TimerDelay_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void DS18x8_1_TimerDelay_SaveConfig(void) 
{
    #if (!DS18x8_1_TimerDelay_UsingFixedFunction)
        DS18x8_1_TimerDelay_backup.TimerUdb = DS18x8_1_TimerDelay_ReadCounter();
        DS18x8_1_TimerDelay_backup.InterruptMaskValue = DS18x8_1_TimerDelay_STATUS_MASK;
        #if (DS18x8_1_TimerDelay_UsingHWCaptureCounter)
            DS18x8_1_TimerDelay_backup.TimerCaptureCounter = DS18x8_1_TimerDelay_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!DS18x8_1_TimerDelay_UDB_CONTROL_REG_REMOVED)
            DS18x8_1_TimerDelay_backup.TimerControlRegister = DS18x8_1_TimerDelay_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: DS18x8_1_TimerDelay_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  DS18x8_1_TimerDelay_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void DS18x8_1_TimerDelay_RestoreConfig(void) 
{   
    #if (!DS18x8_1_TimerDelay_UsingFixedFunction)

        DS18x8_1_TimerDelay_WriteCounter(DS18x8_1_TimerDelay_backup.TimerUdb);
        DS18x8_1_TimerDelay_STATUS_MASK =DS18x8_1_TimerDelay_backup.InterruptMaskValue;
        #if (DS18x8_1_TimerDelay_UsingHWCaptureCounter)
            DS18x8_1_TimerDelay_SetCaptureCount(DS18x8_1_TimerDelay_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!DS18x8_1_TimerDelay_UDB_CONTROL_REG_REMOVED)
            DS18x8_1_TimerDelay_WriteControlRegister(DS18x8_1_TimerDelay_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: DS18x8_1_TimerDelay_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  DS18x8_1_TimerDelay_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void DS18x8_1_TimerDelay_Sleep(void) 
{
    #if(!DS18x8_1_TimerDelay_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(DS18x8_1_TimerDelay_CTRL_ENABLE == (DS18x8_1_TimerDelay_CONTROL & DS18x8_1_TimerDelay_CTRL_ENABLE))
        {
            /* Timer is enabled */
            DS18x8_1_TimerDelay_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            DS18x8_1_TimerDelay_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    DS18x8_1_TimerDelay_Stop();
    DS18x8_1_TimerDelay_SaveConfig();
}


/*******************************************************************************
* Function Name: DS18x8_1_TimerDelay_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  DS18x8_1_TimerDelay_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void DS18x8_1_TimerDelay_Wakeup(void) 
{
    DS18x8_1_TimerDelay_RestoreConfig();
    #if(!DS18x8_1_TimerDelay_UDB_CONTROL_REG_REMOVED)
        if(DS18x8_1_TimerDelay_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                DS18x8_1_TimerDelay_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
