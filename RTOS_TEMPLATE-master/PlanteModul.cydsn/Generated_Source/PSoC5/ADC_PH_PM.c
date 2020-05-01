/*******************************************************************************
* File Name: ADC_PH_PM.c
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

#include "ADC_PH.h"


/***************************************
* Local data allocation
***************************************/

static ADC_PH_BACKUP_STRUCT  ADC_PH_backup =
{
    ADC_PH_DISABLED
};


/*******************************************************************************
* Function Name: ADC_PH_SaveConfig
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
void ADC_PH_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_PH_RestoreConfig
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
void ADC_PH_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_PH_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The ADC_PH_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_PH_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_PH_Sleep(void)
{
    if((ADC_PH_PWRMGR_SAR_REG  & ADC_PH_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_PH_SAR_CSR0_REG & ADC_PH_SAR_SOF_START_CONV) != 0u)
        {
            ADC_PH_backup.enableState = ADC_PH_ENABLED | ADC_PH_STARTED;
        }
        else
        {
            ADC_PH_backup.enableState = ADC_PH_ENABLED;
        }
        ADC_PH_Stop();
    }
    else
    {
        ADC_PH_backup.enableState = ADC_PH_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_PH_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  ADC_PH_Sleep() was called. If the component was enabled before the
*  ADC_PH_Sleep() function was called, the
*  ADC_PH_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_PH_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_PH_Wakeup(void)
{
    if(ADC_PH_backup.enableState != ADC_PH_DISABLED)
    {
        ADC_PH_Enable();
        #if(ADC_PH_DEFAULT_CONV_MODE != ADC_PH__HARDWARE_TRIGGER)
            if((ADC_PH_backup.enableState & ADC_PH_STARTED) != 0u)
            {
                ADC_PH_StartConvert();
            }
        #endif /* End ADC_PH_DEFAULT_CONV_MODE != ADC_PH__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
