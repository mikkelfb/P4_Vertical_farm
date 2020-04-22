/*******************************************************************************
* File Name: Pin_LightTestLED.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pin_LightTestLED_H) /* Pins Pin_LightTestLED_H */
#define CY_PINS_Pin_LightTestLED_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_LightTestLED_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pin_LightTestLED__PORT == 15 && ((Pin_LightTestLED__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Pin_LightTestLED_Write(uint8 value);
void    Pin_LightTestLED_SetDriveMode(uint8 mode);
uint8   Pin_LightTestLED_ReadDataReg(void);
uint8   Pin_LightTestLED_Read(void);
void    Pin_LightTestLED_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pin_LightTestLED_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pin_LightTestLED_SetDriveMode() function.
     *  @{
     */
        #define Pin_LightTestLED_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pin_LightTestLED_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pin_LightTestLED_DM_RES_UP          PIN_DM_RES_UP
        #define Pin_LightTestLED_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pin_LightTestLED_DM_OD_LO           PIN_DM_OD_LO
        #define Pin_LightTestLED_DM_OD_HI           PIN_DM_OD_HI
        #define Pin_LightTestLED_DM_STRONG          PIN_DM_STRONG
        #define Pin_LightTestLED_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Pin_LightTestLED_MASK               Pin_LightTestLED__MASK
#define Pin_LightTestLED_SHIFT              Pin_LightTestLED__SHIFT
#define Pin_LightTestLED_WIDTH              1u

/* Interrupt constants */
#if defined(Pin_LightTestLED__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pin_LightTestLED_SetInterruptMode() function.
     *  @{
     */
        #define Pin_LightTestLED_INTR_NONE      (uint16)(0x0000u)
        #define Pin_LightTestLED_INTR_RISING    (uint16)(0x0001u)
        #define Pin_LightTestLED_INTR_FALLING   (uint16)(0x0002u)
        #define Pin_LightTestLED_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Pin_LightTestLED_INTR_MASK      (0x01u) 
#endif /* (Pin_LightTestLED__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_LightTestLED_PS                     (* (reg8 *) Pin_LightTestLED__PS)
/* Data Register */
#define Pin_LightTestLED_DR                     (* (reg8 *) Pin_LightTestLED__DR)
/* Port Number */
#define Pin_LightTestLED_PRT_NUM                (* (reg8 *) Pin_LightTestLED__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_LightTestLED_AG                     (* (reg8 *) Pin_LightTestLED__AG)                       
/* Analog MUX bux enable */
#define Pin_LightTestLED_AMUX                   (* (reg8 *) Pin_LightTestLED__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_LightTestLED_BIE                    (* (reg8 *) Pin_LightTestLED__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_LightTestLED_BIT_MASK               (* (reg8 *) Pin_LightTestLED__BIT_MASK)
/* Bypass Enable */
#define Pin_LightTestLED_BYP                    (* (reg8 *) Pin_LightTestLED__BYP)
/* Port wide control signals */                                                   
#define Pin_LightTestLED_CTL                    (* (reg8 *) Pin_LightTestLED__CTL)
/* Drive Modes */
#define Pin_LightTestLED_DM0                    (* (reg8 *) Pin_LightTestLED__DM0) 
#define Pin_LightTestLED_DM1                    (* (reg8 *) Pin_LightTestLED__DM1)
#define Pin_LightTestLED_DM2                    (* (reg8 *) Pin_LightTestLED__DM2) 
/* Input Buffer Disable Override */
#define Pin_LightTestLED_INP_DIS                (* (reg8 *) Pin_LightTestLED__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_LightTestLED_LCD_COM_SEG            (* (reg8 *) Pin_LightTestLED__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_LightTestLED_LCD_EN                 (* (reg8 *) Pin_LightTestLED__LCD_EN)
/* Slew Rate Control */
#define Pin_LightTestLED_SLW                    (* (reg8 *) Pin_LightTestLED__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_LightTestLED_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_LightTestLED__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_LightTestLED_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_LightTestLED__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_LightTestLED_PRTDSI__OE_SEL0        (* (reg8 *) Pin_LightTestLED__PRTDSI__OE_SEL0) 
#define Pin_LightTestLED_PRTDSI__OE_SEL1        (* (reg8 *) Pin_LightTestLED__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_LightTestLED_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_LightTestLED__PRTDSI__OUT_SEL0) 
#define Pin_LightTestLED_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_LightTestLED__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_LightTestLED_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_LightTestLED__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Pin_LightTestLED__SIO_CFG)
    #define Pin_LightTestLED_SIO_HYST_EN        (* (reg8 *) Pin_LightTestLED__SIO_HYST_EN)
    #define Pin_LightTestLED_SIO_REG_HIFREQ     (* (reg8 *) Pin_LightTestLED__SIO_REG_HIFREQ)
    #define Pin_LightTestLED_SIO_CFG            (* (reg8 *) Pin_LightTestLED__SIO_CFG)
    #define Pin_LightTestLED_SIO_DIFF           (* (reg8 *) Pin_LightTestLED__SIO_DIFF)
#endif /* (Pin_LightTestLED__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pin_LightTestLED__INTSTAT)
    #define Pin_LightTestLED_INTSTAT            (* (reg8 *) Pin_LightTestLED__INTSTAT)
    #define Pin_LightTestLED_SNAP               (* (reg8 *) Pin_LightTestLED__SNAP)
    
	#define Pin_LightTestLED_0_INTTYPE_REG 		(* (reg8 *) Pin_LightTestLED__0__INTTYPE)
#endif /* (Pin_LightTestLED__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pin_LightTestLED_H */


/* [] END OF FILE */
