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
#include "InterruptUART.h"
#include "InterruptRoutines.h"
#include "Rx_Hn.h"
#include "R_TIA_Select.h"
#include "project.h"

#define POT_H 0xA0
#define POT_T 0xC0
#define CRONO_H 0x0D 
#define CRONO_T 0xFE
#define CRONO_HN 0x0E
#define CRONO_TN 0xFD
#define H_PER 0x0F
#define T_PER 0xFB
#define H_R 0xAA
#define T_R 0xBB

#define IDLE 0
#define HEADER 1
#define DATA1 2
#define DATA2 3
#define TAIL 4

uint8 data_received = 0;
uint8 Flag_DataOK = 0;
uint8 Flag_DataCrono = 0;
uint8 Flag_SetV = 0;
uint8 Flag_CV = 1;
uint8 Flag_Crono = 0;
uint8 Flag_Programmed = 0;
uint8 Flag_One_Period = 0;
uint8 Flag_Two_Period = 1;
uint8 state = 0;
uint8 buffer[4];
uint16 DataValue = 0;
uint8 StartCalibration = 0;
uint8 Stop = 0;
uint8 Flag_Hundred = 0;
uint8 Flag_Fifty = 0;
uint16 Period_Value = 0;
uint32 R_setted = 120000;

uint16* Rx_Handler(uint16 *Received_Ar){
    
        data_received = UART_ReadRxData(); //Read one byte at a time
        state ++;
        
        if(data_received == 'v'){ //Start calibration 
            state =IDLE;
            StartCalibration = 1;
        }
        else if(data_received == 'f'){ //Stop measurement 
            state = IDLE;
            Stop = 1;
        }
        else if(data_received == 'c'){ //Set cyclovoltammetry
            state = IDLE;
            Flag_CV = 1;
            Flag_Crono = 0;
        }
        else if(data_received == 'r'){ //Set chronoamperometry
            state = IDLE;
            PWM_isr_Sleep();
            PWM_isr_WritePeriod(7199);
            Flag_Crono = 1;
            Flag_CV = 0;
        }
        else if(data_received == 'p'){ //To verify if the PSoC is correctly programmed
            state = IDLE;
            Flag_Programmed = 1;
        }
        else if(data_received == 'h'){ //Set PWM for a scan rate of 100mV/s
            state = IDLE;
            Flag_Hundred = 1;
        }
        else if(data_received == 's'){ //Set PWM for a scan rate of 50mV/s
            state = IDLE;
            Flag_Fifty = 1;
        }
        else if(data_received == 'w'){ //Set two-electrodes mode
            state = IDLE;
            AMux_Electrodes_Select(0);
            Pin_LED_Write(0);
        }
       else if(data_received == 'e'){ //Set three-electrodes mode
            state = IDLE;
            AMux_Electrodes_Select(1);
            Pin_LED_Write(1);
        } 
        //Unpacking and input data recognition: potential for CV or chrono
        switch(state){    
            case HEADER:
                if(data_received == POT_H){
                    buffer[state-1] = data_received;
                }
                else if(data_received == CRONO_H){
                    buffer[state-1] = 1;
                }
                else if(data_received == CRONO_HN){
                    buffer[state-1] = 2;
                }
                else if(data_received == H_PER){
                    buffer[state-1] = data_received;
                }
                else if(data_received == H_R){
                    buffer[state-1] = data_received;
                }
                break;
                
            case DATA1:
                buffer[state-1] = data_received;
                break;
            
            case DATA2:
                buffer[state-1] = data_received;
                break;
            
            case TAIL:
                if(data_received == POT_T){
                    buffer[state-1] = data_received;
                    state = IDLE;
                    Flag_DataOK++;
                    if(Flag_DataOK == 1){
                        Received_Ar[0] = buffer[1] << 8 | buffer[2]; //Negative CV potential
                    }
                    if(Flag_DataOK == 2){
                    Flag_DataOK = 0;
                    Received_Ar[1] = buffer[1] << 8 | buffer[2]; //Positive CV potential
                    Flag_SetV = 1;
                    return Received_Ar;
                    }
                }
                else if((data_received == CRONO_T) | (data_received == CRONO_TN)){
                    buffer[state-1] = data_received;
                    state = IDLE;
                    Received_Ar[0] = buffer[1] << 8 | buffer[2]; //Chrono potential
                    if(buffer[0] == 2){
                        Received_Ar[0] = -Received_Ar[0];
                    }
                    Flag_DataCrono = 1;
                    return Received_Ar;
                }
                else if(data_received == T_PER){
                    buffer[state-1] = data_received;
                    state = IDLE;
                    Period_Value = buffer[1] << 8 | buffer[2]; 
                    if(Period_Value == 0){
                        Flag_One_Period = 1;
                        Flag_Two_Period = 0;
                        
                    }
                    else if(Period_Value == 1){
                        Flag_Two_Period = 1;
                        Flag_One_Period = 0;
                        
                    }
                }
                
                else if(data_received == T_R){
                    buffer[state-1] = data_received;
                    state = IDLE;
                    R_TIA_value = buffer[1] << 8 | buffer[2]; 
                    R_setted = R_Tia(R_TIA_value);
      
                }
                break; 
        }
}
/* [] END OF FILE */
