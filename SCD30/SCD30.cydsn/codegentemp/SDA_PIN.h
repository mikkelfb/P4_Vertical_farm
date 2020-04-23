/*******************************************************************************
* File Name: SDA_PIN.h  
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

#if !defined(CY_PINS_SDA_PIN_H) /* Pins SDA_PIN_H */
#define CY_PINS_SDA_PIN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SDA_PIN_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SDA_PIN__PORT == 15 && ((SDA_PIN__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    SDA_PIN_Write(uint8 value);
void    SDA_PIN_SetDriveMode(uint8 mode);
uint8   SDA_PIN_ReadDataReg(void);
uint8   SDA_PIN_Read(void);
void    SDA_PIN_SetInterruptMode(uint16 position, uint16 mode);
uint8   SDA_PIN_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the SDA_PIN_SetDriveMode() function.
     *  @{
     */
        #define SDA_PIN_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define SDA_PIN_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define SDA_PIN_DM_RES_UP          PIN_DM_RES_UP
        #define SDA_PIN_DM_RES_DWN         PIN_DM_RES_DWN
        #define SDA_PIN_DM_OD_LO           PIN_DM_OD_LO
        #define SDA_PIN_DM_OD_HI           PIN_DM_OD_HI
        #define SDA_PIN_DM_STRONG          PIN_DM_STRONG
        #define SDA_PIN_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define SDA_PIN_MASK               SDA_PIN__MASK
#define SDA_PIN_SHIFT              SDA_PIN__SHIFT
#define SDA_PIN_WIDTH              1u

/* Interrupt constants */
#if defined(SDA_PIN__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SDA_PIN_SetInterruptMode() function.
     *  @{
     */
        #define SDA_PIN_INTR_NONE      (uint16)(0x0000u)
        #define SDA_PIN_INTR_RISING    (uint16)(0x0001u)
        #define SDA_PIN_INTR_FALLING   (uint16)(0x0002u)
        #define SDA_PIN_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define SDA_PIN_INTR_MASK      (0x01u) 
#endif /* (SDA_PIN__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SDA_PIN_PS                     (* (reg8 *) SDA_PIN__PS)
/* Data Register */
#define SDA_PIN_DR                     (* (reg8 *) SDA_PIN__DR)
/* Port Number */
#define SDA_PIN_PRT_NUM                (* (reg8 *) SDA_PIN__PRT) 
/* Connect to Analog Globals */                                                  
#define SDA_PIN_AG                     (* (reg8 *) SDA_PIN__AG)                       
/* Analog MUX bux enable */
#define SDA_PIN_AMUX                   (* (reg8 *) SDA_PIN__AMUX) 
/* Bidirectional Enable */                                                        
#define SDA_PIN_BIE                    (* (reg8 *) SDA_PIN__BIE)
/* Bit-mask for Aliased Register Access */
#define SDA_PIN_BIT_MASK               (* (reg8 *) SDA_PIN__BIT_MASK)
/* Bypass Enable */
#define SDA_PIN_BYP                    (* (reg8 *) SDA_PIN__BYP)
/* Port wide control signals */                                                   
#define SDA_PIN_CTL                    (* (reg8 *) SDA_PIN__CTL)
/* Drive Modes */
#define SDA_PIN_DM0                    (* (reg8 *) SDA_PIN__DM0) 
#define SDA_PIN_DM1                    (* (reg8 *) SDA_PIN__DM1)
#define SDA_PIN_DM2                    (* (reg8 *) SDA_PIN__DM2) 
/* Input Buffer Disable Override */
#define SDA_PIN_INP_DIS                (* (reg8 *) SDA_PIN__INP_DIS)
/* LCD Common or Segment Drive */
#define SDA_PIN_LCD_COM_SEG            (* (reg8 *) SDA_PIN__LCD_COM_SEG)
/* Enable Segment LCD */
#define SDA_PIN_LCD_EN                 (* (reg8 *) SDA_PIN__LCD_EN)
/* Slew Rate Control */
#define SDA_PIN_SLW                    (* (reg8 *) SDA_PIN__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SDA_PIN_PRTDSI__CAPS_SEL       (* (reg8 *) SDA_PIN__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SDA_PIN_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SDA_PIN__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SDA_PIN_PRTDSI__OE_SEL0        (* (reg8 *) SDA_PIN__PRTDSI__OE_SEL0) 
#define SDA_PIN_PRTDSI__OE_SEL1        (* (reg8 *) SDA_PIN__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SDA_PIN_PRTDSI__OUT_SEL0       (* (reg8 *) SDA_PIN__PRTDSI__OUT_SEL0) 
#define SDA_PIN_PRTDSI__OUT_SEL1       (* (reg8 *) SDA_PIN__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SDA_PIN_PRTDSI__SYNC_OUT       (* (reg8 *) SDA_PIN__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(SDA_PIN__SIO_CFG)
    #define SDA_PIN_SIO_HYST_EN        (* (reg8 *) SDA_PIN__SIO_HYST_EN)
    #define SDA_PIN_SIO_REG_HIFREQ     (* (reg8 *) SDA_PIN__SIO_REG_HIFREQ)
    #define SDA_PIN_SIO_CFG            (* (reg8 *) SDA_PIN__SIO_CFG)
    #define SDA_PIN_SIO_DIFF           (* (reg8 *) SDA_PIN__SIO_DIFF)
#endif /* (SDA_PIN__SIO_CFG) */

/* Interrupt Registers */
#if defined(SDA_PIN__INTSTAT)
    #define SDA_PIN_INTSTAT            (* (reg8 *) SDA_PIN__INTSTAT)
    #define SDA_PIN_SNAP               (* (reg8 *) SDA_PIN__SNAP)
    
	#define SDA_PIN_0_INTTYPE_REG 		(* (reg8 *) SDA_PIN__0__INTTYPE)
#endif /* (SDA_PIN__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SDA_PIN_H */


/* [] END OF FILE */
