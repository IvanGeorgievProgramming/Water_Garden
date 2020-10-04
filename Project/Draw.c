#include "Draw.h"
#include "systick_timer.h"
#include "delay.h"
#include "Convert.h"
#include "font.h"
#include "dispcolor.h"
#include "button.h"

////////////////////////////////////////////////////////////////////////////////
uint32_t pre_time1 = 0, time1 = 0; 

////////////////////////////////////////////////////////////////////////////////
void RCapRect(int tx1, int ty1, int tw1, int th1){  
    dispcolor_FillRect(tx1, ty1, 10, 10, RGB565(67, 221, 245));
    dispcolor_FillRect(tx1+tw1-10, ty1, 10, 10, RGB565(67, 221, 245));
    dispcolor_FillRect(tx1, ty1+th1-10, 10, 10, RGB565(67, 221, 245));
    dispcolor_FillRect(tx1+tw1-10, ty1+th1-10, 10, 10, RGB565(67, 221, 245));
  
    dispcolor_DrawCircleFilled(tx1+10, ty1+10, 9, RGB565(255, 27, 157));
    dispcolor_DrawCircleFilled(tx1+tw1-10, ty1+10, 9, RGB565(95, 95, 205));
    dispcolor_DrawCircleFilled(tx1+10, ty1+th1-10, 9, RGB565(255, 27, 157));
    dispcolor_DrawCircleFilled(tx1+tw1-10, ty1+th1-10, 9, RGB565(95, 95, 205));
}

void Draw_Main_screen(void){

    dispcolor_FillScreen(Background_color);
  
    dispcolor_DrawCircleFilled(160, 120, 75, RGB565(20, 0, 255));
    dispcolor_DrawCircleFilled(80, 120, 75, RGB565(246, 182, 34));
    dispcolor_DrawCircleFilled(175, 120, 60,  RGB565(20, 0, 255));
  
    dispcolor_FillRect(10, 7, 110, 44,  RGB565(255, 27, 157));
    dispcolor_FillRect(120, 7, 110, 44, RGB565(95, 95, 205));
    RCapRect(x1, y1, w1 + w2, h1);

    dispcolor_FillRect(10, 189, 110, 44,  RGB565(255, 27, 157));
    dispcolor_FillRect(120, 189, 110, 44, RGB565(95, 95, 205));
    RCapRect(x3, y3, w3 + w4, h3);
  
    dispcolor_FillRect(95, 179, 50, 20, RGB565(130, 169, 21));
    if(cntSpeedUnit == 0){
        dispcolor_printf(Water_unit_text_coordinates, FONTID_16F, Water_unit_text_color, "L / S");
    }else if(cntSpeedUnit == 1){
        dispcolor_printf(Water_unit_text_coordinates, FONTID_16F, Water_unit_text_color, "L / M");
    }else if(cntSpeedUnit == 2){
        dispcolor_printf(Water_unit_text_coordinates, FONTID_16F, Water_unit_text_color, "L / H");
    }
}

////////////////////////////////////////////////////////////////////////////////
void Write_to_Work_screen(void){  
  
    if(pmonth_warm_water != month_warm_water){
        convert_month_char(pmonth_warm_water);
        dispcolor_printf(35, 15, FONTID_32F, Warm_watermeter_color, WriteOut);
        convert_month_char(month_warm_water);
        dispcolor_printf(35, 15, FONTID_32F, WHITE, WriteOut);
        pmonth_warm_water = month_warm_water;
    }
  
    if(pmonth_cold_water != month_cold_water){
        convert_month_char(pmonth_cold_water);
        dispcolor_printf(145, 15, FONTID_32F, Cold_watermeter_color, WriteOut);
        convert_month_char(month_cold_water);
        dispcolor_printf(145, 15, FONTID_32F, WHITE, WriteOut);
        pmonth_cold_water = month_cold_water;
    }
  
    if(pspeed_warm_water != speed_warm_water){
        convert_speed_char(pspeed_warm_water);
        dispcolor_printf(30, 90, FONTID_64F, Warm_water_rect_color, WriteOut);
        convert_speed_char(speed_warm_water);
        dispcolor_printf(30, 90, FONTID_64F, WHITE, WriteOut);
        pspeed_warm_water = speed_warm_water;
    }
  
    if(pspeed_cold_water != speed_cold_water){
        convert_speed_char(pspeed_cold_water);
        dispcolor_printf(130, 90, FONTID_64F, Cold_water_rect_color, WriteOut);
        convert_speed_char(speed_cold_water);
        dispcolor_printf(130, 90, FONTID_64F, WHITE, WriteOut);
        pspeed_cold_water = speed_cold_water;
    }

 
    if(pcurrent_warm_water != current_warm_water && current_warm_water > 0){
        convert_current_char(pcurrent_warm_water);
        dispcolor_printf(35, 195, FONTID_32F, Warm_watermeter_color, WriteOut);
        convert_current_char(current_warm_water);
        dispcolor_printf(35, 195, FONTID_32F, WHITE, WriteOut);
        pcurrent_warm_water = current_warm_water;
    }

    if(pcurrent_cold_water != current_cold_water && current_cold_water > 0){
        convert_current_char(pcurrent_cold_water);
        dispcolor_printf(145, 195, FONTID_32F, Cold_watermeter_color,WriteOut);
        convert_current_char(current_cold_water);
        dispcolor_printf(145, 195, FONTID_32F, WHITE,WriteOut);
        pcurrent_cold_water = current_cold_water;
    }
}

