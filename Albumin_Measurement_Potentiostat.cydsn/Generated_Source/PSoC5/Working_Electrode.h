/*******************************************************************************
* File Name: Working_Electrode.h  
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

#if !defined(CY_PINS_Working_Electrode_H) /* Pins Working_Electrode_H */
#define CY_PINS_Working_Electrode_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Working_Electrode_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Working_Electrode__PORT == 15 && ((Working_Electrode__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Working_Electrode_Write(uint8 value);
void    Working_Electrode_SetDriveMode(uint8 mode);
uint8   Working_Electrode_ReadDataReg(void);
uint8   Working_Electrode_Read(void);
void    Working_Electrode_SetInterruptMode(uint16 position, uint16 mode);
uint8   Working_Electrode_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Working_Electrode_SetDriveMode() function.
     *  @{
     */
        #define Working_Electrode_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Working_Electrode_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Working_Electrode_DM_RES_UP          PIN_DM_RES_UP
        #define Working_Electrode_DM_RES_DWN         PIN_DM_RES_DWN
        #define Working_Electrode_DM_OD_LO           PIN_DM_OD_LO
        #define Working_Electrode_DM_OD_HI           PIN_DM_OD_HI
        #define Working_Electrode_DM_STRONG          PIN_DM_STRONG
        #define Working_Electrode_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Working_Electrode_MASK               Working_Electrode__MASK
#define Working_Electrode_SHIFT              Working_Electrode__SHIFT
#define Working_Electrode_WIDTH              1u

/* Interrupt constants */
#if defined(Working_Electrode__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Working_Electrode_SetInterruptMode() function.
     *  @{
     */
        #define Working_Electrode_INTR_NONE      (uint16)(0x0000u)
        #define Working_Electrode_INTR_RISING    (uint16)(0x0001u)
        #define Working_Electrode_INTR_FALLING   (uint16)(0x0002u)
        #define Working_Electrode_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Working_Electrode_INTR_MASK      (0x01u) 
#endif /* (Working_Electrode__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Working_Electrode_PS                     (* (reg8 *) Working_Electrode__PS)
/* Data Register */
#define Working_Electrode_DR                     (* (reg8 *) Working_Electrode__DR)
/* Port Number */
#define Working_Electrode_PRT_NUM                (* (reg8 *) Working_Electrode__PRT) 
/* Connect to Analog Globals */                                                  
#define Working_Electrode_AG                     (* (reg8 *) Working_Electrode__AG)                       
/* Analog MUX bux enable */
#define Working_Electrode_AMUX                   (* (reg8 *) Working_Electrode__AMUX) 
/* Bidirectional Enable */                                                        
#define Working_Electrode_BIE                    (* (reg8 *) Working_Electrode__BIE)
/* Bit-mask for Aliased Register Access */
#define Working_Electrode_BIT_MASK               (* (reg8 *) Working_Electrode__BIT_MASK)
/* Bypass Enable */
#define Working_Electrode_BYP                    (* (reg8 *) Working_Electrode__BYP)
/* Port wide control signals */                                                   
#define Working_Electrode_CTL                    (* (reg8 *) Working_Electrode__CTL)
/* Drive Modes */
#define Working_Electrode_DM0                    (* (reg8 *) Working_Electrode__DM0) 
#define Working_Electrode_DM1                    (* (reg8 *) Working_Electrode__DM1)
#define Working_Electrode_DM2                    (* (reg8 *) Working_Electrode__DM2) 
/* Input Buffer Disable Override */
#define Working_Electrode_INP_DIS                (* (reg8 *) Working_Electrode__INP_DIS)
/* LCD Common or Segment Drive */
#define Working_Electrode_LCD_COM_SEG            (* (reg8 *) Working_Electrode__LCD_COM_SEG)
/* Enable Segment LCD */
#define Working_Electrode_LCD_EN                 (* (reg8 *) Working_Electrode__LCD_EN)
/* Slew Rate Control */
#define Working_Electrode_SLW                    (* (reg8 *) Working_Electrode__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Working_Electrode_PRTDSI__CAPS_SEL       (* (reg8 *) Working_Electrode__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Working_Electrode_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Working_Electrode__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Working_Electrode_PRTDSI__OE_SEL0        (* (reg8 *) Working_Electrode__PRTDSI__OE_SEL0) 
#define Working_Electrode_PRTDSI__OE_SEL1        (* (reg8 *) Working_Electrode__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Working_Electrode_PRTDSI__OUT_SEL0       (* (reg8 *) Working_Electrode__PRTDSI__OUT_SEL0) 
#define Working_Electrode_PRTDSI__OUT_SEL1       (* (reg8 *) Working_Electrode__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Working_Electrode_PRTDSI__SYNC_OUT       (* (reg8 *) Working_Electrode__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Working_Electrode__SIO_CFG)
    #define Working_Electrode_SIO_HYST_EN        (* (reg8 *) Working_Electrode__SIO_HYST_EN)
    #define Working_Electrode_SIO_REG_HIFREQ     (* (reg8 *) Working_Electrode__SIO_REG_HIFREQ)
    #define Working_Electrode_SIO_CFG            (* (reg8 *) Working_Electrode__SIO_CFG)
    #define Working_Electrode_SIO_DIFF           (* (reg8 *) Working_Electrode__SIO_DIFF)
#endif /* (Working_Electrode__SIO_CFG) */

/* Interrupt Registers */
#if defined(Working_Electrode__INTSTAT)
    #define Working_Electrode_INTSTAT            (* (reg8 *) Working_Electrode__INTSTAT)
    #define Working_Electrode_SNAP               (* (reg8 *) Working_Electrode__SNAP)
    
	#define Working_Electrode_0_INTTYPE_REG 		(* (reg8 *) Working_Electrode__0__INTTYPE)
#endif /* (Working_Electrode__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Working_Electrode_H */


/* [] END OF FILE */
