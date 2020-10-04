//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include <types.h>

#ifndef _DELAY_H
#define _DELAY_H


////////////////////////////////////////////////////////////////////////////////

typedef enum waitmsChannel_e{
    channelBtnMatrix = 0,
    channelCurrentTime = 1,
    channelMenuTime = 2,
    channelSleepTime = 3,
    channelTestLed = 4
} waitmsChannel_t;

////////////////////////////////////////////////////////////////////////////////

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

void clear_wait_ms();
uint8_t wait_ms(uint32_t time);

void clear_wait_ms_ch(uint8_t channel);
void reset_wait_ms_ch(uint8_t channel);
uint8_t wait_ms_ch(uint8_t channel, uint32_t time);

////////////////////////////////////////////////////////////////////////////////

#endif