void Draw_Month_Measurements(void){
    convert_month_char(month_warm_water);
    dispcolor_printf(Month_measurements_warm_water_text_coordinates, FONTID_32F, Text_color1, WriteOut);

    convert_month_char(month_cold_water);   
    dispcolor_printf(Month_measurements_cold_water_text_coordinates, FONTID_32F, Text_color1, WriteOut);
}

void Erase_measurements(void){
    convert_current_char(current_warm_water);
    dispcolor_printf(Month_measurements_warm_water_text_mask_coordinates, FONTID_32F, Warm_watermeter_color, WriteOut);

    convert_current_char(current_cold_water);
    dispcolor_printf(Month_measurements_cold_water_text_mask_coordinates, FONTID_32F, Cold_watermeter_color, WriteOut);

    convert_speed_char(speed_warm_water);
    dispcolor_printf(30, 90, FONTID_64F, Warm_water_rect_color, WriteOut);

    convert_speed_char(speed_cold_water);
    dispcolor_printf(130, 90, FONTID_64F, Cold_water_rect_color, WriteOut);

    current_warm_water = 0.0;
    current_cold_water = 0.0;
}

////////////////////////////////////////////////////////////////////////////////
void Draw_Menu(void){
    dispcolor_FillScreen(RGB565(0, 0, 0));
    Draw_Menu_Clock(0);
    Draw_Menu_Date(0);
    Draw_Menu_SpeedUnit(0);
    Draw_Menu_ResetCurrentTime(0);
    Draw_Menu_ExitMenuTime(0);
    Draw_Menu_SleepTime(0);
    Reset_Month_Measurements(0);
    Write_to_SD_Card(0);
    Draw_Menu_Exit(0);
}

////////////////////////////////////////////////////////////////////////////////
void Draw_Menu_Clock(uint8_t opacity){
  
    dispcolor_FillRect(10, 20, 240, 22, BLACK);
  
    if(opacity == 1)    dispcolor_FillRect(10, 20, 120, 20, BLUE);
    if(opacity == 2)    dispcolor_FillRect(163, 21, 27, 20, BLUE);
    if(opacity == 3)    dispcolor_FillRect(194, 21, 27, 20, BLUE);
 
    dispcolor_printf(15, 22, FONTID_16F, Text_color1, "Clock");  
    convert_time_char(RTC_DateTime.RTC_Hours);
    dispcolor_printf(165, 20, FONTID_24F, Text_color2, WriteOut);        
    dispcolor_printf(191, 20, FONTID_24F, Text_color2, ":");
    convert_time_char(RTC_DateTime.RTC_Minutes);
    dispcolor_printf(196, 20, FONTID_24F, Text_color2, WriteOut);
}

