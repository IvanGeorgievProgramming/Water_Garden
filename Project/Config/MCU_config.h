
#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H

////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#include "rtc_.h"

////////////////////////////////////////////////////////////////////////////////

#define LED_Port                GPIOA
#define LED_Pin                 GPIO_Pin_1  // Led

#define BUTTON_Port             GPIOA
#define MENU_Button_Pin         GPIO_Pin_8  // Button MENU
#define UP_Button_Pin           GPIO_Pin_9  // Button UP
#define DOWN_Button_Pin         GPIO_Pin_10  // Button DOWN

// WaterMeter Optic
#define OPTIC_PWM_Port          GPIOA
#define DataIn_Port             GPIOA
#define OPTIC_Pwm_Pin           GPIO_Pin_0  // OPTIC_PWM OUT    see:  timer_oc.TIM_OCMode = TIM_OCMode_PWM1;

#define OPTIC_DataWarm_Pin      GPIO_Pin_2  // OPTIC_Warm_Data IN
#define HAL_DataWarm_Pin        GPIO_Pin_3  // HAL_Warm_Data IN

#define OPTIC_DataCold_Pin      GPIO_Pin_4  // OPTIC_Cold_Data IN
#define HAL_DataCold_Pin        GPIO_Pin_5  // HAL_Cold_Data IN

////////////////////////////////////////////////////////////////////////////////
#define MENU_Pin                (!PIN_GET(BUTTON_Port, MENU_Button_Pin))
#define UP_Pin                  (!PIN_GET(BUTTON_Port, UP_Button_Pin))
#define DOWN_Pin                (!PIN_GET(BUTTON_Port, DOWN_Button_Pin))


////////////////////////////////////////////////////////////////////////////////

#define PORT_GET(port, pins)    ((port->ODR) & (pins))  // read port

#define PIN_SET(port, pins)     (port->ODR |= (pins))   // set pins on port
#define PIN_CLR(port, pins)     (port->ODR &=~(pins))   // clear pins on port
#define PIN_GET(port, pins)     ((port->IDR) & (pins))  // read pins on port

#define FPIN_SET(port, pins)    (port->BSRR = (pins))   // fast_set pins on port
#define FPIN_CLR(port, pins)    (port->BRR = (pins))    // fast_clear pins on port
#define FPIN_GET(port, pins)    (PIN_GET(port, pins))   // no fast get routines

////////////////////////////////////////////////////////////////////////////////

extern RTC_DateTimeTypeDef RTC_DateTime;
void MCU_Init(void);

////////////////////////////////////////////////////////////////////////////////

#endif // __MCU_CONFIG_H
