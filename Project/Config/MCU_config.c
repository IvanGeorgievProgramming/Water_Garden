
#include "MCU_config.h"
#include "dispcolor.h"
#include "tick_timer_isr.h"
#include "EXTI_isr.h"

////////////////////////////////////////////////////////////////////////////////

void PIN_CONFIG(GPIO_TypeDef* port, uint16_t pins, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed){
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin = pins; //GPIO_Pin_0..15 + GPIO_Pin_All
    GPIO_InitStruct.GPIO_Mode = mode;
    GPIO_InitStruct.GPIO_Speed = speed;
    GPIO_Init(port, &GPIO_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////

void RCC_Init(void){
    // Reset value: 0x0C000000, reset by system Reset, except reset flags by power Reset only
    //RCC->CSR = (0x0C000000);
    
    // Reset value: 0x00000000
    RCC->BDCR = (0x00000000);
    
    // SRAM interface clock enable
    // FLITF clock enable (Flash Memory Interface)
    //
    // Reset value: 0x00000014
    RCC->AHBENR = (RCC_AHBENR_FLITFEN | RCC_AHBENR_SRAMEN);

    // Timer 2 clock enabled
    // Timer 3 clock enabled
    // Backup interface clock enable
    // Power interface clock enable
    RCC->APB1ENR = (RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN);

    // Alternate Function I/O clock enable
    // I/O port A clock enable
    RCC->APB2ENR = (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN);
}

////////////////////////////////////////////////////////////////////////////////

void SysTickInit(uint16_t frequency){
    RCC_ClocksTypeDef RCC_Clocks;

    RCC_GetClocksFreq(&RCC_Clocks);
    (void) SysTick_Config(RCC_Clocks.HCLK_Frequency / frequency);
}

////////////////////////////////////////////////////////////////////////////////

void GPIOs_Init(void){

    PIN_CONFIG(LED_Port, LED_Pin, GPIO_Mode_Out_PP, GPIO_Speed_50MHz); // Led
    PIN_CONFIG(BUTTON_Port, MENU_Button_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // Button MENU input
    PIN_CONFIG(BUTTON_Port, UP_Button_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // Button UP input
    PIN_CONFIG(BUTTON_Port, DOWN_Button_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // Button DOWN input

    PIN_CONFIG(OPTIC_PWM_Port, OPTIC_Pwm_Pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz); // OPTIC_PWM Out
    //PIN_CONFIG(DataIn_Port, OPTIC_DataWarm_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // OPTIC_Warm_Data IN
    //PIN_CONFIG(DataIn_Port, HAL_DataWarm_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // HAL_Warm_Data IN
    PIN_CONFIG(GPIOB, OPTIC_DataWarm_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // OPTIC_Warm_Data IN
    PIN_CONFIG(GPIOB, HAL_DataWarm_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // HAL_Warm_Data IN

    PIN_CONFIG(DataIn_Port, OPTIC_DataCold_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // OPTIC_Cold_Data IN
    PIN_CONFIG(DataIn_Port, HAL_DataCold_Pin, GPIO_Mode_IPU, GPIO_Speed_50MHz); // HAL_Cold_Data IN

    GPIO_EventOutputConfig(0, 0); // Disable EVENTOUT output
}

////////////////////////////////////////////////////////////////////////////////

void MCU_Init(void){

    RCC_Init();

    // Init SysTick timer
    // 1000 -> 1000 us = 1.000 ms
    // 2000 ->  500 us = 0.500 ms
    // 4000 ->  250 us = 0.250 ms
    SysTickInit(1000);

    if(RTC_Init() == 1){        // If it is the first initialisation of RTC, then set these setings
      RTC_DateTime.RTC_Date = 19;
      RTC_DateTime.RTC_Month = 9;
      RTC_DateTime.RTC_Year = 2005;

      RTC_DateTime.RTC_Hours = 00;
      RTC_DateTime.RTC_Minutes = 05;
      RTC_DateTime.RTC_Seconds = 00;

      RTC_SetCounter(RTC_GetRTC_Counter(&RTC_DateTime));
    }
    
    dispcolor_Init(240, 240);   //Display initialisation

    GPIOs_Init();

    Timer2_Init();      // Init Timer2 as PWM Signal
    Timer3_Init();      // Init Timer3 as timer with resolution in ... seconds
    EXTInt_Init();      // Init OPTIC_Data IN & HAL_Data IN
}

////////////////////////////////////////////////////////////////////////////////
