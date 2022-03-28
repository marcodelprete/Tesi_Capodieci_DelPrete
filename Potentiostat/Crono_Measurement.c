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
#include "VoltageControl.h"
#include "InterruptRoutines.h"
#include "InterruptUART.h"
#include "Rx_Hn.h"
#include "project.h"
#include "Crono_Measurement.h"


uint8 Flag_Crono_M = 0;
uint16 Flag_contasec = 0;

//Every 30 ms set Flag_Crono_M high 
CY_ISR(Custom_ISR_Crono){
PWM_isr_ReadStatusRegister();
 if ((Start_Measure == 1) & (Flag_Crono == 1)){
    Flag_Crono_M = 1;
    //Flag_contasec = Flag_contasec + 1;
 }
}
/* [] END OF FILE */
