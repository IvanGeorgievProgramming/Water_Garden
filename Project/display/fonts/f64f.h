//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _F64F_H
#define _F64F_H

#include <types.h>


// Шрифт пропорциональный, высота 64
#define f64_FLOAT_HEIGHT        64

// Кол-во символов в таблице шрифта
#define f64f_NOFCHARS           27


// Функция возвращает указатель на подтаблицу символа Char
uint8_t *f64f_GetCharTable(uint8_t Char);

#endif 
