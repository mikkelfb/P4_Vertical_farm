/*******************************************************************************
* File Name: ADC_Flow_PM.c
* Version 3.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_Flow.h"


/***************************************
* Local data allocation
***************************************/

static ADC_Flow_BACKUP_STRUCT  ADC_Flow_backup =
{
    ADC_Flow_DISABLED
};


/*******************************************************************************
* Function Name: ADC_Flow_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_Flow_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_Flow_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_Flow_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_Flow_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The ADC_Flow_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_Flow_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_Flow_Sleep(void)
{
    if((ADC_Flow_PWRMGR_SAR_REG  & ADC_Flow_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_Flow_SAR_CSR0_REG & ADC_Flow_SAR_SOF_START_CONV) != 0u)
        {
            ADC_Flow_backup.enableState = ADC_Flow_ENABLED | ADC_Flow_STARTED;
        }
        else
        {
            ADC_Flow_backup.enableState = ADC_Flow_ENABLED;
        }
        ADC_Flow_Stop();
    }
    else
    {
        ADC_Flow_backup.enableState = ADC_Flow_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_Flow_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  ADC_Flow_Sleep() was called. If the component was enabled before the
*  ADC_Flow_Sleep() function was called, the
*  ADC_Flow_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_Flow_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_Flow_Wakeup(void)
{
    if(ADC_Flow_backup.enableState != ADC_Flow_DISABLED)
    {
        ADC_Flow_Enable();
        #if(ADC_Flow_DEFAULT_CONV_MODE != ADC_Flow__HARDWARE_TRIGGER)
            if((ADC_Flow_backup.enableState & ADC_Flow_STARTED) != 0u)
            {
                ADC_Flow_StartConvert();
            }
        #endif /* End ADC_Flow_DEFAULT_CONV_MODE != ADC_Flow__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
