#include "MCU_config.h"
#include "main.h"
#include "systick_timer.h"
#include "tick_timer_isr.h"


////////////////////////////////////////////////////////////////////////////////


void TIM3_IRQHandler(void){  
    static uint32_t Seconds_sys_tick_warm = 0, First_sys_tick_warm = 0;
    static uint32_t Seconds_sys_tick_cold = 0, First_sys_tick_cold = 0;
    static uint32_t impulses_warm = 0, impulses_cold = 0;
    static int warm_entries = 0, cold_entries = 0, zero_warm_speed_cnt = 0, zero_cold_speed_cnt = 0;
    static bool end_warm_measurements = TRUE, end_cold_measurements = TRUE;
    float  time_warm, time_cold;
    
    if(impulses_cold_speed > 0){
        impulses_warm = impulses_cold_speed - impulses_warm;
        warm_entries++;

        if(warm_entries > 1){
            Seconds_sys_tick_warm = GetSysTickTimer_ms();
            end_warm_measurements = FALSE;
            if(Seconds_sys_tick_warm > First_sys_tick_warm)
                time_warm = (float)(Seconds_sys_tick_warm - First_sys_tick_warm) / 1000;
            else if(Seconds_sys_tick_warm < First_sys_tick_warm)
                time_warm = (float)(Seconds_sys_tick_warm - First_sys_tick_warm) / 1000;

            speed_cold_water = ((float)impulses_warm / (float)Water_meter_const) / time_warm;

            impulses_cold_speed = 0;
            impulses_warm = 0;
            First_sys_tick_warm = Seconds_sys_tick_warm;
        }else if(warm_entries == 1) First_sys_tick_warm = GetSysTickTimer_ms();   

    }else{
        if(++zero_warm_speed_cnt == 3){
            zero_warm_speed_cnt = 0;
            speed_cold_water = 0;
            warm_entries = 0;
            if(Data_Water_Expense.full_struct == FALSE && end_warm_measurements == FALSE){
                end_warm_measurements = TRUE;
                Data_Water_Expense.end_warm_expense_time = RTC_GetCounter();
                Data_Water_Expense.current_warm_water = current_warm_water;
                start_warm_measurements = FALSE;
                if(start_cold_measurements == FALSE) Data_Water_Expense.full_struct = TRUE; 
            }
        }
    }
    ////////////////////////////
    if(impulses_cold_speed > 0){
        impulses_cold = impulses_cold_speed - impulses_cold;
        cold_entries++;

        if(cold_entries > 1){
            Seconds_sys_tick_cold = GetSysTickTimer_ms();
            end_cold_measurements = FALSE;
            if(Seconds_sys_tick_cold > First_sys_tick_cold)
                time_cold = (float)(Seconds_sys_tick_cold - First_sys_tick_cold) / 1000;
            else if(Seconds_sys_tick_cold < First_sys_tick_cold)
                time_cold = (float)(Seconds_sys_tick_cold - First_sys_tick_cold) / 1000;

            speed_cold_water = ((float)impulses_cold / (float)Water_meter_const) / time_cold;

            impulses_cold_speed = 0;
            impulses_cold = 0;
            First_sys_tick_cold = Seconds_sys_tick_cold;
        }else if(cold_entries == 1) First_sys_tick_cold = GetSysTickTimer_ms();   

    }else{
        if(++zero_cold_speed_cnt == 3){
            zero_cold_speed_cnt = 0;
            speed_cold_water = 0;
            cold_entries = 0;
            if(Data_Water_Expense.full_struct == FALSE && end_cold_measurements == FALSE){
                end_cold_measurements = TRUE;
                Data_Water_Expense.end_cold_expense_time = RTC_GetCounter();
                Data_Water_Expense.current_cold_water = current_cold_water;
                start_cold_measurements = FALSE;
                if(start_warm_measurements == FALSE) Data_Water_Expense.full_struct = TRUE; 
            }
        }
    }
    
    TIM3->SR &= ~TIM_SR_UIF;    // reset isr
}

////////////////////////////////////////////////////////////////////////////////

void Timer2_Init(void)
{
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);
  base_timer.TIM_Prescaler = 7200000 / 100000 - 1;
  base_timer.TIM_Period = 200;
  TIM_TimeBaseInit(TIM2, &base_timer);

  TIM_OCInitTypeDef timer_oc;
  TIM_OCStructInit(&timer_oc);
  timer_oc.TIM_Pulse = 200;
  timer_oc.TIM_OCMode = TIM_OCMode_PWM1;
  timer_oc.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC2Init(TIM2, &timer_oc);
  
  TIM_BDTRInitTypeDef bdtr;
  TIM_BDTRStructInit(&bdtr);
  bdtr.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(TIM2, &bdtr);

  /* Interupt when fill enable */
  //TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //TIM_Cmd(TIM2, ENABLE);

  //NVIC_EnableIRQ(TIM2_UP_IRQn);
}

void Timer3_Init(void){

    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitTypeDef base_timer;
    TIM_TimeBaseStructInit(&base_timer);
    base_timer.TIM_Prescaler = 7190;    // 100us
    base_timer.TIM_Period = 10000;      // 1000 * 100us
    //base_timer.TIM_RepetitionCounter = 0xFFFF;     // TIM2->ARR = 0xFFFF;
    TIM_TimeBaseInit(TIM3, &base_timer);

    /* Interupt when fill enable */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);     // TIM3->DIER = 0x01;
    TIM_Cmd(TIM3, ENABLE);                         // TIM3->CR1 = 0x0085;

    NVIC_EnableIRQ(TIM3_IRQn);
}
