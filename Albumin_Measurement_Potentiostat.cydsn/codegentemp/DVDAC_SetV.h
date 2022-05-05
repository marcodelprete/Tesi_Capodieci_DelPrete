/*******************************************************************************
* File Name: DVDAC_SetV.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the DVDAC component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_DVDAC_DVDAC_SetV_H)
#define CY_DVDAC_DVDAC_SetV_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Data Struct Definition
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} DVDAC_SetV_BACKUP_STRUCT;

/* Component init state */
extern uint8 DVDAC_SetV_initVar;


/***************************************
*        API constants
***************************************/
#define DVDAC_SetV_RESOLUTION                 (12u)
#define DVDAC_SetV_INTERNAL_CLOCK_USED        (1u)

#define DVDAC_SetV_INTEGER_PORTION_SHIFT      (DVDAC_SetV_RESOLUTION - 8u)
#define DVDAC_SetV_DITHERED_ARRAY_SIZE        ((uint8)(1u << DVDAC_SetV_INTEGER_PORTION_SHIFT))
#define DVDAC_SetV_FRACTIONAL_PORTION_MASK    (DVDAC_SetV_DITHERED_ARRAY_SIZE - 1u)
#define DVDAC_SetV_INTEGER_PORTION_MAX_VALUE  (0xFFu)

/* DMA Configuration */
#define DVDAC_SetV_DMA_BYTES_PER_BURST        (1u)
#define DVDAC_SetV_DMA_REQUEST_PER_BURST      (1u)
#define DVDAC_SetV_DMA_SRC_BASE               (CYDEV_SRAM_BASE)
#define DVDAC_SetV_DMA_DST_BASE               (CYDEV_PERIPH_BASE)


#if   (DVDAC_SetV_RESOLUTION ==  9u)
    #define DVDAC_SetV_DVDAC_MAX_VALUE        (0x1FEu)
#elif (DVDAC_SetV_RESOLUTION == 10u)
    #define DVDAC_SetV_DVDAC_MAX_VALUE        (0x3FCu)
#elif (DVDAC_SetV_RESOLUTION == 11u)
    #define DVDAC_SetV_DVDAC_MAX_VALUE        (0x7F8u)
#else   /* Resolution 12 bits */
    #define DVDAC_SetV_DVDAC_MAX_VALUE        (0xFF0u)
#endif  /* (DVDAC_SetV_RESOLUTION ==  9u) */


/***************************************
*        Function Prototypes
***************************************/

void DVDAC_SetV_Init(void)                ;
void DVDAC_SetV_Enable(void)              ;
void DVDAC_SetV_Start(void)               ;
void DVDAC_SetV_Stop(void)                ;
void DVDAC_SetV_SetValue(uint16 value)    ;
void DVDAC_SetV_Sleep(void)               ;
void DVDAC_SetV_Wakeup(void)              ;
void DVDAC_SetV_SaveConfig(void)          ;
void DVDAC_SetV_RestoreConfig(void)       ;

#endif /* CY_DVDAC_DVDAC_SetV_H */


/* [] END OF FILE */
