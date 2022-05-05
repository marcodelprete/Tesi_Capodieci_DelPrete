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
#ifndef __CALIBRATION_H
    #define __CALIBRATION_H
    
    #define R_TIA 120000 //TIA resistance value
    
    #include "cytypes.h"
    #include "stdio.h"
    //#include "math.h"
    
    /*
    Struct definition that contains the type and name
    of the coefficients of the linear regression
    */
    struct Coeff {
    float32 first, second;
    } typedef Coeff;
    
    Coeff FindCoefficeint(); //Function definition for TIA calibration
    
#endif
/* [] END OF FILE */
