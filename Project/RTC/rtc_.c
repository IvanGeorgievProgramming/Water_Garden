#include "stm32f10x.h"

////////////////////////////////////////////////////////////////////////////////

#include "rtc_.h"

////////////////////////////////////////////////////////////////////////////////

__IO uint32_t TimeDisplay = 0;

////////////////////////////////////////////////////////////////////////////////
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // ok

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second Interrupt */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

uint32_t Time_Regulate(void)
{
  uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

  while (Tmp_HH == 0xFF)
  {
    Tmp_HH = 23;
  }
  while (Tmp_MM == 0xFF)
  {
    Tmp_MM = 59;
  }
  while (Tmp_SS == 0xFF)
  {
    Tmp_SS = 50;
  }

  /* Return the value to store in RTC counter register */
  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

void Time_Adjust(void)
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
//  RTC_SetCounter(Time_Regulate());
    RTC_SetCounter(rtccnt);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

struct {
    uint32_t THH;
    uint32_t TMM;
    uint32_t TSS;
} time;

/**
  * @brief  Displays the current time.
  * @param  TimeVar: RTC counter value.
  * @retval None
  */
void Time_Display(uint32_t TimeVar)
{
  uint32_t THH = 0, TMM = 0, TSS = 0;  
  /* Reset RTC Counter when Time is 23:59:59 */
  if (RTC_GetCounter() == 0x0001517F)
  {
     RTC_SetCounter(0x00);
     /* Wait until last write operation on RTC registers has finished */
     RTC_WaitForLastTask();
  }
  
  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

  time.THH = THH;
  time.TMM = TMM;
  time.TSS = TSS;

}

/**
  * @brief  Shows the current time (HH:MM:SS) on the Hyperterminal.
  * @param  None
  * @retval None
  */   
void Time_View(void)
{
    /* If 1s has been elapsed */
    if (TimeDisplay == 1)
    {
        /* Display current time */
        Time_Display(rtccnt);
        TimeDisplay = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void RTC_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////

unsigned char RTC_Init(void){
#if 1
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);  // Enable PWR and BKP clocks
    
    // PWR_BackupAccessCmd(ENABLE);
    PWR->CR |= PWR_CR_DBP;                                      // Disable Backup Domain write protection

    if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN){
        RCC->BDCR |= RCC_BDCR_BDRST;                              // Forces the Backup domain reset
        RCC->BDCR &= ~RCC_BDCR_BDRST;                             // Releases the Backup domain reset

//      RCC_LSEConfig(RCC_LSE_ON);
        RCC->BDCR |= RCC_BDCR_LSEON;                              // Enable the LSE clock
//      while ((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY) {}
        while (!(RCC->BDCR & RCC_BDCR_LSERDY)) {}                 // Poll the LSERDY flag until the LSE clock is ready
      
//      RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;                         // Select LSE as the RTC clock source
//      RCC_RTCCLKCmd(ENABLE);
        RCC->BDCR |= RCC_BDCR_RTCEN;                              // Enable the RTC clock

        RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;                      // Clear RSF flag
        while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET) {}   // Loop until RSF flag is set
    
        RTC_WaitForLastTask();                                    // Wait until last write operation on RTC registers has finished
    
        RTC->CRH |= RTC_CRH_SECIE;                                // Enable the RTC Second Interrupt
        RTC_WaitForLastTask();                                    // Wait until last write operation on RTC registers has finished

//      RTC_SetPrescaler(0x7FFF);
        RTC_SetPrescaler(32767);                                  // RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
        RTC_WaitForLastTask();                                    // Wait until last write operation on RTC registers has finished

      
//      RTC_WaitForSynchro();
        //PWR->CR &= ~PWR_CR_DBP;                                   // Enable Backup Domain write protection
      
        return 1;
    }
#endif


#if 0
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */
    /* RTC Configuration */
    RTC_Configuration();
    /* Adjust time by values entered by the user on the hyperterminal */
    Time_Adjust();

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
    }

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    //RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }
#endif

  RTC_NVIC_Configuration();
  return 0;
}

//–-----------------------------------------------------------------------------
// Get current date
void RTC_GetDateTime(uint32_t RTC_Counter, RTC_DateTimeTypeDef* RTC_DateTimeStruct){   // &RTC_DateTime
   unsigned long time;
   unsigned long t1, a, b, c, d, e, m;
   int year = 0;
   int mon = 0;
   int wday = 0;
   int mday = 0;
   int hour = 0;
   int min = 0;
   int sec = 0;
   uint64_t jd = 0;;
   uint64_t jdn = 0;

   jd = ((RTC_Counter+43200)/(86400>>1)) + (2440587<<1) + 1;
   jdn = jd>>1;

   time = RTC_Counter;
   t1 = time/60;
   sec = time - t1*60;

   time = t1;
   t1 = time/60;
   min = time - t1*60;

   time = t1;
   t1 = time/24;
   hour = time - t1*24;

   wday = jdn%7;

   a = jdn + 32044;
   b = (4*a+3)/146097;
   c = a - (146097*b)/4;
   d = (4*c+3)/1461;
   e = c - (1461*d)/4;
   m = (5*e+2)/153;
   mday = e - (153*m+2)/5 + 1;
   mon = m + 3 - 12*(m/10);
   year = 100*b + d - 4800 + (m/10);

   RTC_DateTimeStruct->RTC_Year = year;
   RTC_DateTimeStruct->RTC_Month = mon;
   RTC_DateTimeStruct->RTC_Date = mday;
   RTC_DateTimeStruct->RTC_Hours = hour;
   RTC_DateTimeStruct->RTC_Minutes = min;
   RTC_DateTimeStruct->RTC_Seconds = sec;
   RTC_DateTimeStruct->RTC_Wday = wday;
}
////////////////////////////////////////////////////////////////////////////////

uint32_t RTC_GetRTC_Counter(RTC_DateTimeTypeDef* RTC_DateTimeStruct) {
   uint8_t a;
   uint16_t y;
   uint8_t m;
   uint32_t JDN;

   a=(14-RTC_DateTimeStruct->RTC_Month)/12;
   y=RTC_DateTimeStruct->RTC_Year+4800-a;
   m=RTC_DateTimeStruct->RTC_Month+(12*a)-3;

   JDN=RTC_DateTimeStruct->RTC_Date;
   JDN+=(153*m+2)/5;
   JDN+=365*y;
   JDN+=y/4;
   JDN+=-y/100;
   JDN+=y/400;
   JDN = JDN -32045;
   JDN = JDN - JULIAN_DATE_BASE;
   JDN*=86400;
   JDN+=(RTC_DateTimeStruct->RTC_Hours*3600);
   JDN+=(RTC_DateTimeStruct->RTC_Minutes*60);
   JDN+=(RTC_DateTimeStruct->RTC_Seconds);

   return JDN;
}
////////////////////////////////////////////////////////////////////////////////