void Draw_Menu_Date(uint8_t opacity){
  
    dispcolor_FillRect(10, 40, 240, 22, BLACK);
    
    if(opacity == 1)    dispcolor_FillRect(10, 40, 90, 20, BLUE);
    if(opacity == 2)    dispcolor_FillRect(111, 41, 27, 20, BLUE);
    if(opacity == 3)    dispcolor_FillRect(142, 41, 27, 20, BLUE);
    if(opacity == 4)    dispcolor_FillRect(172, 41, 51, 20, BLUE);
   
    dispcolor_printf(15, 42, FONTID_16F, Text_color1, "Date");  
    convert_time_char(RTC_DateTime.RTC_Date);
    dispcolor_printf(114, 40, FONTID_24F, Text_color2, WriteOut);        
    dispcolor_printf(139, 40, FONTID_24F, Text_color2, ":");
    convert_time_char(RTC_DateTime.RTC_Month);
    dispcolor_printf(144, 40, FONTID_24F, Text_color2, WriteOut);
    dispcolor_printf(169, 40, FONTID_24F, Text_color2, ":");
    convert_year_char(RTC_DateTime.RTC_Year);
    dispcolor_printf(174, 40, FONTID_24F, Text_color2, WriteOut);
}

void Draw_Menu_SpeedUnit(uint8_t opacity){
  
    dispcolor_FillRect(10, 60, 240, 22, BLACK);
    
    if(opacity == 1)    dispcolor_FillRect(10, 60, 120, 20, BLUE);
    if(opacity == 2)    dispcolor_FillRect(153, 61, 70, 20, BLUE);
 
    dispcolor_printf(15, 62, FONTID_16F, Text_color1, "Speed Unit");
    if(cntSpeedUnit == 0)  dispcolor_printf(159, 63, FONTID_16F, Text_color2, "L / sec");
    if(cntSpeedUnit == 1)  dispcolor_printf(159, 63, FONTID_16F, Text_color2, "L / min");
    if(cntSpeedUnit == 2)  dispcolor_printf(159, 63, FONTID_16F, Text_color2, "L / hour");
}

void Draw_Menu_ResetCurrentTime(uint8_t opacity){
  
    dispcolor_FillRect(10, 80, 240, 22, BLACK);
    
    if(opacity == 1)    dispcolor_FillRect(10, 80, 150, 20, BLUE);
    if(opacity == 2)    dispcolor_FillRect(163, 81, 27, 20, BLUE);
    if(opacity == 3)    dispcolor_FillRect(194, 81, 27, 20, BLUE);
 
    dispcolor_printf(15, 82, FONTID_16F, Text_color1, "Reset current time");  
    convert_mintime_char(cntCurrentTime);
    dispcolor_printf(165, 80, FONTID_24F, Text_color2, WriteOut);        
    dispcolor_printf(191, 80, FONTID_24F, Text_color2, ":");
    convert_sectime_char(cntCurrentTime);
    dispcolor_printf(196, 80, FONTID_24F, Text_color2, WriteOut);
}

void Draw_Menu_ExitMenuTime(uint8_t opacity){
  
    dispcolor_FillRect(10, 100, 240, 22, BLACK);
    
    if(opacity == 1)    dispcolor_FillRect(10, 100, 140, 20, BLUE);
    if(opacity == 2)    dispcolor_FillRect(163, 101, 27, 20, BLUE);
    if(opacity == 3)    dispcolor_FillRect(194, 101, 27, 20, BLUE);
 
    dispcolor_printf(15, 102, FONTID_16F, Text_color1, "Reset menu time");  
    convert_mintime_char(cntMenuTime);
    dispcolor_printf(165, 100, FONTID_24F, Text_color2, WriteOut);        
    dispcolor_printf(191, 100, FONTID_24F, Text_color2, ":");
    convert_sectime_char(cntMenuTime);
    dispcolor_printf(196, 100, FONTID_24F, Text_color2, WriteOut);
}

void Draw_Menu_SleepTime(uint8_t opacity){
  
    dispcolor_FillRect(10, 120, 240, 22, BLACK);
    
    if(opacity == 1)    dispcolor_FillRect(10, 120, 90, 20, BLUE);
    if(opacity == 2)    dispcolor_FillRect(163, 121, 27, 20, BLUE);
    if(opacity == 3)    dispcolor_FillRect(194, 121, 27, 20, BLUE);
 
    dispcolor_printf( 15, 122, FONTID_16F, Text_color1, "Sleep time");  
    convert_mintime_char(cntSleepTime);
    dispcolor_printf(165, 120, FONTID_24F, Text_color2, WriteOut);        
    dispcolor_printf(191, 120, FONTID_24F, Text_color2, ":");
    convert_sectime_char(cntSleepTime);
    dispcolor_printf(196, 120, FONTID_24F, Text_color2, WriteOut);
}

