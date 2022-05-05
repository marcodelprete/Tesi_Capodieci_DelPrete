/*******************************************************************************
* File Name: IDAC_Calibration.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the 8-bit Current 
*  DAC (IDAC8) User Module.
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

#include "cytypes.h"
#include "IDAC_Calibration.h"   

#if (CY_PSOC5A)
    #include <CyLib.h>
#endif /* CY_PSOC5A */


uint8 IDAC_Calibration_initVar = 0u;


#if (CY_PSOC5A)
    static IDAC_Calibration_LOWPOWER_BACKUP_STRUCT  IDAC_Calibration_lowPowerBackup;
#endif /* CY_PSOC5A */

/* Variable to decide whether or not to restore control register in Enable()
   API. This valid only for PSoC5A */
#if (CY_PSOC5A)
    static uint8 IDAC_Calibration_restoreReg = 0u;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: IDAC_Calibration_Init
********************************************************************************
* Summary:
*  Initialize to the schematic state.
* 
* Parameters:
*  void:
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC_Calibration_Init(void) 
{
    IDAC_Calibration_CR0 = (IDAC_Calibration_MODE_I | IDAC_Calibration_DEFAULT_RANGE );

    /* Set default data source */
    #if(IDAC_Calibration_DEFAULT_DATA_SRC != 0u )    
        IDAC_Calibration_CR1 = (IDAC_Calibration_DEFAULT_CNTL | IDAC_Calibration_DACBUS_ENABLE);
    #else
        IDAC_Calibration_CR1 = (IDAC_Calibration_DEFAULT_CNTL | IDAC_Calibration_DACBUS_DISABLE);
    #endif /* (IDAC_Calibration_DEFAULT_DATA_SRC != 0u ) */
    
    /*Controls polarity from UDB Control*/
    #if(IDAC_Calibration_DEFAULT_POLARITY == IDAC_Calibration_HARDWARE_CONTROLLED)
        IDAC_Calibration_CR1 |= IDAC_Calibration_IDIR_SRC_UDB;
    #else
        IDAC_Calibration_CR1 |= IDAC_Calibration_DEFAULT_POLARITY;
    #endif/* (IDAC_Calibration_DEFAULT_POLARITY == IDAC_Calibration_HARDWARE_CONTROLLED) */
    /*Controls Current Source from UDB Control*/
    #if(IDAC_Calibration_HARDWARE_ENABLE != 0u ) 
        IDAC_Calibration_CR1 |= IDAC_Calibration_IDIR_CTL_UDB;
    #endif /* (IDAC_Calibration_HARDWARE_ENABLE != 0u ) */ 
    
    /* Set default strobe mode */
    #if(IDAC_Calibration_DEFAULT_STRB != 0u)
        IDAC_Calibration_Strobe |= IDAC_Calibration_STRB_EN;
    #endif /* (IDAC_Calibration_DEFAULT_STRB != 0u) */
    
    /* Set default speed */
    IDAC_Calibration_SetSpeed(IDAC_Calibration_DEFAULT_SPEED);
    
    /* Set proper DAC trim */
    IDAC_Calibration_DacTrim();
    
}


