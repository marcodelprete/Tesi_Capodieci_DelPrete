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


uint8 Flag_Limit = 0;
uint16 Value_mV = 0;
uint8 value = 0; 
uint8 Flag_Period = 0;
uint8 Flag_Measure = 0;

/*
Function that creates the triangular shape of the applied potential. 
Starting from the initil value set by the user, the virtual mass id added
and then a step potential of 3 mV until the maximum value set, after
which the direction of the potential is reversed. Two period are repeted.
*/

CY_ISR(Custom_ISR_CV)
{

 if ((Start_Measure == 1) & (Flag_CV == 1))
    {
       if(value == 0){
            Value_mV = 2000 + Neg_Pot;
            value = 1;
       }
    
       DVDAC_SetV_SetValue(Value_mV); 
       Flag_Measure = 1;
    
       //sprintf(DataBuffer6, "%hd\r\n", Value_mV);
       //UART_PutString(DataBuffer6);
   
        if(Flag_Limit == 0){
            Value_mV -= 3;
            if(Value_mV < (2000 + Pos_Pot)){
                Flag_Limit = 1;
            }
        }
            
        if(Flag_Limit == 1){
            Value_mV = Value_mV + 3;
            if(Value_mV >= (2000 + Neg_Pot)){
                Flag_Limit = 0;
                Flag_Period ++;
            }
        }
 }
}
/* [] END OF FILE */
