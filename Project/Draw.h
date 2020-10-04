#ifndef _DRAW_H
#define _DRAW_H


#include "stm32f10x.h"
#include "Convert.h"

////////////////////////////////////////////////////////////////////////////////
#define  Background_color  RGB565(67, 221, 245)
#define  Sleep_clock_background  RGB565(11, 12, 13)
#define  Text_color1  WHITE
#define  Text_color2  RGB565(112, 239, 205)

#define  Cold_water_rect_color  RGB565(20, 0, 255)
#define  Warm_water_rect_color  RGB565(246, 182, 34)
#define  Cold_watermeter_color  RGB565(95, 95, 205)
#define  Warm_watermeter_color  RGB565(255, 27, 157)
#define  Water_unit_rect_color  RGB565(130, 169, 21)
#define  Water_unit_text_color  RGB565(255, 255, 255)

//#define  Sleep_Clock_color  RGB565(79, 255, 132)
#define  Sleep_clock_color  RGB565(79, 148, 255)

#define x1 10
#define y1 7
#define w1 110
#define h1 44

#define x2 120
#define y2 7
#define w2 110
#define h2 44

#define x3 10 
#define y3 189
#define w3 110
#define h3 44

#define x4 120
#define y4 189
#define w4 110
#define h4 44

#define Water_unit_rect_coordinates  95, 179, 50, 20
#define Water_unit_text_coordinates  100, 181

#define  Month_measurements_warm_water_text_coordinates  35, 15
#define  Month_measurements_cold_water_text_coordinates  145, 15

#define  Month_measurements_warm_water_text_mask_coordinates  35, 195
#define  Month_measurements_cold_water_text_mask_coordinates  145, 195

#define  _BLACK RGB565(0, 0, 0)

////////////////////////////////////////////////////////////////////////////////
extern float current_warm_water, current_cold_water;
extern uint32_t month_warm_water, month_cold_water;
extern uint32_t pmonth_warm_water, pmonth_cold_water;
extern uint8_t hour, sec, min;
extern uint8_t cntSpeedUnit;
extern uint8_t depth, line;

extern float speed_cold_water, speed_warm_water;
extern float current_cold_water, current_warm_water;
extern float pspeed_cold_water, pspeed_warm_water;
extern float pcurrent_cold_water, pcurrent_warm_water;

extern char WriteOut[];
extern RTC_DateTimeTypeDef PRTC_DateTime;
extern RTC_DateTimeTypeDef PSleepRTC_DateTime;

////////////////////////////////////////////////////////////////////////////////
void RCapRect(int tx1, int ty1, int tw1, int th1);
void Draw_Month_Measurements(void);
void Erase_measurements(void);

void Draw_Main_screen(void);
void Write_to_Work_screen(void);
void Draw_SleepMode_Clock(void);
void Draw_waitScreen(void);
void Write_Reset_Month_Measurements_and_Speed(void);
void Draw_Menu(void);

void Draw_Menu_Clock(uint8_t opacity);
void Draw_Menu_Date(uint8_t opacity);
void Draw_Menu_SpeedUnit(uint8_t opacity);
void Draw_Menu_ResetCurrentTime(uint8_t opacity);
void Draw_Menu_ExitMenuTime(uint8_t opacity);
void Draw_Menu_SleepTime(uint8_t opacity);
void Reset_Month_Measurements(uint8_t opacity);
void Write_to_SD_Card(uint8_t opacity);
void Draw_Menu_Exit(uint8_t opacity);

void increment_line(void);
void decrement_line(void);
void increment_depth(void);
////////////////////////////////////////////////////////////////////////////////

#endif  //Draw.h