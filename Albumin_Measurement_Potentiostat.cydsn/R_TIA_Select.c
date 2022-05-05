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

#include "Rx_Hn.h"
#include "project.h"

uint32 R_set = 120000;


uint32 R_Tia(uint8 R_TIA_value){
    if(R_TIA_value == 0){
        TIA_SetResFB(TIA_RES_FEEDBACK_20K);
        Pin_LED_Write(1);
        R_set = 20000;
        return R_set;
        
    }
    else if(R_TIA_value == 1){
        TIA_SetResFB(TIA_RES_FEEDBACK_30K);
        R_set = 30000;
        return R_set;
    } 
    else if(R_TIA_value == 2){
        TIA_SetResFB(TIA_RES_FEEDBACK_40K);
        R_set = 40000;
        return R_set;
    }
    else if(R_TIA_value == 3){
        TIA_SetResFB(TIA_RES_FEEDBACK_80K);
        R_set = 80000;
        return R_set;
    }
    else if(R_TIA_value == 4){
        TIA_SetResFB(TIA_RES_FEEDBACK_120K);
        Pin_LED_Write(0);
        R_set = 120000;
        return R_set;
    }
    else if(R_TIA_value == 5){
        TIA_SetResFB(TIA_RES_FEEDBACK_250K);
        R_set = 250000;
        return R_set;
    }
    else if(R_TIA_value == 6){
        TIA_SetResFB(TIA_RES_FEEDBACK_500K);
        R_set = 500000;
        return R_set;
    }
    else if(R_TIA_value == 7){
        TIA_SetResFB(TIA_RES_FEEDBACK_1000K);
        R_set = 1000000;
        return R_set;
    }
}
/* [] END OF FILE */
