/*******************************************************************************
* File Name: AMux_Cur.h
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

#if !defined(CY_AMUX_AMux_Cur_H)
#define CY_AMUX_AMux_Cur_H

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

void AMux_Cur_Start(void) ;
#define AMux_Cur_Init() AMux_Cur_Start()
void AMux_Cur_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_Cur_Stop(void); */
/* void AMux_Cur_Select(uint8 channel); */
/* void AMux_Cur_Connect(uint8 channel); */
/* void AMux_Cur_Disconnect(uint8 channel); */
/* void AMux_Cur_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_Cur_CHANNELS  2u
#define AMux_Cur_MUXTYPE   1
#define AMux_Cur_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_Cur_NULL_CHANNEL 0xFFu
#define AMux_Cur_MUX_SINGLE   1
#define AMux_Cur_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_Cur_MUXTYPE == AMux_Cur_MUX_SINGLE
# if !AMux_Cur_ATMOSTONE
#  define AMux_Cur_Connect(channel) AMux_Cur_Set(channel)
# endif
# define AMux_Cur_Disconnect(channel) AMux_Cur_Unset(channel)
#else
# if !AMux_Cur_ATMOSTONE
void AMux_Cur_Connect(uint8 channel) ;
# endif
void AMux_Cur_Disconnect(uint8 channel) ;
#endif

#if AMux_Cur_ATMOSTONE
# define AMux_Cur_Stop() AMux_Cur_DisconnectAll()
# define AMux_Cur_Select(channel) AMux_Cur_FastSelect(channel)
void AMux_Cur_DisconnectAll(void) ;
#else
# define AMux_Cur_Stop() AMux_Cur_Start()
void AMux_Cur_Select(uint8 channel) ;
# define AMux_Cur_DisconnectAll() AMux_Cur_Start()
#endif

#endif /* CY_AMUX_AMux_Cur_H */


/* [] END OF FILE */
