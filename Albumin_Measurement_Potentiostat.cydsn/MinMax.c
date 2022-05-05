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

#include "MinMax.h"
#include "Rx_Hn.h"
#include "project.h"

int32 max = -5000;
int32 max_old = 0;
int32 min = 5000;
int32 min_old = 0;
int16 mV_index_Ox = 0;
int16 mV_index_Red = 0;
int16 peak_Ox = 0;
int16 peak_Red = 0;

/*
Function that finds the value of the potential
corresponding to the oxidative current peak
*/
int16 Max_Function(int32 c_ox){
    max_old  = max;
    max = c_ox;
    
    if(c_ox > max_old){
        peak_Ox = -Neg_Pot + mV_index_Ox*3;
    }
    else{
        max = max_old;
    }
    mV_index_Ox++;
    
    return peak_Ox;
}

/*
Function that finds the value of the potential
corresponding to the reductive current peak
*/
int16 Min_Function(int32 c_red){
    min_old  = min;
    min = c_red;
    if(c_red < min_old){
        peak_Red = -Pos_Pot - mV_index_Red*3;
    }
    else{
        min = min_old;
    }
    mV_index_Red++;
       
    return peak_Red;
}

/* [] END OF FILE */
