#include "Convert.h"
#include "delay.h"

////////////////////////////////////////////////////////////////////////////////
bool start_warm_measurements = FALSE;
bool start_cold_measurements = FALSE;

////////////////////////////////////////////////////////////////////////////////
void make_current_water(void){

    if(impulses_warm_water >= Water_meter_const / 10){
        reset_wait_ms_ch(channelSleepTime);     
        reset_wait_ms_ch(channelCurrentTime);
        
        if(start_warm_measurements == FALSE && Data_Water_Expense.full_struct == FALSE){
            start_warm_measurements = TRUE;
            Data_Water_Expense.start_warm_expense_time = RTC_GetCounter();
        }
        
        if(flag_in_sleep == 1){
            flag_in_sleep = 0;
            flag_show_main_screen= 0;
        }
        
        float  liters = (int)((impulses_warm_water * 10 )/ Water_meter_const);
        liters /= 10;
        current_warm_water += liters;
        impulses_warm_water = impulses_warm_water - liters * Water_meter_const;
    }
    //////////////////////////////////////////////////
    if(impulses_cold_water >= Water_meter_const / 10){
        reset_wait_ms_ch(channelSleepTime);     
        reset_wait_ms_ch(channelCurrentTime);
        
        if(start_cold_measurements == FALSE && Data_Water_Expense.full_struct == FALSE){
            start_cold_measurements = TRUE;
            Data_Water_Expense.start_cold_expense_time = RTC_GetCounter();
        }
        
        if(flag_in_sleep == 1){
            flag_in_sleep = 0;
            flag_show_main_screen= 0;
        }
        
        float  liters = (int)((impulses_cold_water * 10 )/ Water_meter_const);
        liters /= 10;
        current_cold_water += liters;
        impulses_cold_water = impulses_cold_water - liters * Water_meter_const;
    }
}

////////////////////////////////////////////////////////////////////////////////
void func_resettime(){
    static uint32_t *p;

    if(line == 4) p = &cntCurrentTime;        
    if(line == 5) p = &cntMenuTime;
    if(line == 6) p = &cntSleepTime;  
  
    if(currentMatrix == 2){                                                     // button Up is pressed
        if(depth == 2){
            if(*p < 60000*59) *p += 60000;
            else              *p = *p - ((int)(*p / 60000)*60000); 
        }else if(depth == 3){      
            if((*p - ((int)(*p / 60000))*60000) == 59000)    *p -= 59000;
            else                                             *p += 1000; 
        }
    }  
  
    if(currentMatrix == 4){                                                     // button Down is pressed  
        if(depth == 2){      
        if(*p >= 60000)   *p -= 60000;
        else              *p += 60000*59;      
    }else if(depth == 3){ 
        if((*p - (((int)(*p / 60000))*60000)) == 0)      *p += 59000;
        else                                             *p -= 1000; 
    }    
  }    
}

////////////////////////////////////////////////////////////////////////////////
void convert_month_char(uint32_t value){
    uint32_t tens = 0, hundreds = 0, thousands = 0, ten_thousands = 0;
    
    if(value >= 10000){
        ten_thousands = value / 10000;
        value = value - ten_thousands * 10000;
        WriteOut[0] = ten_thousands + 0x30;
    }else  WriteOut[0]=0x20;
    
    if(value >= 1000){
        thousands = value / 1000;
        value = value - thousands * 1000;
        WriteOut[1] = thousands + 0x30;
    }else if(WriteOut[0] > 0x30)      WriteOut[1] = 0x30;
    else                          WriteOut[1] = 0x20;
      
    if(value >= 100){
        hundreds = value / 100;
        value = value - hundreds * 100;
        WriteOut[2] = hundreds + 0x30;
    }else if(WriteOut[1] >= 0x30)     WriteOut[2] = 0x30;
    else                          WriteOut[2] = 0x20;

    if(value >= 10){
        tens = value / 10;
        value = value - tens * 10;
        WriteOut[3] = tens + 0x30;
    }else if(WriteOut[2] >= 0x30)     WriteOut[3] = 0x30;
    else                          WriteOut[3] = 0x20;

    WriteOut[4] = value + 0x30;
} 

////////////////////////////////////////////////////////////////////////////////
void convert_speed_char(float speed){
    uint32_t tens= 0; 

    if(cntSpeedUnit == 1)  speed *= 60;
    if(cntSpeedUnit == 2)  speed *= 3600;
    
    if(speed >= 100){
        WriteOut[0] = 0x3A;
        WriteOut[1] = 0x3A;
        WriteOut[2] = 0x00;
        return;
    }else if(speed >= 10){
        tens = (uint32_t)speed / 10;
        speed = speed - tens * 10;
        WriteOut[0] = tens + 0x30;
        WriteOut[1] = (uint32_t)speed + 0x30;
        WriteOut[2] = 0x00;
    }else if(speed == (uint32_t) speed){
        WriteOut[0] = 0x20;
        WriteOut[1] = (uint32_t)speed + 0x30;
        WriteOut[2] = 0x00;
    }else{
        uint32_t remainder = (speed - (uint32_t)speed) * 10;    
        speed = (uint32_t)speed;
        WriteOut[0] = (uint32_t)speed + 0x30;      
        WriteOut[1] = ',';      
        WriteOut[2] = remainder + 0x30;      
        WriteOut[3] = 0x00;      
    }
} 

