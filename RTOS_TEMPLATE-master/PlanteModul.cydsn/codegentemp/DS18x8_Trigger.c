/*******************************************************************************
* File Name: DS18x8_Trigger.c  
* Version 1.80
*
* Description:
*  This file contains API to enable firmware control of a Control Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "DS18x8_Trigger.h"

/* Check for removal by optimization */
#if !defined(DS18x8_Trigger_Sync_ctrl_reg__REMOVED)

    
/*******************************************************************************
* Function Name: DS18x8_Trigger_Write
********************************************************************************
*
* Summary:
*  Write a byte to the Control Register.
*
* Parameters:
*  control:  The value to be assigned to the Control Register.
*
* Return:
*  None.
*
*******************************************************************************/
void DS18x8_Trigger_Write(uint8 control) 
{
    DS18x8_Trigger_Control = control;
}


/*******************************************************************************
* Function Name: DS18x8_Trigger_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Control Register.
*
* Parameters:
*  None.
*
* Return:
*  Returns the current value in the Control Register.
*
*******************************************************************************/
uint8 DS18x8_Trigger_Read(void) 
{
    return DS18x8_Trigger_Control;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */