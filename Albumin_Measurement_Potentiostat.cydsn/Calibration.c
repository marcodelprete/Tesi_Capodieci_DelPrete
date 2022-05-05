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
#include "Calibration.h"
#include "project.h"

#define SIZE_ARRAY_CALIB 19 //Number of elements for linear regression

int8 c_index = 0;
int32 value_digit_calib;
float32 value_mv_calib;
int32 Output_Calib_Array[SIZE_ARRAY_CALIB];
float32 x_sum = 0;
float32 y_sum = 0;
float64 x2_sum = 0;
float32 x_y_sum = 0;
float32 den = 0;

Coeff FindCoefficeint(){
    
    //Current values in hexadecimal to be loaded into the IDAC (31,875 : 255 = value_µA : x)
    uint8 Calib_current_array[] = {75, 67, 59, 51, 43, 35, 26, 18, 10, 0, 10, 18, 26, 35, 43, 51, 59, 67, 75}; 
    //Expected µA values
    int16 Decimal_current_array[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    IDAC_Calibration_Start();
    IDAC_Calibration_SetPolarity(IDAC_Calibration_SINK); 
    AMux_Cur_Select(1);
    
    Coeff ret;
    //Offset = b*measured current + a
    float32 a = 0;
    float32 b = 0;
    
    for(c_index=0; c_index<SIZE_ARRAY_CALIB; c_index++){
        /*
        One element at a time of the array Calib_current_array[] is loaded into IDAC, the first ten 
        in SINK mode and then in SOURCE mode. For each element, the corresopnding current value is 
        measured and the offset with respect to the expected value is stored in the array Output_Calib_Array[]
        */
        if(c_index > 9){
            IDAC_Calibration_SetPolarity(IDAC_Calibration_SOURCE);
            CyDelay(5);
        }
        
        IDAC_Calibration_SetValue(Calib_current_array[c_index]);
        CyDelay(5); //Delay to set the current
        
        value_digit_calib = ADC_DelSig_Read32();
        if(value_digit_calib < -65535)    value_digit_calib = -65535;
        if(value_digit_calib > 65535)     value_digit_calib = 65535;
        value_mv_calib = ADC_DelSig_CountsTo_mVolts(value_digit_calib);
        int32 I_read = (value_mv_calib*1000000)/R_TIA; //Cuurent in nA
        
        if(c_index <= 9){
            Output_Calib_Array[c_index] = I_read - (Decimal_current_array[c_index]*1000);
        }
        if(c_index > 9){
            Output_Calib_Array[c_index] = I_read + (Decimal_current_array[c_index]*1000);
        }
    }
    
    //Parameter calculation to derive the coefficients a and b
    for(c_index=0; c_index<SIZE_ARRAY_CALIB/2; c_index++){
        x_sum = x_sum + Decimal_current_array[c_index]*1000;
        y_sum = y_sum + Output_Calib_Array[c_index];
        x2_sum = x2_sum + ((Decimal_current_array[c_index]*Decimal_current_array[c_index])*1000000);
        x_y_sum = x_y_sum + (Decimal_current_array[c_index]*Output_Calib_Array[c_index]*1000);   
    }
    for(c_index=(SIZE_ARRAY_CALIB/2)+1; c_index<SIZE_ARRAY_CALIB; c_index++){
        x_sum = x_sum - Decimal_current_array[c_index]*1000;
        y_sum = y_sum + Output_Calib_Array[c_index];
        x2_sum = x2_sum + ((Decimal_current_array[c_index]*Decimal_current_array[c_index])*1000000);
        x_y_sum = x_y_sum + (-Decimal_current_array[c_index]*Output_Calib_Array[c_index]*1000);   
    }
    
    den = (SIZE_ARRAY_CALIB*x2_sum)-(x_sum*x_sum); //Denominator of the formula for a and b
    a = ((y_sum*x2_sum)-(x_sum*x_y_sum))/den;
    b = ((SIZE_ARRAY_CALIB*x_y_sum)-(x_sum*y_sum))/den;

    ret.first = a;
    ret.second = b;
    
    return ret;
}
/* [] END OF FILE */
