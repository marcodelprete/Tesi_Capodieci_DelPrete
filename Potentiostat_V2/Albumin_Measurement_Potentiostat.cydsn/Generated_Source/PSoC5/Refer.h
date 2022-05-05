/*******************************************************************************
* File Name: Refer.h  
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

#if !defined(CY_PINS_Refer_H) /* Pins Refer_H */
#define CY_PINS_Refer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Refer_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Refer__PORT == 15 && ((Refer__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Refer_Write(uint8 value);
void    Refer_SetDriveMode(uint8 mode);
uint8   Refer_ReadDataReg(void);
uint8   Refer_Read(void);
void    Refer_SetInterruptMode(uint16 position, uint16 mode);
uint8   Refer_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Refer_SetDriveMode() function.
     *  @{
     */
        #define Refer_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Refer_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Refer_DM_RES_UP          PIN_DM_RES_UP
        #define Refer_DM_RES_DWN         PIN_DM_RES_DWN
        #define Refer_DM_OD_LO           PIN_DM_OD_LO
        #define Refer_DM_OD_HI           PIN_DM_OD_HI
        #define Refer_DM_STRONG          PIN_DM_STRONG
        #define Refer_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Refer_MASK               Refer__MASK
#define Refer_SHIFT              Refer__SHIFT
#define Refer_WIDTH              1u

/* Interrupt constants */
#if defined(Refer__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Refer_SetInterruptMode() function.
     *  @{
     */
        #define Refer_INTR_NONE      (uint16)(0x0000u)
        #define Refer_INTR_RISING    (uint16)(0x0001u)
        #define Refer_INTR_FALLING   (uint16)(0x0002u)
        #define Refer_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Refer_INTR_MASK      (0x01u) 
#endif /* (Refer__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Refer_PS                     (* (reg8 *) Refer__PS)
/* Data Register */
#define Refer_DR                     (* (reg8 *) Refer__DR)
/* Port Number */
#define Refer_PRT_NUM                (* (reg8 *) Refer__PRT) 
/* Connect to Analog Globals */                                                  
#define Refer_AG                     (* (reg8 *) Refer__AG)                       
/* Analog MUX bux enable */
#define Refer_AMUX                   (* (reg8 *) Refer__AMUX) 
/* Bidirectional Enable */                                                        
#define Refer_BIE                    (* (reg8 *) Refer__BIE)
/* Bit-mask for Aliased Register Access */
#define Refer_BIT_MASK               (* (reg8 *) Refer__BIT_MASK)
/* Bypass Enable */
#define Refer_BYP                    (* (reg8 *) Refer__BYP)
/* Port wide control signals */                                                   
#define Refer_CTL                    (* (reg8 *) Refer__CTL)
/* Drive Modes */
#define Refer_DM0                    (* (reg8 *) Refer__DM0) 
#define Refer_DM1                    (* (reg8 *) Refer__DM1)
#define Refer_DM2                    (* (reg8 *) Refer__DM2) 
/* Input Buffer Disable Override */
#define Refer_INP_DIS                (* (reg8 *) Refer__INP_DIS)
/* LCD Common or Segment Drive */
#define Refer_LCD_COM_SEG            (* (reg8 *) Refer__LCD_COM_SEG)
/* Enable Segment LCD */
#define Refer_LCD_EN                 (* (reg8 *) Refer__LCD_EN)
/* Slew Rate Control */
#define Refer_SLW                    (* (reg8 *) Refer__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Refer_PRTDSI__CAPS_SEL       (* (reg8 *) Refer__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Refer_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Refer__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Refer_PRTDSI__OE_SEL0        (* (reg8 *) Refer__PRTDSI__OE_SEL0) 
#define Refer_PRTDSI__OE_SEL1        (* (reg8 *) Refer__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Refer_PRTDSI__OUT_SEL0       (* (reg8 *) Refer__PRTDSI__OUT_SEL0) 
#define Refer_PRTDSI__OUT_SEL1       (* (reg8 *) Refer__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Refer_PRTDSI__SYNC_OUT       (* (reg8 *) Refer__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Refer__SIO_CFG)
    #define Refer_SIO_HYST_EN        (* (reg8 *) Refer__SIO_HYST_EN)
    #define Refer_SIO_REG_HIFREQ     (* (reg8 *) Refer__SIO_REG_HIFREQ)
    #define Refer_SIO_CFG            (* (reg8 *) Refer__SIO_CFG)
    #define Refer_SIO_DIFF           (* (reg8 *) Refer__SIO_DIFF)
#endif /* (Refer__SIO_CFG) */

/* Interrupt Registers */
#if defined(Refer__INTSTAT)
    #define Refer_INTSTAT            (* (reg8 *) Refer__INTSTAT)
    #define Refer_SNAP               (* (reg8 *) Refer__SNAP)
    
	#define Refer_0_INTTYPE_REG 		(* (reg8 *) Refer__0__INTTYPE)
#endif /* (Refer__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Refer_H */


/* [] END OF FILE */
