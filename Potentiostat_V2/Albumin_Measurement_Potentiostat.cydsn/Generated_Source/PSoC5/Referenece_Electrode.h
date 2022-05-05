/*******************************************************************************
* File Name: Referenece_Electrode.h  
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

#if !defined(CY_PINS_Referenece_Electrode_H) /* Pins Referenece_Electrode_H */
#define CY_PINS_Referenece_Electrode_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Referenece_Electrode_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Referenece_Electrode__PORT == 15 && ((Referenece_Electrode__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Referenece_Electrode_Write(uint8 value);
void    Referenece_Electrode_SetDriveMode(uint8 mode);
uint8   Referenece_Electrode_ReadDataReg(void);
uint8   Referenece_Electrode_Read(void);
void    Referenece_Electrode_SetInterruptMode(uint16 position, uint16 mode);
uint8   Referenece_Electrode_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Referenece_Electrode_SetDriveMode() function.
     *  @{
     */
        #define Referenece_Electrode_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Referenece_Electrode_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Referenece_Electrode_DM_RES_UP          PIN_DM_RES_UP
        #define Referenece_Electrode_DM_RES_DWN         PIN_DM_RES_DWN
        #define Referenece_Electrode_DM_OD_LO           PIN_DM_OD_LO
        #define Referenece_Electrode_DM_OD_HI           PIN_DM_OD_HI
        #define Referenece_Electrode_DM_STRONG          PIN_DM_STRONG
        #define Referenece_Electrode_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Referenece_Electrode_MASK               Referenece_Electrode__MASK
#define Referenece_Electrode_SHIFT              Referenece_Electrode__SHIFT
#define Referenece_Electrode_WIDTH              1u

/* Interrupt constants */
#if defined(Referenece_Electrode__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Referenece_Electrode_SetInterruptMode() function.
     *  @{
     */
        #define Referenece_Electrode_INTR_NONE      (uint16)(0x0000u)
        #define Referenece_Electrode_INTR_RISING    (uint16)(0x0001u)
        #define Referenece_Electrode_INTR_FALLING   (uint16)(0x0002u)
        #define Referenece_Electrode_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Referenece_Electrode_INTR_MASK      (0x01u) 
#endif /* (Referenece_Electrode__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Referenece_Electrode_PS                     (* (reg8 *) Referenece_Electrode__PS)
/* Data Register */
#define Referenece_Electrode_DR                     (* (reg8 *) Referenece_Electrode__DR)
/* Port Number */
#define Referenece_Electrode_PRT_NUM                (* (reg8 *) Referenece_Electrode__PRT) 
/* Connect to Analog Globals */                                                  
#define Referenece_Electrode_AG                     (* (reg8 *) Referenece_Electrode__AG)                       
/* Analog MUX bux enable */
#define Referenece_Electrode_AMUX                   (* (reg8 *) Referenece_Electrode__AMUX) 
/* Bidirectional Enable */                                                        
#define Referenece_Electrode_BIE                    (* (reg8 *) Referenece_Electrode__BIE)
/* Bit-mask for Aliased Register Access */
#define Referenece_Electrode_BIT_MASK               (* (reg8 *) Referenece_Electrode__BIT_MASK)
/* Bypass Enable */
#define Referenece_Electrode_BYP                    (* (reg8 *) Referenece_Electrode__BYP)
/* Port wide control signals */                                                   
#define Referenece_Electrode_CTL                    (* (reg8 *) Referenece_Electrode__CTL)
/* Drive Modes */
#define Referenece_Electrode_DM0                    (* (reg8 *) Referenece_Electrode__DM0) 
#define Referenece_Electrode_DM1                    (* (reg8 *) Referenece_Electrode__DM1)
#define Referenece_Electrode_DM2                    (* (reg8 *) Referenece_Electrode__DM2) 
/* Input Buffer Disable Override */
#define Referenece_Electrode_INP_DIS                (* (reg8 *) Referenece_Electrode__INP_DIS)
/* LCD Common or Segment Drive */
#define Referenece_Electrode_LCD_COM_SEG            (* (reg8 *) Referenece_Electrode__LCD_COM_SEG)
/* Enable Segment LCD */
#define Referenece_Electrode_LCD_EN                 (* (reg8 *) Referenece_Electrode__LCD_EN)
/* Slew Rate Control */
#define Referenece_Electrode_SLW                    (* (reg8 *) Referenece_Electrode__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Referenece_Electrode_PRTDSI__CAPS_SEL       (* (reg8 *) Referenece_Electrode__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Referenece_Electrode_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Referenece_Electrode__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Referenece_Electrode_PRTDSI__OE_SEL0        (* (reg8 *) Referenece_Electrode__PRTDSI__OE_SEL0) 
#define Referenece_Electrode_PRTDSI__OE_SEL1        (* (reg8 *) Referenece_Electrode__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Referenece_Electrode_PRTDSI__OUT_SEL0       (* (reg8 *) Referenece_Electrode__PRTDSI__OUT_SEL0) 
#define Referenece_Electrode_PRTDSI__OUT_SEL1       (* (reg8 *) Referenece_Electrode__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Referenece_Electrode_PRTDSI__SYNC_OUT       (* (reg8 *) Referenece_Electrode__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Referenece_Electrode__SIO_CFG)
    #define Referenece_Electrode_SIO_HYST_EN        (* (reg8 *) Referenece_Electrode__SIO_HYST_EN)
    #define Referenece_Electrode_SIO_REG_HIFREQ     (* (reg8 *) Referenece_Electrode__SIO_REG_HIFREQ)
    #define Referenece_Electrode_SIO_CFG            (* (reg8 *) Referenece_Electrode__SIO_CFG)
    #define Referenece_Electrode_SIO_DIFF           (* (reg8 *) Referenece_Electrode__SIO_DIFF)
#endif /* (Referenece_Electrode__SIO_CFG) */

/* Interrupt Registers */
#if defined(Referenece_Electrode__INTSTAT)
    #define Referenece_Electrode_INTSTAT            (* (reg8 *) Referenece_Electrode__INTSTAT)
    #define Referenece_Electrode_SNAP               (* (reg8 *) Referenece_Electrode__SNAP)
    
	#define Referenece_Electrode_0_INTTYPE_REG 		(* (reg8 *) Referenece_Electrode__0__INTTYPE)
#endif /* (Referenece_Electrode__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Referenece_Electrode_H */


/* [] END OF FILE */
