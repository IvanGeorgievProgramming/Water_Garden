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

    // Connect EXTI Line to Button GPIO Pin
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, OPTIC_DataCold_Pin);//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, OPTIC_DataCold_Pin);
    // Connect EXTI Line to Button GPIO Pin
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, HAL_DataCold_Pin);//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, HAL_DataCold_Pin);
    
    // Configure OPTIC_DataCold_Pin EXTI line
    EXTI_InitTypeDef exti;
    exti.EXTI_Line    = EXTI_Line4;//EXTI_Line6
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);    
    // Configure HAL_DataCold_Pin EXTI line
    exti.EXTI_Line    = EXTI_Line5;//EXTI_Line8
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    
    // Enable and set EXTI Interrupt to the lowest priority
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel                   = EXTI0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
    nvic.NVIC_IRQChannelSubPriority        = 0x0F;
    nvic.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel                   = EXTI4_IRQn;
    NVIC_Init(&nvic);    
    
    nvic.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    NVIC_Init(&nvic);    
}



void EXTI0_IRQHandler(void){            // OPTIC_Warm_Data IN
    if(EXTI_GetITStatus(EXTI_Line0) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line0);

        impulses_warm_water++;
        impulses_warm_speed++;
    }
}

void EXTI4_IRQHandler(void){            // OPTIC_Cold_Data IN
    if(EXTI_GetITStatus(EXTI_Line4) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line4);

        impulses_cold_water++;//month_warm_water++;
        impulses_cold_speed++;//BKP_WriteBackupRegister(BKP_DR2, month_warm_water);
    }
}

void EXTI9_5_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line5) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line5);//EXTI_Line8

        month_cold_water++;
        BKP_WriteBackupRegister(BKP_DR3, month_cold_water);
    } 
    
    if(EXTI_GetITStatus(EXTI_Line0) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line0);

        impulses_warm_water++;
        impulses_warm_speed++;
    }

    if(EXTI_GetITStatus(EXTI_Line6) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line6);

        impulses_cold_water++;
        impulses_cold_speed++;
    }
}