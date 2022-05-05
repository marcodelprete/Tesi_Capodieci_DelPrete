/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "InterruptRoutines.h"
#include "InterruptUART.h"
#include "VoltageControl.h"
#include "Calibration.h"
#include "project.h"

uint8 Flag_Hn = 0;

//When a data arrives to the UART set Flag_Hn high

CY_ISR(Custom_ISR_RX){
    
     Flag_Hn = 1;
}  
/* [] END OF FILE */
