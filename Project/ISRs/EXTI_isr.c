/*****************************************************************************
* @file    EXTI_isr.c
* @author  
* @version V1.0
* @date    2020/09/27
* @brief   This file provides EXTI service routines
*****************************************************************************/

#include "MCU_config.h"
#include "EXTI_isr.h"

////////////////////////////////////////////////////////////////////////////////

void EXTInt_Init(void){

    // Connect EXTI Line to GPIO Pin
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, OPTIC_DataWarm_Pin);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, OPTIC_DataCold_Pin);
    // Connect EXTI Line to GPIO Pin
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, HAL_DataWarm_Pin);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, HAL_DataCold_Pin);
    
    // Configure OPTIC_DataCold_Pin EXTI line
    EXTI_InitTypeDef exti;
    exti.EXTI_Line    = EXTI_Line2;
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);    
    // Configure HAL_DataCold_Pin EXTI line
    exti.EXTI_Line    = EXTI_Line3;
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    // Configure OPTIC_DataCold_Pin EXTI line
    exti.EXTI_Line    = EXTI_Line4;
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);    
    // Configure HAL_DataCold_Pin EXTI line
    exti.EXTI_Line    = EXTI_Line5;
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    
    // Enable and set EXTI Interrupt to the lowest priority
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel                   = EXTI2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
    nvic.NVIC_IRQChannelSubPriority        = 0x0F;
    nvic.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic);

    nvic.NVIC_IRQChannel                   = EXTI3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
    nvic.NVIC_IRQChannelSubPriority        = 0x0F;
    nvic.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel                   = EXTI4_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
    nvic.NVIC_IRQChannelSubPriority        = 0x0F;
    nvic.NVIC_IRQChannelCmd                = ENABLE;    
    NVIC_Init(&nvic);    
    
    nvic.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
    nvic.NVIC_IRQChannelSubPriority        = 0x0F;
    nvic.NVIC_IRQChannelCmd                = ENABLE;   
    NVIC_Init(&nvic);    
}



void EXTI2_IRQHandler(void){            // OPTIC_Warm_Data IN
    if(EXTI_GetITStatus(EXTI_Line2) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line2);

        impulses_warm_water++;
        impulses_warm_speed++;
    }
}

void EXTI3_IRQHandler(void){            // OPTIC_Warm_Data IN
    if(EXTI_GetITStatus(EXTI_Line3) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line3);

        month_warm_water++;
        BKP_WriteBackupRegister(BKP_DR2, month_warm_water);
    }
}

void EXTI4_IRQHandler(void){            // OPTIC_Cold_Data IN
    if(EXTI_GetITStatus(EXTI_Line4) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line4);

        impulses_cold_water++;
        impulses_cold_speed++;
    }
}

void EXTI9_5_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line5) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line5);

        month_cold_water++;
        BKP_WriteBackupRegister(BKP_DR3, month_cold_water);
    } 
}