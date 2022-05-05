/*******************************************************************************
* File Name: VDAC_VG_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VDAC_VG.h"

static VDAC_VG_backupStruct VDAC_VG_backup;


/*******************************************************************************
* Function Name: VDAC_VG_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC_VG_SaveConfig(void) 
{
    if (!((VDAC_VG_CR1 & VDAC_VG_SRC_MASK) == VDAC_VG_SRC_UDB))
    {
        VDAC_VG_backup.data_value = VDAC_VG_Data;
    }
}


/*******************************************************************************
* Function Name: VDAC_VG_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void VDAC_VG_RestoreConfig(void) 
{
    if (!((VDAC_VG_CR1 & VDAC_VG_SRC_MASK) == VDAC_VG_SRC_UDB))
    {
        if((VDAC_VG_Strobe & VDAC_VG_STRB_MASK) == VDAC_VG_STRB_EN)
        {
            VDAC_VG_Strobe &= (uint8)(~VDAC_VG_STRB_MASK);
            VDAC_VG_Data = VDAC_VG_backup.data_value;
            VDAC_VG_Strobe |= VDAC_VG_STRB_EN;
        }
        else
        {
            VDAC_VG_Data = VDAC_VG_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VDAC_VG_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  VDAC_VG_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VDAC_VG_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VDAC_VG_ACT_PWR_EN == (VDAC_VG_PWRMGR & VDAC_VG_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VDAC_VG_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VDAC_VG_backup.enableState = 0u;
    }
    
    VDAC_VG_Stop();
    VDAC_VG_SaveConfig();
}


/*******************************************************************************
* Function Name: VDAC_VG_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  VDAC_VG_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VDAC_VG_Wakeup(void) 
{
    VDAC_VG_RestoreConfig();
    
    if(VDAC_VG_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VDAC_VG_Enable();

        /* Restore the data register */
        VDAC_VG_SetValue(VDAC_VG_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
