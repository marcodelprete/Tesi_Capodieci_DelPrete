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
#include "VoltageControl.h"
#include "Rx_Hn.h" 
#include "project.h"

int32 value_digit;
int32 value_mv;
uint8 SendBytesFlag = 0;

//Acquisition and return of mV values from ADC_DelSig

int32 Measure_Function(){
    if (Start_Measure == 1){
        CyDelay(5);
        value_digit = ADC_DelSig_Read32();
        if(value_digit < -65535)    value_digit = -65535;
        if(value_digit > 65535)     value_digit = 65535;
        
        value_mv = ADC_DelSig_CountsTo_mVolts(value_digit);
        
        return value_mv;
    }
}
/* [] END OF FILE */
