/*******************************************************************************
* File Name: DS18x8_1_isr_DataReady.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_DS18x8_1_isr_DataReady_H)
#define CY_ISR_DS18x8_1_isr_DataReady_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void DS18x8_1_isr_DataReady_Start(void);
void DS18x8_1_isr_DataReady_StartEx(cyisraddress address);
void DS18x8_1_isr_DataReady_Stop(void);

CY_ISR_PROTO(DS18x8_1_isr_DataReady_Interrupt);

void DS18x8_1_isr_DataReady_SetVector(cyisraddress address);
cyisraddress DS18x8_1_isr_DataReady_GetVector(void);

void DS18x8_1_isr_DataReady_SetPriority(uint8 priority);
uint8 DS18x8_1_isr_DataReady_GetPriority(void);

void DS18x8_1_isr_DataReady_Enable(void);
uint8 DS18x8_1_isr_DataReady_GetState(void);
void DS18x8_1_isr_DataReady_Disable(void);

void DS18x8_1_isr_DataReady_SetPending(void);
void DS18x8_1_isr_DataReady_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the DS18x8_1_isr_DataReady ISR. */
#define DS18x8_1_isr_DataReady_INTC_VECTOR            ((reg32 *) DS18x8_1_isr_DataReady__INTC_VECT)

/* Address of the DS18x8_1_isr_DataReady ISR priority. */
#define DS18x8_1_isr_DataReady_INTC_PRIOR             ((reg8 *) DS18x8_1_isr_DataReady__INTC_PRIOR_REG)

/* Priority of the DS18x8_1_isr_DataReady interrupt. */
#define DS18x8_1_isr_DataReady_INTC_PRIOR_NUMBER      DS18x8_1_isr_DataReady__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable DS18x8_1_isr_DataReady interrupt. */
#define DS18x8_1_isr_DataReady_INTC_SET_EN            ((reg32 *) DS18x8_1_isr_DataReady__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the DS18x8_1_isr_DataReady interrupt. */
#define DS18x8_1_isr_DataReady_INTC_CLR_EN            ((reg32 *) DS18x8_1_isr_DataReady__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the DS18x8_1_isr_DataReady interrupt state to pending. */
#define DS18x8_1_isr_DataReady_INTC_SET_PD            ((reg32 *) DS18x8_1_isr_DataReady__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the DS18x8_1_isr_DataReady interrupt. */
#define DS18x8_1_isr_DataReady_INTC_CLR_PD            ((reg32 *) DS18x8_1_isr_DataReady__INTC_CLR_PD_REG)


#endif /* CY_ISR_DS18x8_1_isr_DataReady_H */


/* [] END OF FILE */
