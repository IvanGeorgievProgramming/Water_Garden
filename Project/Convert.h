#ifndef _CONVERT_H
#define _CONVERT_H
////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"
#include "main.h"

////////////////////////////////////////////////////////////////////////////////
extern uint8_t  depth, line;
extern uint8_t  currentMatrix;
extern uint32_t cntCurrentTime, cntMenuTime, cntSleepTime;
extern uint8_t  cntSpeedUnit;
extern float current_warm_water, current_cold_water;
extern uint32_t impulses_warm_water, impulses_cold_water;
extern uint8_t flag_show_main_screen, flag_in_sleep;

extern char WriteOut[];
extern Data_Water_Expense_t Data_Water_Expense;

////////////////////////////////////////////////////////////////////////////////
void func_resettime(void);
void make_current_water(void);

void convert_month_char(uint32_t value);
void convert_speed_char(float speed);
void convert_current_char(float number);
void convert_time_char(uint8_t number);
void convert_year_char(uint16_t number);
void convert_mintime_char(uint32_t number);
void convert_sectime_char(uint32_t number);
void convert_date_char(uint32_t number);
void convert_min_char_SD(uint8_t min);
void convert_year_char_SD(uint16_t year);

#endif  //Convert.h