void Reset_Month_Measurements(uint8_t opacity){
  
    dispcolor_FillRect(10, 140, 240, 22, BLACK);
    
    if(opacity == 1)    dispcolor_FillRect(10, 140, 240, 22, BLUE);
    if(opacity == 2)    dispcolor_FillRect(194, 140, 24, 20, BLUE);
 
    dispcolor_printf(15, 142, FONTID_16F, Text_color1, "Res. Month Measure.");
    dispcolor_printf(196, 142, FONTID_16F, Text_color1, "OK");
}

void Write_to_SD_Card(uint8_t opacity){
  
    dispcolor_FillRect(10, 160, 240, 22, BLACK);
    
    if(opacity == 1)    dispcolor_FillRect(10, 160, 240, 22, BLUE);
    if(opacity == 2)    dispcolor_FillRect(194, 160, 24, 20, BLUE);
 
    dispcolor_printf(15, 162, FONTID_16F, Text_color1, "Write to SD Card");
    dispcolor_printf(196, 162, FONTID_16F, Text_color1, "OK");
}

void Draw_Menu_Exit(uint8_t opacity){
  
    dispcolor_FillRect(10, 180, 240, 22, BLACK);
    
    if(opacity == 1)    dispcolor_FillRect(10, 180, 40, 20, BLUE);
  
    dispcolor_printf(15, 182, FONTID_16F, Text_color1, "Exit");  
}

////////////////////////////////////////////////////////////////////////////////
void Draw_waitScreen(void){
  
    //dispcolor_FillScreen(BLACK);
    dispcolor_FillRect(40, 80, 150, 85, RGB565(35, 80, 140));
    dispcolor_printf(90, 100, FONTID_16F, RGB565(127, 255, 0),   "Please");
    dispcolor_printf(90, 125, FONTID_16F, RGB565(127, 255, 0),   " wait ...");
    return;
}

////////////////////////////////////////////////////////////////////////////////
void Draw_SleepMode_Clock(void){
  
    if(RTC_DateTime.RTC_Hours != PSleepRTC_DateTime.RTC_Hours){
        convert_date_char(PSleepRTC_DateTime.RTC_Hours);
        
        WriteOut[1] = WriteOut[3];
        WriteOut[2] = WriteOut[4];
        WriteOut[3] = 0x0;
        
        dispcolor_printf(19, 95, FONTID_64F, Sleep_clock_background, WriteOut);
        convert_date_char(RTC_DateTime.RTC_Hours);    
        
        WriteOut[1] = WriteOut[3];
        WriteOut[2] = WriteOut[4];                
        WriteOut[3] = 0x0;
        
        dispcolor_printf(19, 95, FONTID_64F, Sleep_clock_color, WriteOut);
        PSleepRTC_DateTime.RTC_Hours = RTC_DateTime.RTC_Hours;
    }
  
    if(RTC_DateTime.RTC_Minutes != PSleepRTC_DateTime.RTC_Minutes){
        convert_date_char(PSleepRTC_DateTime.RTC_Minutes);    
        dispcolor_printf(31, 95, FONTID_64F, Sleep_clock_background, WriteOut);
        convert_date_char(RTC_DateTime.RTC_Minutes);    
        dispcolor_printf(31, 95, FONTID_64F, Sleep_clock_color, WriteOut);
        PSleepRTC_DateTime.RTC_Minutes = RTC_DateTime.RTC_Minutes;
    }

    dispcolor_printf(103, 95, FONTID_64F, Sleep_clock_color, ":");
    return;
}

////////////////////////////////////////////////////////////////////////////////
void increment_line(void){
    if(line < 9) line++;
    else line = 1;
}

void decrement_line(void){
    if(line == 1) line = 9;
    else          line--;
}

////////////////////////////////////////////////////////////////////////////////
void increment_depth(void){

    if(line == 2){      
        if(depth < 4) depth++;
        else depth= 1;
    }
    else if(line == 3){
        if(depth == 1) depth= 2;
        else depth= 1;    
    }
    else if(line == 7){
        if(depth == 1) depth = 2;
        else if(depth == 2) depth = 1;    
    }
    else if(line == 8){    
      if(depth == 1) depth= 2;
      else depth= 1;    
    }
    else if(line == 9) depth= 0;
    else{
        if(depth < 3) depth++;
        else depth= 1;   
    }
}
////////////////////////////////////////////////////////////////////////////////
