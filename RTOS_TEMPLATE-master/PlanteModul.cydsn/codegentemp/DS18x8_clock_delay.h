/*******************************************************************************
* File Name: DS18x8_clock_delay.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_DS18x8_clock_delay_H)
#define CY_CLOCK_DS18x8_clock_delay_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void DS18x8_clock_delay_Start(void) ;
void DS18x8_clock_delay_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void DS18x8_clock_delay_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void DS18x8_clock_delay_StandbyPower(uint8 state) ;
void DS18x8_clock_delay_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 DS18x8_clock_delay_GetDividerRegister(void) ;
void DS18x8_clock_delay_SetModeRegister(uint8 modeBitMask) ;
void DS18x8_clock_delay_ClearModeRegister(uint8 modeBitMask) ;
uint8 DS18x8_clock_delay_GetModeRegister(void) ;
void DS18x8_clock_delay_SetSourceRegister(uint8 clkSource) ;
uint8 DS18x8_clock_delay_GetSourceRegister(void) ;
#if defined(DS18x8_clock_delay__CFG3)
void DS18x8_clock_delay_SetPhaseRegister(uint8 clkPhase) ;
uint8 DS18x8_clock_delay_GetPhaseRegister(void) ;
#endif /* defined(DS18x8_clock_delay__CFG3) */

#define DS18x8_clock_delay_Enable()                       DS18x8_clock_delay_Start()
#define DS18x8_clock_delay_Disable()                      DS18x8_clock_delay_Stop()
#define DS18x8_clock_delay_SetDivider(clkDivider)         DS18x8_clock_delay_SetDividerRegister(clkDivider, 1u)
#define DS18x8_clock_delay_SetDividerValue(clkDivider)    DS18x8_clock_delay_SetDividerRegister((clkDivider) - 1u, 1u)
#define DS18x8_clock_delay_SetMode(clkMode)               DS18x8_clock_delay_SetModeRegister(clkMode)
#define DS18x8_clock_delay_SetSource(clkSource)           DS18x8_clock_delay_SetSourceRegister(clkSource)
#if defined(DS18x8_clock_delay__CFG3)
#define DS18x8_clock_delay_SetPhase(clkPhase)             DS18x8_clock_delay_SetPhaseRegister(clkPhase)
#define DS18x8_clock_delay_SetPhaseValue(clkPhase)        DS18x8_clock_delay_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(DS18x8_clock_delay__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define DS18x8_clock_delay_CLKEN              (* (reg8 *) DS18x8_clock_delay__PM_ACT_CFG)
#define DS18x8_clock_delay_CLKEN_PTR          ((reg8 *) DS18x8_clock_delay__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define DS18x8_clock_delay_CLKSTBY            (* (reg8 *) DS18x8_clock_delay__PM_STBY_CFG)
#define DS18x8_clock_delay_CLKSTBY_PTR        ((reg8 *) DS18x8_clock_delay__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define DS18x8_clock_delay_DIV_LSB            (* (reg8 *) DS18x8_clock_delay__CFG0)
#define DS18x8_clock_delay_DIV_LSB_PTR        ((reg8 *) DS18x8_clock_delay__CFG0)
#define DS18x8_clock_delay_DIV_PTR            ((reg16 *) DS18x8_clock_delay__CFG0)

/* Clock MSB divider configuration register. */
#define DS18x8_clock_delay_DIV_MSB            (* (reg8 *) DS18x8_clock_delay__CFG1)
#define DS18x8_clock_delay_DIV_MSB_PTR        ((reg8 *) DS18x8_clock_delay__CFG1)

/* Mode and source configuration register */
#define DS18x8_clock_delay_MOD_SRC            (* (reg8 *) DS18x8_clock_delay__CFG2)
#define DS18x8_clock_delay_MOD_SRC_PTR        ((reg8 *) DS18x8_clock_delay__CFG2)

#if defined(DS18x8_clock_delay__CFG3)
/* Analog clock phase configuration register */
#define DS18x8_clock_delay_PHASE              (* (reg8 *) DS18x8_clock_delay__CFG3)
#define DS18x8_clock_delay_PHASE_PTR          ((reg8 *) DS18x8_clock_delay__CFG3)
#endif /* defined(DS18x8_clock_delay__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define DS18x8_clock_delay_CLKEN_MASK         DS18x8_clock_delay__PM_ACT_MSK
#define DS18x8_clock_delay_CLKSTBY_MASK       DS18x8_clock_delay__PM_STBY_MSK

/* CFG2 field masks */
#define DS18x8_clock_delay_SRC_SEL_MSK        DS18x8_clock_delay__CFG2_SRC_SEL_MASK
#define DS18x8_clock_delay_MODE_MASK          (~(DS18x8_clock_delay_SRC_SEL_MSK))

#if defined(DS18x8_clock_delay__CFG3)
/* CFG3 phase mask */
#define DS18x8_clock_delay_PHASE_MASK         DS18x8_clock_delay__CFG3_PHASE_DLY_MASK
#endif /* defined(DS18x8_clock_delay__CFG3) */

#endif /* CY_CLOCK_DS18x8_clock_delay_H */


/* [] END OF FILE */
