/*******************************************************************************
* File Name: Worki.h  
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

#if !defined(CY_PINS_Worki_H) /* Pins Worki_H */
#define CY_PINS_Worki_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Worki_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Worki__PORT == 15 && ((Worki__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Worki_Write(uint8 value);
void    Worki_SetDriveMode(uint8 mode);
uint8   Worki_ReadDataReg(void);
uint8   Worki_Read(void);
void    Worki_SetInterruptMode(uint16 position, uint16 mode);
uint8   Worki_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Worki_SetDriveMode() function.
     *  @{
     */
        #define Worki_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Worki_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Worki_DM_RES_UP          PIN_DM_RES_UP
        #define Worki_DM_RES_DWN         PIN_DM_RES_DWN
        #define Worki_DM_OD_LO           PIN_DM_OD_LO
        #define Worki_DM_OD_HI           PIN_DM_OD_HI
        #define Worki_DM_STRONG          PIN_DM_STRONG
        #define Worki_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Worki_MASK               Worki__MASK
#define Worki_SHIFT              Worki__SHIFT
#define Worki_WIDTH              1u

/* Interrupt constants */
#if defined(Worki__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Worki_SetInterruptMode() function.
     *  @{
     */
        #define Worki_INTR_NONE      (uint16)(0x0000u)
        #define Worki_INTR_RISING    (uint16)(0x0001u)
        #define Worki_INTR_FALLING   (uint16)(0x0002u)
        #define Worki_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Worki_INTR_MASK      (0x01u) 
#endif /* (Worki__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Worki_PS                     (* (reg8 *) Worki__PS)
/* Data Register */
#define Worki_DR                     (* (reg8 *) Worki__DR)
/* Port Number */
#define Worki_PRT_NUM                (* (reg8 *) Worki__PRT) 
/* Connect to Analog Globals */                                                  
#define Worki_AG                     (* (reg8 *) Worki__AG)                       
/* Analog MUX bux enable */
#define Worki_AMUX                   (* (reg8 *) Worki__AMUX) 
/* Bidirectional Enable */                                                        
#define Worki_BIE                    (* (reg8 *) Worki__BIE)
/* Bit-mask for Aliased Register Access */
#define Worki_BIT_MASK               (* (reg8 *) Worki__BIT_MASK)
/* Bypass Enable */
#define Worki_BYP                    (* (reg8 *) Worki__BYP)
/* Port wide control signals */                                                   
#define Worki_CTL                    (* (reg8 *) Worki__CTL)
/* Drive Modes */
#define Worki_DM0                    (* (reg8 *) Worki__DM0) 
#define Worki_DM1                    (* (reg8 *) Worki__DM1)
#define Worki_DM2                    (* (reg8 *) Worki__DM2) 
/* Input Buffer Disable Override */
#define Worki_INP_DIS                (* (reg8 *) Worki__INP_DIS)
/* LCD Common or Segment Drive */
#define Worki_LCD_COM_SEG            (* (reg8 *) Worki__LCD_COM_SEG)
/* Enable Segment LCD */
#define Worki_LCD_EN                 (* (reg8 *) Worki__LCD_EN)
/* Slew Rate Control */
#define Worki_SLW                    (* (reg8 *) Worki__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Worki_PRTDSI__CAPS_SEL       (* (reg8 *) Worki__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Worki_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Worki__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Worki_PRTDSI__OE_SEL0        (* (reg8 *) Worki__PRTDSI__OE_SEL0) 
#define Worki_PRTDSI__OE_SEL1        (* (reg8 *) Worki__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Worki_PRTDSI__OUT_SEL0       (* (reg8 *) Worki__PRTDSI__OUT_SEL0) 
#define Worki_PRTDSI__OUT_SEL1       (* (reg8 *) Worki__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Worki_PRTDSI__SYNC_OUT       (* (reg8 *) Worki__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Worki__SIO_CFG)
    #define Worki_SIO_HYST_EN        (* (reg8 *) Worki__SIO_HYST_EN)
    #define Worki_SIO_REG_HIFREQ     (* (reg8 *) Worki__SIO_REG_HIFREQ)
    #define Worki_SIO_CFG            (* (reg8 *) Worki__SIO_CFG)
    #define Worki_SIO_DIFF           (* (reg8 *) Worki__SIO_DIFF)
#endif /* (Worki__SIO_CFG) */

/* Interrupt Registers */
#if defined(Worki__INTSTAT)
    #define Worki_INTSTAT            (* (reg8 *) Worki__INTSTAT)
    #define Worki_SNAP               (* (reg8 *) Worki__SNAP)
    
	#define Worki_0_INTTYPE_REG 		(* (reg8 *) Worki__0__INTTYPE)
#endif /* (Worki__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Worki_H */


/* [] END OF FILE */
