/*****************************************************************************
* @file    EXTI_isr.c
* @author  
* @version V1.0
* @date    2020/09/27
* @brief   This file provides EXTI service routines
*****************************************************************************/

#ifndef __EXTI_ISR_H
#define __EXTI_ISR_H


////////////////////////////////////////////////////////////////////////////////
extern  uint32_t  impulses_warm_water, impulses_cold_water;
extern  uint32_t  impulses_warm_speed, impulses_cold_speed;
extern  uint32_t  month_warm_water, month_cold_water;

////////////////////////////////////////////////////////////////////////////////
void EXTInt_Init(void);

////////////////////////////////////////////////////////////////////////////////

#endif // __EXTI_ISR_H