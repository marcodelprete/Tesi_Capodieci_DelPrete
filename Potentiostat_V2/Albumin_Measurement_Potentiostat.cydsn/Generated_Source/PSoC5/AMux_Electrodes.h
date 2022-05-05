/*******************************************************************************
* File Name: AMux_Electrodes.h
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

#if !defined(CY_AMUX_AMux_Electrodes_H)
#define CY_AMUX_AMux_Electrodes_H

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

void AMux_Electrodes_Start(void) ;
#define AMux_Electrodes_Init() AMux_Electrodes_Start()
void AMux_Electrodes_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_Electrodes_Stop(void); */
/* void AMux_Electrodes_Select(uint8 channel); */
/* void AMux_Electrodes_Connect(uint8 channel); */
/* void AMux_Electrodes_Disconnect(uint8 channel); */
/* void AMux_Electrodes_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_Electrodes_CHANNELS  2u
#define AMux_Electrodes_MUXTYPE   1
#define AMux_Electrodes_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_Electrodes_NULL_CHANNEL 0xFFu
#define AMux_Electrodes_MUX_SINGLE   1
#define AMux_Electrodes_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_Electrodes_MUXTYPE == AMux_Electrodes_MUX_SINGLE
# if !AMux_Electrodes_ATMOSTONE
#  define AMux_Electrodes_Connect(channel) AMux_Electrodes_Set(channel)
# endif
# define AMux_Electrodes_Disconnect(channel) AMux_Electrodes_Unset(channel)
#else
# if !AMux_Electrodes_ATMOSTONE
void AMux_Electrodes_Connect(uint8 channel) ;
# endif
void AMux_Electrodes_Disconnect(uint8 channel) ;
#endif

#if AMux_Electrodes_ATMOSTONE
# define AMux_Electrodes_Stop() AMux_Electrodes_DisconnectAll()
# define AMux_Electrodes_Select(channel) AMux_Electrodes_FastSelect(channel)
void AMux_Electrodes_DisconnectAll(void) ;
#else
# define AMux_Electrodes_Stop() AMux_Electrodes_Start()
void AMux_Electrodes_Select(uint8 channel) ;
# define AMux_Electrodes_DisconnectAll() AMux_Electrodes_Start()
#endif

#endif /* CY_AMUX_AMux_Electrodes_H */


/* [] END OF FILE */
