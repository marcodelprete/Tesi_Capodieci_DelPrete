/*******************************************************************************
* File Name: IDAC_Calibration.c
* Version 2.0
*
* Description:
*  This file provides the power management source code to API for the
*  IDAC8.
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


#include "IDAC_Calibration.h"

static IDAC_Calibration_backupStruct IDAC_Calibration_backup;


/*******************************************************************************
* Function Name: IDAC_Calibration_SaveConfig
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
void IDAC_Calibration_SaveConfig(void) 
{
    if (!((IDAC_Calibration_CR1 & IDAC_Calibration_SRC_MASK) == IDAC_Calibration_SRC_UDB))
    {
        IDAC_Calibration_backup.data_value = IDAC_Calibration_Data;
    }
}


/*******************************************************************************
* Function Name: IDAC_Calibration_RestoreConfig
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
void IDAC_Calibration_RestoreConfig(void) 
{
    if (!((IDAC_Calibration_CR1 & IDAC_Calibration_SRC_MASK) == IDAC_Calibration_SRC_UDB))
    {
        if((IDAC_Calibration_Strobe & IDAC_Calibration_STRB_MASK) == IDAC_Calibration_STRB_EN)
        {
            IDAC_Calibration_Strobe &= (uint8)(~IDAC_Calibration_STRB_MASK);
            IDAC_Calibration_Data = IDAC_Calibration_backup.data_value;
            IDAC_Calibration_Strobe |= IDAC_Calibration_STRB_EN;
        }
        else
        {
            IDAC_Calibration_Data = IDAC_Calibration_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: IDAC_Calibration_Sleep
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
*  IDAC_Calibration_backup.enableState: Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void IDAC_Calibration_Sleep(void) 
{
    if(IDAC_Calibration_ACT_PWR_EN == (IDAC_Calibration_PWRMGR & IDAC_Calibration_ACT_PWR_EN))
    {
        /* IDAC8 is enabled */
        IDAC_Calibration_backup.enableState = 1u;
    }
    else
    {
        /* IDAC8 is disabled */
        IDAC_Calibration_backup.enableState = 0u;
    }

    IDAC_Calibration_Stop();
    IDAC_Calibration_SaveConfig();
}


/*******************************************************************************
* Function Name: IDAC_Calibration_Wakeup
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
*  IDAC_Calibration_backup.enableState: Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void IDAC_Calibration_Wakeup(void) 
{
    IDAC_Calibration_RestoreConfig();
    
    if(IDAC_Calibration_backup.enableState == 1u)
    {
        /* Enable IDAC8's operation */
        IDAC_Calibration_Enable();
        
        /* Set the data register */
        IDAC_Calibration_SetValue(IDAC_Calibration_Data);
    } /* Do nothing if IDAC8 was disabled before */    
}


/* [] END OF FILE */