////////////////////////////////////////////////////////////////////////////////
void convert_current_char(float number){
    uint16_t hundreds = 0, tens = 0;
    uint16_t remainder = (number - (uint16_t)number) * 10;
    number = (uint16_t)number;
  
    if (number >= 100){
        hundreds = (uint16_t)number / 100;
        number = number - (hundreds * 100);
        WriteOut[0] = hundreds + 0x30;
    
        tens = (uint16_t)number / 10;
        number = number - (tens * 10);
        WriteOut[1] = tens + 0x30;
    
        WriteOut[2] = (uint16_t)number + 0x30;
        WriteOut[3] = ',';
        WriteOut[4] = remainder + 0x30;  
    }else{
        tens = (uint16_t)number / 10;
        number = number - (tens * 10);
        WriteOut[0] = 0x20;
        WriteOut[1] = tens + 0x30;

        WriteOut[2] = (uint16_t)number + 0x30;
        WriteOut[3] = ',';
        WriteOut[4] = remainder + 0x30;  
    }
}

////////////////////////////////////////////////////////////////////////////////
void convert_time_char(uint8_t number){
    uint8_t tens= 0;
    WriteOut[2] = 0;
  
    if(number >= 10){
        tens = number / 10;
        number = number - tens * 10;
        WriteOut[0] = tens + 0x30;
    }else  WriteOut[0] = 0x30;

    WriteOut[1] = number + 0x30;
}

////////////////////////////////////////////////////////////////////////////////
void convert_year_char(uint16_t number){
    uint8_t thousands = 0, hundreds = 0, tens = 0;  
    for(int counter = 0; counter < 5; counter++) WriteOut[counter] = 0;
  
    if(number >= 1000){
        thousands = number / 1000;
        number = number - thousands * 1000;
        WriteOut[0] = thousands + 0x30;
    }
      
    if(number >= 100){
        hundreds = number / 100;
        number = number - hundreds * 100;
        WriteOut[1] = hundreds + 0x30;
    }else  WriteOut[1] = 0x30;

    if(number >= 10){
        tens = number / 10;
        number = number - tens * 10;
        WriteOut[2] = tens + 0x30;
    }else  WriteOut[2] = 0x30;
  
    WriteOut[3] = number + 0x30;
    WriteOut[4] = 0x00;  
}

////////////////////////////////////////////////////////////////////////////////
void convert_mintime_char(uint32_t number){
    int tens = number / 60000;
    
    WriteOut[0] = (int)(tens / 10) + 0x30;
    tens = tens - ((int)(tens / 10)*10);
    WriteOut[1] = tens + 0x30;
    WriteOut[2] = 0x00;
}

////////////////////////////////////////////////////////////////////////////////
void convert_sectime_char(uint32_t number){
    uint32_t minutes = 0, seconds = 0;
    number /= 1000;
    minutes = number / 60;
    seconds = number - (uint32_t)(minutes * 60);
    WriteOut[0] = (uint32_t)(seconds / 10) + 0x30;
    WriteOut[1] = (seconds - ((uint32_t)(seconds / 10)*10)) + 0x30;
    WriteOut[2] = 0x00;
}

////////////////////////////////////////////////////////////////////////////////
void convert_date_char(uint32_t number){
  uint32_t tens = 0, hundreds = 0, thousands = 0, ten_thousands = 0;
    
    if(number >= 10000){
        ten_thousands = number / 10000;
        number = number - ten_thousands * 10000;
        WriteOut[0] = ten_thousands + 0x30;
    }else  WriteOut[0] = 0x20;
    
    if(number >= 1000){
        thousands = number / 1000;
        number = number - thousands * 1000;
        WriteOut[1] = thousands + 0x30;
    }else if(WriteOut[0]> 0x30)   WriteOut[1]=0x30;
    else                          WriteOut[1]=0x20;
      
    if(number >= 100){
        hundreds = number / 100;
        number = number - hundreds * 100;
        WriteOut[2] = hundreds + 0x30;
    }else if(WriteOut[1] >= 0x30) WriteOut[2]=0x30;
    else                          WriteOut[2]=0x20;

    if(number >= 10){
        tens = number / 10;
        number = number - tens * 10;
        WriteOut[3] = tens + 0x30;
    }else if(WriteOut[2] >= 0x30) WriteOut[3]=0x30;
    else                          WriteOut[3]=0x30;

    WriteOut[4] = number + 0x30; 
}

////////////////////////////////////////////////////////////////////////////////
void convert_min_char_SD(uint8_t min){
    int tens = (int)(min / 10);
    int ones = (min - (tens * 10));

    for(int counter = 0; counter < 5; counter++) WriteOut[counter] = 0;
  
    if(tens > 0){ 
        WriteOut[0] = tens + 0x30; 
        WriteOut[1] = ones + 0x30;
    }else{
        WriteOut[0] = ones + 0x30;
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////
void convert_year_char_SD(uint16_t year){
    int thousand = (int)(year / 1000);
    int hundreds = (int)((year - (thousand * 1000)) / 100);
    int tens = (int)(((year - (thousand * 1000)) - (hundreds * 100)) / 10);
    int ones = (((year - (thousand * 1000)) - (hundreds * 100)) - (tens * 10));  

    for(int counter = 0; counter < 5; counter++) WriteOut[counter] = 0;

    WriteOut[0] = thousand + 0x30;
    WriteOut[1] = hundreds + 0x30;
    WriteOut[2] = tens + 0x30;
    WriteOut[3] = ones + 0x30;
    return;
}

////////////////////////////////////////////////////////////////////////////////
