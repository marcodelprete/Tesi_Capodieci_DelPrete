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
#ifndef __RX_HN_H
    #define __RX_HN_H
    
    #include "cytypes.h"
    #include "stdio.h"

    uint16 Received_Ar[2];
    uint16* Rx_Handler(uint16 *arr);
    
    uint8 StartCalibration;
    uint8 Flag_SetV; 
    uint8 Stop;
    int16 Neg_Pot;
    int16 Pos_Pot;
    int16 Crono_Pot;
    uint16 Pot_Range;
    uint8 Flag_CV;
    uint8 Flag_Crono;
    uint8 Flag_DataCrono;
    uint8 Flag_Programmed;
    uint8 Flag_Hundred;
    uint8 Flag_Fifty;
    uint8 Flag_One_Period;
    uint8 Flag_Two_Period;
    
#endif
/* [] END OF FILE */
