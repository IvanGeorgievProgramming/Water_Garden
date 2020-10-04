#include <MCU_config.h>
#include "main.h"
#include "Draw.h"
#include "button.h"
#include "delay.h"
#include "dispcolor.h"

#define DISPLAY DISPLAY_240x240

////////////////////////////////////////////////////////////////////////////////
uint32_t impulses_warm_water = 0, impulses_cold_water = 0;
uint32_t impulses_warm_speed = 0, impulses_cold_speed = 0;
uint32_t month_warm_water = 0, month_cold_water = 0;
uint32_t pmonth_cold_water = 0, pmonth_warm_water = 0;

uint8_t  hour= 1, sec = 0, min = 1, Max_days_for_month = 31;
uint32_t cntCurrentTime = 20000, cntMenuTime= 20000, cntSleepTime= 185000;
uint8_t  cntSpeedUnit = 1;
uint8_t  line = 1, depth = 0;

uint16_t reload_data;     // to save Month data & Reset times to Backup registers (BKP)


uint8_t flag_pressed_button = 0, flag_show_main_screen = 0, flag_Done_Exit = 1, flag_write_SDcard = 0, flag_Show_Measurements = 1,  flag_in_sleep = 0;
uint8_t currentMatrix = 0;

RTC_DateTimeTypeDef RTC_DateTime;
RTC_DateTimeTypeDef PRTC_DateTime;
RTC_DateTimeTypeDef PSleepRTC_DateTime;

float speed_cold_water= 10, speed_warm_water= 2;
float current_cold_water= 10, current_warm_water= 0;
float pspeed_cold_water= 0, pspeed_warm_water= 0;
float pcurrent_cold_water= 0, pcurrent_warm_water= 0;

char WriteOut[5]= {0x30,0x30,0x30,0x30,0x30};


////////////////////////////////////////////////////////////////////////////////
void Save_cntCurrentTime();
void Save_cntMenuTime();
void Save_cntSleepTime();
void menu(void);
void func_MENU(void);
void Max_Days(void);
int Comparator(void);

