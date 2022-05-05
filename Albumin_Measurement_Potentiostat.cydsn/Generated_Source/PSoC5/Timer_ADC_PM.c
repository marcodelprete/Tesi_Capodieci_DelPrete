/*******************************************************************************
* File Name: Timer_ADC_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Timer_ADC.h"

static Timer_ADC_backupStruct Timer_ADC_backup;


/*******************************************************************************
* Function Name: Timer_ADC_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_ADC_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_ADC_SaveConfig(void) 
{
    #if (!Timer_ADC_UsingFixedFunction)
        Timer_ADC_backup.TimerUdb = Timer_ADC_ReadCounter();
        Timer_ADC_backup.InterruptMaskValue = Timer_ADC_STATUS_MASK;
        #if (Timer_ADC_UsingHWCaptureCounter)
            Timer_ADC_backup.TimerCaptureCounter = Timer_ADC_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_ADC_UDB_CONTROL_REG_REMOVED)
            Timer_ADC_backup.TimerControlRegister = Timer_ADC_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_ADC_RestoreConfig
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
* Global variables:
*  Timer_ADC_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_ADC_RestoreConfig(void) 
{   
    #if (!Timer_ADC_UsingFixedFunction)

        Timer_ADC_WriteCounter(Timer_ADC_backup.TimerUdb);
        Timer_ADC_STATUS_MASK =Timer_ADC_backup.InterruptMaskValue;
        #if (Timer_ADC_UsingHWCaptureCounter)
            Timer_ADC_SetCaptureCount(Timer_ADC_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_ADC_UDB_CONTROL_REG_REMOVED)
            Timer_ADC_WriteControlRegister(Timer_ADC_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_ADC_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_ADC_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_ADC_Sleep(void) 
{
    #if(!Timer_ADC_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_ADC_CTRL_ENABLE == (Timer_ADC_CONTROL & Timer_ADC_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_ADC_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_ADC_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_ADC_Stop();
    Timer_ADC_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_ADC_Wakeup
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
*  Timer_ADC_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_ADC_Wakeup(void) 
{
    Timer_ADC_RestoreConfig();
    #if(!Timer_ADC_UDB_CONTROL_REG_REMOVED)
        if(Timer_ADC_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_ADC_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
