#ifndef __RTC_H
#define __RTC_H

////////////////////////////////////////////////////////////////////////////////

//extern __IO uint32_t TimeDisplay;
extern uint32_t      rtccnt;
extern uint16_t      year;
#define start_year              1970
#define current_year            year
#define JULIAN_DATE_BASE        2440588
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   uint8_t RTC_Hours;
   uint8_t RTC_Minutes;
   uint8_t RTC_Seconds;
   uint8_t RTC_Date;
   uint8_t RTC_Wday;
   uint8_t RTC_Month;
   uint16_t RTC_Year;
}RTC_DateTimeTypeDef;


void Time_Display(uint32_t TimeVar);
void Time_View(void);
unsigned char RTC_Init(void);
void RTC_GetDateTime(uint32_t RTC_Counter, RTC_DateTimeTypeDef* RTC_DateTimeStruct);
uint32_t RTC_GetRTC_Counter(RTC_DateTimeTypeDef* RTC_DateTimeStruct);

////////////////////////////////////////////////////////////////////////////////

#endif // __RTC_H
