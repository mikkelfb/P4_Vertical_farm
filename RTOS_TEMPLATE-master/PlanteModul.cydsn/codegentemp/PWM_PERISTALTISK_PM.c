/*******************************************************************************
* File Name: PWM_PERISTALTISK_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_PERISTALTISK.h"

static PWM_PERISTALTISK_backupStruct PWM_PERISTALTISK_backup;


/*******************************************************************************
* Function Name: PWM_PERISTALTISK_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_PERISTALTISK_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_PERISTALTISK_SaveConfig(void) 
{

    #if(!PWM_PERISTALTISK_UsingFixedFunction)
        #if(!PWM_PERISTALTISK_PWMModeIsCenterAligned)
            PWM_PERISTALTISK_backup.PWMPeriod = PWM_PERISTALTISK_ReadPeriod();
        #endif /* (!PWM_PERISTALTISK_PWMModeIsCenterAligned) */
        PWM_PERISTALTISK_backup.PWMUdb = PWM_PERISTALTISK_ReadCounter();
        #if (PWM_PERISTALTISK_UseStatus)
            PWM_PERISTALTISK_backup.InterruptMaskValue = PWM_PERISTALTISK_STATUS_MASK;
        #endif /* (PWM_PERISTALTISK_UseStatus) */

        #if(PWM_PERISTALTISK_DeadBandMode == PWM_PERISTALTISK__B_PWM__DBM_256_CLOCKS || \
            PWM_PERISTALTISK_DeadBandMode == PWM_PERISTALTISK__B_PWM__DBM_2_4_CLOCKS)
            PWM_PERISTALTISK_backup.PWMdeadBandValue = PWM_PERISTALTISK_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_PERISTALTISK_KillModeMinTime)
             PWM_PERISTALTISK_backup.PWMKillCounterPeriod = PWM_PERISTALTISK_ReadKillTime();
        #endif /* (PWM_PERISTALTISK_KillModeMinTime) */

        #if(PWM_PERISTALTISK_UseControl)
            PWM_PERISTALTISK_backup.PWMControlRegister = PWM_PERISTALTISK_ReadControlRegister();
        #endif /* (PWM_PERISTALTISK_UseControl) */
    #endif  /* (!PWM_PERISTALTISK_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_PERISTALTISK_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_PERISTALTISK_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_PERISTALTISK_RestoreConfig(void) 
{
        #if(!PWM_PERISTALTISK_UsingFixedFunction)
            #if(!PWM_PERISTALTISK_PWMModeIsCenterAligned)
                PWM_PERISTALTISK_WritePeriod(PWM_PERISTALTISK_backup.PWMPeriod);
            #endif /* (!PWM_PERISTALTISK_PWMModeIsCenterAligned) */

            PWM_PERISTALTISK_WriteCounter(PWM_PERISTALTISK_backup.PWMUdb);

            #if (PWM_PERISTALTISK_UseStatus)
                PWM_PERISTALTISK_STATUS_MASK = PWM_PERISTALTISK_backup.InterruptMaskValue;
            #endif /* (PWM_PERISTALTISK_UseStatus) */

            #if(PWM_PERISTALTISK_DeadBandMode == PWM_PERISTALTISK__B_PWM__DBM_256_CLOCKS || \
                PWM_PERISTALTISK_DeadBandMode == PWM_PERISTALTISK__B_PWM__DBM_2_4_CLOCKS)
                PWM_PERISTALTISK_WriteDeadTime(PWM_PERISTALTISK_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_PERISTALTISK_KillModeMinTime)
                PWM_PERISTALTISK_WriteKillTime(PWM_PERISTALTISK_backup.PWMKillCounterPeriod);
            #endif /* (PWM_PERISTALTISK_KillModeMinTime) */

            #if(PWM_PERISTALTISK_UseControl)
                PWM_PERISTALTISK_WriteControlRegister(PWM_PERISTALTISK_backup.PWMControlRegister);
            #endif /* (PWM_PERISTALTISK_UseControl) */
        #endif  /* (!PWM_PERISTALTISK_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_PERISTALTISK_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_PERISTALTISK_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_PERISTALTISK_Sleep(void) 
{
    #if(PWM_PERISTALTISK_UseControl)
        if(PWM_PERISTALTISK_CTRL_ENABLE == (PWM_PERISTALTISK_CONTROL & PWM_PERISTALTISK_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_PERISTALTISK_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_PERISTALTISK_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_PERISTALTISK_UseControl) */

    /* Stop component */
    PWM_PERISTALTISK_Stop();

    /* Save registers configuration */
    PWM_PERISTALTISK_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_PERISTALTISK_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_PERISTALTISK_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_PERISTALTISK_Wakeup(void) 
{
     /* Restore registers values */
    PWM_PERISTALTISK_RestoreConfig();

    if(PWM_PERISTALTISK_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_PERISTALTISK_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
