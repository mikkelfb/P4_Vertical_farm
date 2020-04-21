/*******************************************************************************
* File Name: DS18x8_Trigger_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "DS18x8_Trigger.h"

/* Check for removal by optimization */
#if !defined(DS18x8_Trigger_Sync_ctrl_reg__REMOVED)

static DS18x8_Trigger_BACKUP_STRUCT  DS18x8_Trigger_backup = {0u};

    
/*******************************************************************************
* Function Name: DS18x8_Trigger_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DS18x8_Trigger_SaveConfig(void) 
{
    DS18x8_Trigger_backup.controlState = DS18x8_Trigger_Control;
}


/*******************************************************************************
* Function Name: DS18x8_Trigger_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void DS18x8_Trigger_RestoreConfig(void) 
{
     DS18x8_Trigger_Control = DS18x8_Trigger_backup.controlState;
}


/*******************************************************************************
* Function Name: DS18x8_Trigger_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DS18x8_Trigger_Sleep(void) 
{
    DS18x8_Trigger_SaveConfig();
}


/*******************************************************************************
* Function Name: DS18x8_Trigger_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DS18x8_Trigger_Wakeup(void)  
{
    DS18x8_Trigger_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
