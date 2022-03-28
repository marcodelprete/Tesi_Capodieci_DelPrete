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
#ifndef __MINMAX_H
    #define __MINMAX_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    int16 Max_Function(int32 current_ox);
    int16 Min_Function(int32 current_red);
    
    int32 max;
    int32 max_old;
    int32 min;
    int32 min_old;
    int16 mV_index_Ox;
    int16 mV_index_Red;
    int16 peak_Ox;
    int16 peak_Red;
    
#endif

/* [] END OF FILE */