/*******************************************************************************
* Function Name: IDAC_Calibration_Enable
********************************************************************************
* Summary:
*  Enable the IDAC8
* 
* Parameters:
*  void:
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC_Calibration_Enable(void) 
{
    IDAC_Calibration_PWRMGR |= IDAC_Calibration_ACT_PWR_EN;
    IDAC_Calibration_STBY_PWRMGR |= IDAC_Calibration_STBY_PWR_EN;

    /* This is to restore the value of register CR0 which is saved 
      in prior to the modification in stop() API */
    #if (CY_PSOC5A)
        if(IDAC_Calibration_restoreReg == 1u)
        {
            IDAC_Calibration_CR0 = IDAC_Calibration_lowPowerBackup.DACCR0Reg;

            /* Clear the flag */
            IDAC_Calibration_restoreReg = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: IDAC_Calibration_Start
********************************************************************************
* Summary:
*  Set power level then turn on IDAC8.
*
* Parameters:  
*  power: Sets power level between off (0) and (3) high power
*
* Return:
*  (void)
*
* Global variables:
*  IDAC_Calibration_initVar: Is modified when this function is called for 
*   the first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void IDAC_Calibration_Start(void) 
{
    /* Hardware initiazation only needs to occur the first time */
    if ( IDAC_Calibration_initVar == 0u)  
    {
        IDAC_Calibration_Init();
        
        IDAC_Calibration_initVar = 1u;
    }
   
    /* Enable power to DAC */
    IDAC_Calibration_Enable();

    /* Set default value */
    IDAC_Calibration_SetValue(IDAC_Calibration_DEFAULT_DATA);

}


/*******************************************************************************
* Function Name: IDAC_Calibration_Stop
********************************************************************************
* Summary:
*  Powers down IDAC8 to lowest power state.
*
* Parameters:
*  (void)
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC_Calibration_Stop(void) 
{
    /* Disble power to DAC */
    IDAC_Calibration_PWRMGR &= (uint8)(~IDAC_Calibration_ACT_PWR_EN);
    IDAC_Calibration_STBY_PWRMGR &= (uint8)(~IDAC_Calibration_STBY_PWR_EN);
    
    #if (CY_PSOC5A)
    
        /* Set the global variable  */
        IDAC_Calibration_restoreReg = 1u;

        /* Save the control register and then Clear it. */
        IDAC_Calibration_lowPowerBackup.DACCR0Reg = IDAC_Calibration_CR0;
        IDAC_Calibration_CR0 = (IDAC_Calibration_MODE_I | IDAC_Calibration_RANGE_3 | IDAC_Calibration_HS_HIGHSPEED);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: IDAC_Calibration_SetSpeed
********************************************************************************
* Summary:
*  Set DAC speed
*
* Parameters:
*  power: Sets speed value
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC_Calibration_SetSpeed(uint8 speed) 
{
    /* Clear power mask then write in new value */
    IDAC_Calibration_CR0 &= (uint8)(~IDAC_Calibration_HS_MASK);
    IDAC_Calibration_CR0 |=  ( speed & IDAC_Calibration_HS_MASK);
}


/*******************************************************************************
* Function Name: IDAC_Calibration_SetPolarity
********************************************************************************
* Summary:
*  Sets IDAC to Sink or Source current.
*  
* Parameters:
*  Polarity: Sets the IDAC to Sink or Source 
*  0x00 - Source
*  0x04 - Sink
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
#if(IDAC_Calibration_DEFAULT_POLARITY != IDAC_Calibration_HARDWARE_CONTROLLED)
    void IDAC_Calibration_SetPolarity(uint8 polarity) 
    {
        IDAC_Calibration_CR1 &= (uint8)(~IDAC_Calibration_IDIR_MASK);                /* clear polarity bit */
        IDAC_Calibration_CR1 |= (polarity & IDAC_Calibration_IDIR_MASK);             /* set new value */
    
        IDAC_Calibration_DacTrim();
    }
#endif/*(IDAC_Calibration_DEFAULT_POLARITY != IDAC_Calibration_HARDWARE_CONTROLLED)*/


/*******************************************************************************
* Function Name: IDAC_Calibration_SetRange
********************************************************************************
* Summary:
*  Set current range
*
* Parameters:
*  Range: Sets on of four valid ranges.
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC_Calibration_SetRange(uint8 range) 
{
    IDAC_Calibration_CR0 &= (uint8)(~IDAC_Calibration_RANGE_MASK);       /* Clear existing mode */
    IDAC_Calibration_CR0 |= ( range & IDAC_Calibration_RANGE_MASK );     /*  Set Range  */
    IDAC_Calibration_DacTrim();
}


/*******************************************************************************
* Function Name: IDAC_Calibration_SetValue
********************************************************************************
* Summary:
*  Set DAC value
*
* Parameters:
*  value: Sets DAC value between 0 and 255.
*
* Return:
*  (void)
*
* Theory:
*
* Side Effects:
*
*******************************************************************************/
void IDAC_Calibration_SetValue(uint8 value) 
{

    #if (CY_PSOC5A)
        uint8 IDAC_Calibration_intrStatus = CyEnterCriticalSection();
    #endif /* CY_PSOC5A */

    IDAC_Calibration_Data = value;         /*  Set Value  */
    
    /* PSOC5A silicons require a double write */
    #if (CY_PSOC5A)
        IDAC_Calibration_Data = value;
        CyExitCriticalSection(IDAC_Calibration_intrStatus);
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: IDAC_Calibration_DacTrim
********************************************************************************
* Summary:
*  Set the trim value for the given range.
*
* Parameters:
*  None
*
* Return:
*  (void) 
*
* Theory:
*  Trim values for the IDAC blocks are stored in the "Customer Table" area in 
*  Row 1 of the Hidden Flash.  There are 8 bytes of trim data for each 
*  IDAC block.
*  The values are:
*       I Gain offset, min range, Sourcing
*       I Gain offset, min range, Sinking
*       I Gain offset, med range, Sourcing
*       I Gain offset, med range, Sinking
*       I Gain offset, max range, Sourcing
*       I Gain offset, max range, Sinking
*       V Gain offset, 1V range
*       V Gain offset, 4V range
*
* Side Effects:
*
*******************************************************************************/
void IDAC_Calibration_DacTrim(void) 
{
    uint8 mode;

    mode = ((IDAC_Calibration_CR0 & IDAC_Calibration_RANGE_MASK) >> 1u);
    
    if((IDAC_Calibration_IDIR_MASK & IDAC_Calibration_CR1) == IDAC_Calibration_IDIR_SINK)
    {
        mode++;
    }

    IDAC_Calibration_TR = CY_GET_XTND_REG8((uint8 *)(IDAC_Calibration_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
