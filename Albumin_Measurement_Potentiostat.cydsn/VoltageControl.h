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
#ifndef __VOLTAGE_CONTROL_H
    #define __VOLTAGE_CONTROL_H
    
    #include "cytypes.h"
    #include "stdio.h"
  
    CY_ISR_PROTO(Custom_ISR_CV);
    
    /*
    //Buffer which contains data sampled by ADC_DelSig.
    Initialized to the maximum size of value that can be sampled.
    667 = 1000-(-1000)[mV]/3(step size)
    *4: to contain two periods
    -2: to remove the final value from the two periods which is not sampled
    */
    int32 BufferDataSend[667*4-2]; 

    uint8 Flag_Period;
    uint8 Flag_Limit;
    uint16 Value_mV;
    uint8 Flag_Measure;
    uint8 value;
    
#endif
/* [] END OF FILE */
