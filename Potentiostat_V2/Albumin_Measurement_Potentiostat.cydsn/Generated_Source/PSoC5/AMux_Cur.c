/*******************************************************************************
* File Name: AMux_Cur.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "AMux_Cur.h"

static uint8 AMux_Cur_lastChannel = AMux_Cur_NULL_CHANNEL;


/*******************************************************************************
* Function Name: AMux_Cur_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Cur_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < AMux_Cur_CHANNELS ; chan++)
    {
#if (AMux_Cur_MUXTYPE == AMux_Cur_MUX_SINGLE)
        AMux_Cur_Unset(chan);
#else
        AMux_Cur_CYAMUXSIDE_A_Unset(chan);
        AMux_Cur_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    AMux_Cur_lastChannel = AMux_Cur_NULL_CHANNEL;
}


#if (!AMux_Cur_ATMOSTONE)
/*******************************************************************************
* Function Name: AMux_Cur_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Cur_Select(uint8 channel) 
{
    AMux_Cur_DisconnectAll();        /* Disconnect all previous connections */
    AMux_Cur_Connect(channel);       /* Make the given selection */
    AMux_Cur_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: AMux_Cur_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Cur_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( AMux_Cur_lastChannel != AMux_Cur_NULL_CHANNEL)
    {
        AMux_Cur_Disconnect(AMux_Cur_lastChannel);
    }

    /* Make the new channel connection */
#if (AMux_Cur_MUXTYPE == AMux_Cur_MUX_SINGLE)
    AMux_Cur_Set(channel);
#else
    AMux_Cur_CYAMUXSIDE_A_Set(channel);
    AMux_Cur_CYAMUXSIDE_B_Set(channel);
#endif


    AMux_Cur_lastChannel = channel;   /* Update last channel */
}


#if (AMux_Cur_MUXTYPE == AMux_Cur_MUX_DIFF)
#if (!AMux_Cur_ATMOSTONE)
/*******************************************************************************
* Function Name: AMux_Cur_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Cur_Connect(uint8 channel) 
{
    AMux_Cur_CYAMUXSIDE_A_Set(channel);
    AMux_Cur_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: AMux_Cur_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Cur_Disconnect(uint8 channel) 
{
    AMux_Cur_CYAMUXSIDE_A_Unset(channel);
    AMux_Cur_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (AMux_Cur_ATMOSTONE)
/*******************************************************************************
* Function Name: AMux_Cur_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Cur_DisconnectAll(void) 
{
    if(AMux_Cur_lastChannel != AMux_Cur_NULL_CHANNEL) 
    {
        AMux_Cur_Disconnect(AMux_Cur_lastChannel);
        AMux_Cur_lastChannel = AMux_Cur_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