////////////////////////////////////////////////////////////////////////////////
void main(){
    SystemInit();
    MCU_Init(); 
    
    Data_Water_Expense.full_struct = FALSE;
    GPIO_SetBits(LED_Port, LED_Pin);

    if(BKP_ReadBackupRegister(BKP_DR1) == 0xA5A5){      // Load if saved Month data & Reset times
        reload_data = BKP_ReadBackupRegister(BKP_DR2);
        month_warm_water = (uint32_t)reload_data;
        reload_data = BKP_ReadBackupRegister(BKP_DR3);
        month_cold_water = (uint32_t)reload_data;

        reload_data = BKP_ReadBackupRegister(BKP_DR5);
        cntCurrentTime = (uint32_t)reload_data;
        cntCurrentTime = cntCurrentTime << 16;
        reload_data = BKP_ReadBackupRegister(BKP_DR4);
        cntCurrentTime |= (uint32_t)reload_data;

        reload_data = BKP_ReadBackupRegister(BKP_DR7);
        cntMenuTime = (uint32_t)reload_data;
        cntMenuTime = cntMenuTime << 16;
        reload_data = BKP_ReadBackupRegister(BKP_DR6);
        cntMenuTime |= (uint32_t)reload_data;

        reload_data = BKP_ReadBackupRegister(BKP_DR9);
        cntSleepTime = (uint32_t)reload_data;
        cntSleepTime = cntSleepTime << 16;
        reload_data = BKP_ReadBackupRegister(BKP_DR8);
        cntSleepTime |= (uint32_t)reload_data;

    }else{
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
        BKP_WriteBackupRegister(BKP_DR2, month_warm_water);
        BKP_WriteBackupRegister(BKP_DR3, month_cold_water);

        reload_data = (uint16_t)cntCurrentTime;
        BKP_WriteBackupRegister(BKP_DR4, reload_data);
        reload_data = (uint16_t)(cntCurrentTime >> 16);
        BKP_WriteBackupRegister(BKP_DR5, reload_data);

        reload_data = (uint16_t)cntMenuTime;
        BKP_WriteBackupRegister(BKP_DR6, reload_data);
        reload_data = (uint16_t)(cntMenuTime >> 16);
        BKP_WriteBackupRegister(BKP_DR7, reload_data);

        reload_data = (uint16_t)cntSleepTime;
        BKP_WriteBackupRegister(BKP_DR8, reload_data);
        reload_data = (uint16_t)(cntSleepTime >> 16);
        BKP_WriteBackupRegister(BKP_DR9, reload_data);

    }

    while(1){
        RTC_GetDateTime(RTC_GetCounter(), &RTC_DateTime);

        if(depth == 0 && flag_in_sleep == 0){
            if(flag_show_main_screen == 0){
                flag_show_main_screen= 1;
                Draw_Main_screen();
                Draw_Month_Measurements();
            }

            Write_to_Work_screen();
        }

        if(flag_write_SDcard == 1){
            flag_write_SDcard= 0;
            Draw_waitScreen();  
            GPIO_ResetBits(GPIOA, LED_Pin);
            Save_Data();
            GPIO_SetBits(GPIOA, LED_Pin);
            //if(wait_multi_ms(3000, 6)){
            //    delay_ms(2000);
            depth = 1;
            Draw_Menu();
            Write_to_SD_Card(1);
            //}
        }

        if(flag_in_sleep == 1)  Draw_SleepMode_Clock();

        make_current_water();
        menu();
#if 1
        if(Data_Water_Expense.full_struct == TRUE){
            GPIO_ResetBits(LED_Port, LED_Pin);
        }else{
            GPIO_SetBits(LED_Port, LED_Pin);
            
        }
        
        /*if(wait_ms_ch(channelTestLed, 5000)){
            Data_Water_Expense.full_struct = FALSE;
        }*/
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
void Save_cntCurrentTime(){
    reload_data = (uint16_t)cntCurrentTime;
    BKP_WriteBackupRegister(BKP_DR4, reload_data);
    reload_data = (uint16_t)(cntCurrentTime >> 16);
    BKP_WriteBackupRegister(BKP_DR5, reload_data);
}

void Save_cntMenuTime(){
    reload_data = (uint16_t)cntMenuTime;
    BKP_WriteBackupRegister(BKP_DR6, reload_data);
    reload_data = (uint16_t)(cntMenuTime >> 16);
    BKP_WriteBackupRegister(BKP_DR7, reload_data);
}

void Save_cntSleepTime(){
    reload_data = (uint16_t)cntSleepTime;
    BKP_WriteBackupRegister(BKP_DR8, reload_data);
    reload_data = (uint16_t)(cntSleepTime >> 16);
    BKP_WriteBackupRegister(BKP_DR9, reload_data);
}

////////////////////////////////////////////////////////////////////////////////
int Comparator(){  
    if(PRTC_DateTime.RTC_Seconds !=  RTC_DateTime.RTC_Seconds) return 1;
    if(PRTC_DateTime.RTC_Minutes !=  RTC_DateTime.RTC_Minutes) return 1;
    if(PRTC_DateTime.RTC_Hours   !=  RTC_DateTime.RTC_Hours) return 1;
    if(PRTC_DateTime.RTC_Date    !=  RTC_DateTime.RTC_Date) return 1;
    if(PRTC_DateTime.RTC_Month   !=  RTC_DateTime.RTC_Month) return 1;
    if(PRTC_DateTime.RTC_Year    !=  RTC_DateTime.RTC_Year) return 1;  
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void Max_days(){
    float month = RTC_DateTime.RTC_Month;
  
    if(RTC_DateTime.RTC_Month == 2){
        if(((float)RTC_DateTime.RTC_Year / 4) != ((RTC_DateTime.RTC_Year / 4))) Max_days_for_month = 28;
        else                                                                    Max_days_for_month = 29;
    }else if(month == 4 || month == 6 || month == 9 || month == 11)             Max_days_for_month = 30;
    else                                                                        Max_days_for_month = 31;
}

////////////////////////////////////////////////////////////////////////////////
void  func_MENU(){
  
    if(depth == 0){
        if(flag_in_sleep == 1){
            flag_in_sleep = 0;
            Draw_Main_screen();
            return;
        }else{
            depth = 1;
            flag_Done_Exit = 0;
            Draw_Menu();
        }
    }else increment_depth();
      
    switch(line){ 
        case 1:                      
            Draw_Menu_Clock(depth);
            break;
        case 2:  
            Draw_Menu_Date(depth);
            break;
        case 3:  
            Draw_Menu_SpeedUnit(depth);
            break;
        case 4:  
            Draw_Menu_ResetCurrentTime(depth);
            break;
        case 5:  
            Draw_Menu_ExitMenuTime(depth);
            break;
        case 6:  
            Draw_Menu_SleepTime(depth);
            break;
        case 7:
            Reset_Month_Measurements(depth);           
            break;
        case 8:
            Write_to_SD_Card(depth);
            break;
        case 9:
            flag_show_main_screen = 0;
            flag_Done_Exit= 1;
            line = 1;
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void menu(){

    currentMatrix = readBtnSetupMatrix();

    PRTC_DateTime.RTC_Seconds =  RTC_DateTime.RTC_Seconds; 
    PRTC_DateTime.RTC_Minutes =  RTC_DateTime.RTC_Minutes;
    PRTC_DateTime.RTC_Hours   =  RTC_DateTime.RTC_Hours;     
    PRTC_DateTime.RTC_Date    =  RTC_DateTime.RTC_Date;   
    PRTC_DateTime.RTC_Month   =  RTC_DateTime.RTC_Month; 
    PRTC_DateTime.RTC_Year    =  RTC_DateTime.RTC_Year;      
    
    
    if(currentMatrix == 0){
        flag_pressed_button = 0;

        if(wait_ms_ch(channelCurrentTime, cntCurrentTime) && depth == 0 && flag_in_sleep == 0){
            flag_Show_Measurements = 0;
            Erase_measurements();
        }
        
        if(wait_ms_ch(channelMenuTime, cntMenuTime) && flag_Done_Exit == 0 && flag_in_sleep == 0){
            depth = 0;
            flag_show_main_screen = 0;
            flag_Done_Exit = 1;
        }
      
        if(wait_ms_ch(channelSleepTime, cntSleepTime) && depth == 0 && flag_in_sleep == 0){
            flag_in_sleep = 1;
            flag_Done_Exit = 1;
            dispcolor_FillScreen(Sleep_clock_background);
            PSleepRTC_DateTime.RTC_Hours = RTC_DateTime.RTC_Hours - 1;
            PSleepRTC_DateTime.RTC_Minutes = RTC_DateTime.RTC_Minutes - 1;
            current_warm_water = 0;
            current_cold_water = 0;
        }
    }
    
    if(currentMatrix){
        reset_wait_ms_ch(channelCurrentTime);
        reset_wait_ms_ch(channelMenuTime);
        reset_wait_ms_ch(channelSleepTime);
    }

    if(currentMatrix == MENU){                                                  //button MENU
        if(flag_pressed_button == 0){
            flag_pressed_button = 1;
            func_MENU();
        }
    }

    if(currentMatrix == UP && flag_pressed_button == 0){                        //button UP
        flag_pressed_button = 1;
        if(depth == 1){
            decrement_line();
         
            switch(line){
                case 1:
                    Draw_Menu_Date(0);
                    Draw_Menu_Clock(1);
                    break;
                case 2:
                    Draw_Menu_SpeedUnit(0);
                    Draw_Menu_Date(1);
                    break;
                case 3: 
                    Draw_Menu_ResetCurrentTime(0);
                    Draw_Menu_SpeedUnit(1);
                    break;
                case 4:  
                    Draw_Menu_ExitMenuTime(0);
                    Draw_Menu_ResetCurrentTime(1);
                    break;
                case 5:  
                    Draw_Menu_SleepTime(0);
                    Draw_Menu_ExitMenuTime(1);
                    break;
                case 6:  
                    Reset_Month_Measurements(0);
                    Draw_Menu_SleepTime(1);
                    break;
                case 7:  
                    Write_to_SD_Card(0);
                    Reset_Month_Measurements(1);
                    break;
                case 8:
                    Draw_Menu_Exit(0);
                    Write_to_SD_Card(1);
                    break;
                case 9:
                    Draw_Menu_Clock(0);
                    Draw_Menu_Exit(1);
                    break;
            }         
        }else if(depth > 1){         
            switch(line){
                case 1:
                    if(depth == 2){
                        if(RTC_DateTime.RTC_Hours < 23)    RTC_DateTime.RTC_Hours++;
                        else                               RTC_DateTime.RTC_Hours = 0;
                    }else if(depth == 3){
                        if(RTC_DateTime.RTC_Minutes < 59)  RTC_DateTime.RTC_Minutes++;
                        else                               RTC_DateTime.RTC_Minutes = 0;
                    }
                    Draw_Menu_Clock(depth);
                    break;                    
                case 2:
                    if(depth == 2){
                        if(RTC_DateTime.RTC_Date < Max_days_for_month)  RTC_DateTime.RTC_Date++;
                        else                                            RTC_DateTime.RTC_Date = 1;             
                    }else if(depth == 3){
                        if(RTC_DateTime.RTC_Month < 12)                 RTC_DateTime.RTC_Month++;
                        else                                            RTC_DateTime.RTC_Month = 1;
                    }else if(depth == 4){
                        if(RTC_DateTime.RTC_Year < 2031)                RTC_DateTime.RTC_Year++;
                        else                                            RTC_DateTime.RTC_Year= 1970;
                    }
                    Draw_Menu_Date(depth);
                    break;                    
                case 3:
                    if(cntSpeedUnit < 2)         cntSpeedUnit++;
                    else                         cntSpeedUnit = 0;
                    Draw_Menu_SpeedUnit(2);
                    break;                    
                case 4:  
                    func_resettime();
                    Draw_Menu_ResetCurrentTime(depth);
                    Save_cntCurrentTime();
                    break;                    
                case 5:  
                    func_resettime();
                    Draw_Menu_ExitMenuTime(depth);
                    Save_cntMenuTime();
                    break;                    
                case 6:  
                    func_resettime();
                    Draw_Menu_SleepTime(depth);
                    Save_cntSleepTime();
                    break;                   
                case 7:
                    month_warm_water = 0;
                    month_cold_water = 0;
                    Draw_waitScreen();
                    Draw_Menu();
                    break;
                case 8:
                    flag_write_SDcard= 1;
                    break;
            }              
        }
        Max_days();
    }
    

    if(currentMatrix == 4 && flag_pressed_button == 0){                         // button DOWN        
       flag_pressed_button = 1;
       Max_days();
       if(depth == 1){
         increment_line();
         
         switch(line){
           case 1:  
             Draw_Menu_Exit(0);
             Draw_Menu_Clock(1);
             break;
           case 2:  
             Draw_Menu_Clock(0);
             Draw_Menu_Date(1);
             break;
           case 3:  
             Draw_Menu_Date(0);
             Draw_Menu_SpeedUnit(1);
             break;
           case 4:  
             Draw_Menu_SpeedUnit(0);
             Draw_Menu_ResetCurrentTime(1);
             break;
           case 5:  
             Draw_Menu_ResetCurrentTime(0);
             Draw_Menu_ExitMenuTime(1);
             break;
           case 6:  
             Draw_Menu_ExitMenuTime(0);
             Draw_Menu_SleepTime(1);
             break;
           case 7:
             Draw_Menu_SleepTime(0);
             Reset_Month_Measurements(1);
             break;
           case 8:
             Reset_Month_Measurements(0);
             Write_to_SD_Card(1);
             break;
           case 9:
             Write_to_SD_Card(0);
             Draw_Menu_Exit(1);
             break;
         }
       }else if(depth > 1){
           switch(line){
              case 1:
                  if(depth == 2){
                      if(RTC_DateTime.RTC_Hours > 0)            RTC_DateTime.RTC_Hours--;
                      else                                      RTC_DateTime.RTC_Hours = 23;
                  }else if(depth == 3){           
                      if(RTC_DateTime.RTC_Minutes > 0)          RTC_DateTime.RTC_Minutes--;
                      else                                      RTC_DateTime.RTC_Minutes = 59; 
                  }
                  Draw_Menu_Clock(depth);
                  break;
              case 2:
                  if(depth == 2){
                      if(RTC_DateTime.RTC_Date > 1)             RTC_DateTime.RTC_Date--;
                      else                                      RTC_DateTime.RTC_Date = Max_days_for_month;
                  }else if(depth == 3){
                      if(RTC_DateTime.RTC_Month > 1)            RTC_DateTime.RTC_Month--;
                      else                                      RTC_DateTime.RTC_Month = 12;
                  }else if(depth == 4){
                      if(RTC_DateTime.RTC_Year > 1970)          RTC_DateTime.RTC_Year--;
                      else                                      RTC_DateTime.RTC_Year = 2031;
                  }
                  Draw_Menu_Date(depth);
                  break;           
              case 3: 
                  if(cntSpeedUnit > 0)         cntSpeedUnit--;
                  else                         cntSpeedUnit = 2;
                  Draw_Menu_SpeedUnit(2);
                  break;           
              case 4:  
                  func_resettime();
                  Draw_Menu_ResetCurrentTime(depth);
                  Save_cntCurrentTime();
                  break;
              case 5:  
                  func_resettime();
                  Draw_Menu_ExitMenuTime(depth);
                  Save_cntMenuTime();
                  break;
              case 6:
                  func_resettime();
                  Draw_Menu_SleepTime(depth);
                  Save_cntSleepTime();
                  break;
           }       
       }
       Max_days();
    }
    
    if(Comparator()){
      RTC_SetCounter(RTC_GetRTC_Counter(&RTC_DateTime));
      RTC_WaitForLastTask();
    }
}
////////////////////////////////////////////////////////////////////////////////