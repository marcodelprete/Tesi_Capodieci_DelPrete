/*******************************************************************************
* File Name: AMux_current.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_AMux_current_H)
#define CY_AMUX_AMux_current_H

#include "cyfitter.h"
#include "cyfitter_cfg.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))    
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif /* ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */


/***************************************
*        Function Prototypes
***************************************/

void AMux_current_Start(void) ;
#define AMux_current_Init() AMux_current_Start()
void AMux_current_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_current_Stop(void); */
/* void AMux_current_Select(uint8 channel); */
/* void AMux_current_Connect(uint8 channel); */
/* void AMux_current_Disconnect(uint8 channel); */
/* void AMux_current_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_current_CHANNELS  2u
#define AMux_current_MUXTYPE   1
#define AMux_current_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_current_NULL_CHANNEL 0xFFu
#define AMux_current_MUX_SINGLE   1
#define AMux_current_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_current_MUXTYPE == AMux_current_MUX_SINGLE
# if !AMux_current_ATMOSTONE
#  define AMux_current_Connect(channel) AMux_current_Set(channel)
# endif
# define AMux_current_Disconnect(channel) AMux_current_Unset(channel)
#else
# if !AMux_current_ATMOSTONE
void AMux_current_Connect(uint8 channel) ;
# endif
void AMux_current_Disconnect(uint8 channel) ;
#endif

#if AMux_current_ATMOSTONE
# define AMux_current_Stop() AMux_current_DisconnectAll()
# define AMux_current_Select(channel) AMux_current_FastSelect(channel)
void AMux_current_DisconnectAll(void) ;
#else
# define AMux_current_Stop() AMux_current_Start()
void AMux_current_Select(uint8 channel) ;
# define AMux_current_DisconnectAll() AMux_current_Start()
#endif

#endif /* CY_AMUX_AMux_current_H */


/* [] END OF FILE */
