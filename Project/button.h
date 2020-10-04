
//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include <types.h>

#ifndef BUTTON
#define BUTTON

////////////////////////////////////////////////////////////////////////////////

typedef struct btnData_s {
    uint8_t    newCode;
    uint8_t    oldCode;
    uint8_t    currentCode;
    struct {
        uint8_t    b_btnIsUp           : 1;
        uint8_t    b_btnIsDown         : 1;
        uint8_t    b_btnIsPressed      : 1;
        uint8_t    b_btnIsClick        : 1;
        uint8_t    b_btnEdgeDetected   : 1;
        uint8_t    b_btnEdgeUp2Down    : 1;
        uint8_t    b_btnEdgeDown2Up    : 1;
        uint8_t    b_one_time          : 1;
    } flags;
} btnData_t;

typedef enum buttonSpecialCode_e {
    btnCode_noBtnPress_internal = 0,
    //btnCode_noBtnPress = 0xFF
    btnCode_noBtnPress = 0x00  // !! new value
} buttonSpecialCode_t;

typedef enum buttonSetupCode_e {
    MENU = 0x01, //    btnCodeSET1 = 0b00000001
    UP   = 0x02, //    btnCodeSET2 = 0b00000010
    DOWN = 0x04  //    btnCodeSET3 = 0b00000100
} buttonSetupCode_t;

////////////////////////////////////////////////////////////////////////////////

uint8_t readBtnSetupMatrix();

////////////////////////////////////////////////////////////////////////////////


#endif