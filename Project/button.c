//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include <gpio.h>
#include <delay.h>
#include "MCU_config.h"
#include <button.h>

////////////////////////////////////////////////////////////////////////////////

uint8_t readBtnSetup()
{
    static uint8_t _btn;
    
    _btn = btnCode_noBtnPress;
    if (MENU_Pin)  _btn |= (uint8_t)MENU;
    if (UP_Pin)    _btn |= (uint8_t)UP;
    if (DOWN_Pin)  _btn |= (uint8_t)DOWN;
    
    return(_btn);
}

////////////////////////////////////////////////////////////////////////////////
uint8_t readBtnSetupMatrix(){
  
    static uint8_t newKey, oldKey, currentKey;

    newKey = readBtnSetup();
    if(newKey != oldKey){
      oldKey = newKey;
      clear_wait_ms();
    }else if(  wait_ms_ch(channelBtnMatrix, 10)  )  currentKey = newKey;
    //} else if (wait_ms(10)) currentKey = newKey;

    return(currentKey);
}
////////////////////////////////////////////////////////////////////////////////
//==============================================================================