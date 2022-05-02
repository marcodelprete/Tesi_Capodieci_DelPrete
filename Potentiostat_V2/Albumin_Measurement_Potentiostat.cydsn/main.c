 
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
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"
#include "InterruptUART.h"
#include "VoltageControl.h"
#include "Rx_Hn.h"
#include "Calibration.h"
#include "Crono_Measurement.h"
#include "MinMax.h"
#include "R_TIA_Select.h"

int16 k = 0;
int16 j = 0;
float32 a_coeff = 0;
float32 b_coeff = 0;
int32 Offset = 0;
int32 Offset_Crono = 0;
uint16 d1 = 0;
uint16 d2 = 0;
uint16 Index_Array = 0;
int32 val_mv_crono = 0;
int32 val_cur_crono = 0;
int32 I_Ox;
int32 I_Red;
int32 max_Ox;
int32 min_Red;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    TIA_Start();
    AMux_Electrodes_Start();
    AMux_Cur_Start();
    AMux_Electrodes_Select(0);
    AMux_Cur_Select(0);
    Opamp_Start();
    VDAC_VG_Start();
    DVDAC_SetV_Start();
    ADC_DelSig_Start();
    UART_Start();
    PWM_isr_Start();
    
    isr_Crono_StartEx(Custom_ISR_Crono);   
    isr_CV_StartEx(Custom_ISR_CV);
    isr_RX_StartEx(Custom_ISR_RX);
    
    DataBuffer[0] = 0x0A;
    DataBuffer[3] = 0XFF;
    //DataBuffer1[0] = 0x0D;
    //DataBuffer1[3] = 0xFE;
 
    
    ADC_DelSig_StartConvert();
    
    for(;;)
    {
        /*
        Save the potential values in the respective
        variables
        */
        if(Flag_Hn == 1){
            Flag_Hn = 0;
            uint16* Set_Potential = Rx_Handler(Received_Ar);
            if(Flag_SetV == 1){
                Flag_SetV = 0;
                Neg_Pot = Set_Potential[0];
                Pos_Pot = -Set_Potential[1];
                Pot_Range = (Set_Potential[1] + Set_Potential[0])/3;
            }
            if(Flag_DataCrono == 1){
                Flag_DataCrono = 0;
                Crono_Pot = -Set_Potential[0];
            }
        }
      
        /*
        When the user presses "Stop" in the GUI, the measurement is stopped
        and all variables are re-initialized to the starting value
        */
        if(Stop == 1){
            Start_Measure = 0;
            Stop = 0;
            //PWM_isr_Sleep();
            DVDAC_SetV_Sleep();
            Index_Array=0;
            Flag_Limit = 0;
            Value_mV = 2000 + Neg_Pot;
            value = 0;
            val_mv_crono = 0;
            val_cur_crono = 0;
            max = -5000;
            max_old = 0;
            min = 5000;
            min_old = 0;
            mV_index_Ox = 0;
            mV_index_Red = 0;
            peak_Ox = 0;
            peak_Red = 0;
            //Pin_LED_Write(0);
        }
        
        /*
        Check if PSoC is correctly programmed
        */
        if(Flag_Programmed == 1){
            Flag_Programmed = 0;
            DataBuffer[1] = 0xFF;
            DataBuffer[2] = 0xFF;
            UART_PutArray(DataBuffer, 4);
        }
        
        /*
        Set the PWM period to have a scan rate of 50 mV/s
        */
        if(Flag_Fifty == 1){
            Flag_Fifty = 0;
            PWM_isr_Sleep();
            PWM_isr_WritePeriod(14399);
        }
        /*
        Set the PWM period to have a scan rate of 100 mV/s
        */
        if(Flag_Hundred == 1){
            Flag_Hundred = 0;
            PWM_isr_Sleep();
            PWM_isr_WritePeriod(7199);
        }
        
        /*
        Start the TIA calibration, save the coefficients of the 
        linear regression and enable the isr for CV or Chrono
        according to the user's choice
        */
        if(StartCalibration == 1){
        StartCalibration = 0;
        Coeff ret = FindCoefficeint();
        a_coeff = ret.first;
        b_coeff = ret.second;
        IDAC_Calibration_Stop();
        AMux_Cur_Select(0);
        DVDAC_SetV_Start();
        isr_Crono_Disable();
        isr_CV_Enable();
        if(Flag_Crono == 1){
           DVDAC_SetV_SetValue(Crono_Pot + 2000);
           isr_CV_Disable();
           isr_Crono_Enable();
        }
        
        Start_Measure = 1;
        //Pin_LED_Write(1);
        PWM_isr_Start();
        }
        
        /*
        Save in BufferDataSend all the values in mV coming
        from CV
        */
        if(Flag_Measure == 1){
            Flag_Measure = 0;          
            BufferDataSend[Index_Array] = Measure_Function();
            Index_Array++;
        }
        
        /*
        During the Chrono, measure the values in mV, transform them
        in nA, correct them with the linear regression and send them
        to the GUI
        */
        //if(Flag_contasec >= 340){
          //  Flag_contasec = 0;
          //  Stop = 1;
          //  Pin_LED_Write(1);
       // }
        
        
        /*
        Chrono measurement mode
        */
        if(Flag_Crono_M == 1){
            Flag_Crono_M = 0;
            val_mv_crono = Measure_Function();
            val_cur_crono = val_mv_crono*(1000000/R_set);
            Offset_Crono = b_coeff*val_cur_crono+a_coeff;
            val_cur_crono = val_cur_crono - Offset_Crono;
            DataBuffer[1] = val_cur_crono >> 8;
            DataBuffer[2] = val_cur_crono & 0xFF;
            UART_PutArray(DataBuffer, 4);
        }
        
        /*
        CV measurement mode for one period
        */
        if(Flag_One_Period == 1 && Flag_Period == 1){
            Start_Measure = 0;
            Pin_LED_Write(0);
            PWM_isr_Sleep();
            DVDAC_SetV_Sleep();
            Flag_Period = 0;
            Index_Array=0;
            Flag_Limit = 0;
            Value_mV = 2000 + Neg_Pot;
            value = 0;
            d1 = (2*Pot_Range)+1;
            d2 = d1 + (Pot_Range+1);
            
            for(k=1; k < Pot_Range+1; k++){
                I_Ox = BufferDataSend[k]*(1000000/(R_set));
                Offset = b_coeff*I_Ox+a_coeff;
                I_Ox = I_Ox - Offset;
                max_Ox = Max_Function(I_Ox);
                DataBuffer[1] = I_Ox >> 8;
                DataBuffer[2] = I_Ox & 0xFF;
                UART_PutArray(DataBuffer, 4);
            }
            
            for(j=0; j < Pot_Range-1; j++){
                I_Red = BufferDataSend[j+(Pot_Range)]*(1000000/(R_set));
                Offset = b_coeff*I_Red+a_coeff;
                I_Red = I_Red - Offset;
                min_Red = Min_Function(I_Red);
                DataBuffer[1] = I_Red >> 8;
                DataBuffer[2] = I_Red & 0xFF;
                UART_PutArray(DataBuffer, 4);
            }

            //Peak potential
            DataBuffer[1] = max_Ox >> 8;
            DataBuffer[2] = max_Ox & 0xFF;
            UART_PutArray(DataBuffer, 4);
            CyDelay(2);
            DataBuffer[1] = min_Red >> 8;
            DataBuffer[2] = min_Red & 0xFF;
            UART_PutArray(DataBuffer, 4);
            
            max = -5000;
            max_old = 0;
            min = 5000;
            min_old = 0;
            mV_index_Ox = 0;
            mV_index_Red = 0;
            peak_Ox = 0;
            peak_Red = 0;
        }
        
        
        
        /*
        When the two periods of the CV are completed, reset all variables
        used, make the average of the current values obtained on the periods, 
        correct the offset, find the valuee of the potentials that correspond 
        to the peak current and send current values divided into oxidative and
        reductive and peak potential to the GUI
        */
        if(Flag_Two_Period == 1 && Flag_Period == 2)
        {
            Start_Measure = 0;
            Pin_LED_Write(0);
            PWM_isr_Sleep();
            DVDAC_SetV_Sleep();
            Flag_Period = 0;
            Index_Array=0;
            Flag_Limit = 0;
            Value_mV = 2000 + Neg_Pot;
            value = 0;
            d1 = (2*Pot_Range)+1;
            d2 = d1 + (Pot_Range+1);
            
            for(k=1; k < Pot_Range+1; k++){
                I_Ox = (BufferDataSend[k]+BufferDataSend[k+d1])*(1000000/(R_TIA*2));
                Offset = b_coeff*I_Ox+a_coeff;
                I_Ox = I_Ox - Offset;
                max_Ox = Max_Function(I_Ox);
                DataBuffer[1] = I_Ox >> 8;
                DataBuffer[2] = I_Ox & 0xFF;
                UART_PutArray(DataBuffer, 4);
            }
            
            for(j=0; j < Pot_Range-1; j++){
                I_Red = (BufferDataSend[j+(Pot_Range+1)]+BufferDataSend[j+d2])*(1000000/(R_TIA*2));
                Offset = b_coeff*I_Red+a_coeff;
                I_Red = I_Red - Offset;
                min_Red = Min_Function(I_Red);
                DataBuffer[1] = I_Red >> 8;
                DataBuffer[2] = I_Red & 0xFF;
                UART_PutArray(DataBuffer, 4);
            }

            //Peak potential
            DataBuffer[1] = max_Ox >> 8;
            DataBuffer[2] = max_Ox & 0xFF;
            UART_PutArray(DataBuffer, 4);
            CyDelay(2);
            DataBuffer[1] = min_Red >> 8;
            DataBuffer[2] = min_Red & 0xFF;
            UART_PutArray(DataBuffer, 4);
            
            max = -5000;
            max_old = 0;
            min = 5000;
            min_old = 0;
            mV_index_Ox = 0;
            mV_index_Red = 0;
            peak_Ox = 0;
            peak_Red = 0;
           
        }
    }
}

/* [] END OF FILE */
