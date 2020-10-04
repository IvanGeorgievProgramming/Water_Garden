#ifndef _RTC_H_
#define _RTC_H_

#include "GlobalDefs.h" // change this include to your 'BYTE', 'WORD' and other general prototypes if needed

#ifdef _TBLOADER_
    #undef  _BYTE_ALIGN
    #define _BYTE_ALIGN
#endif

typedef struct
{
    BYTE Hour;
    BYTE Minute;
    BYTE Second;

    BYTE Date;
    BYTE Month;
    BYTE Year;

    BYTE WeekDay;
    bool PM;
}_RTC _BYTE_ALIGN;

void RTC_Init( void );
void GetDateTime( _RTC* Dest );
void SetDateTime( _RTC* Src );
BYTE CheckValidDateTime( _RTC* Src );
void SetDefaultDateTime( void );
bool CheckIfCalendarIsInitilized( void );

#endif // _RTC_H_