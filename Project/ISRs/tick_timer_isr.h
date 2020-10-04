#ifndef __TICK_TIMER_ISR_H
#define __TICK_TIMER_ISR_H

#include "SD_Card.h"
#include "main.h"

////////////////////////////////////////////////////////////////////////////////
extern uint32_t impulses_warm_speed, impulses_cold_speed;
extern float speed_warm_water, speed_cold_water;
extern float current_warm_water, current_cold_water;

extern bool start_warm_measurements, start_cold_measurements;
extern Data_Water_Expense_t Data_Water_Expense;

////////////////////////////////////////////////////////////////////////////////
//uint32_t GetTickTimer_T2(void);
void Timer2_Init(void);
void Timer3_Init(void);

////////////////////////////////////////////////////////////////////////////////

#endif // __TICK_TIMER_ISR_H