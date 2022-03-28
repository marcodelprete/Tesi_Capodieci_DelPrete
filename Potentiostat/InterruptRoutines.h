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
#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    int32 Measure_Function();
    uint8 DataBuffer[4]; //Buffer for sending data. First and last byte are Header and Tail
    char DataBuffer6[64];
    
    //volatile uint8 PacketReadyFlag;
    uint8 Start_Measure;
    
#endif
/* [] END OF FILE */
