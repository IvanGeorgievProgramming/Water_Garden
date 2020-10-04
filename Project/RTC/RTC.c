//#include "Common.h"

#include "stm32f10x.h"
//#include "delay_func.h"

////////////////////////////////////////////////////////////////////////////////

#include "RTC.h"

////////////////////////////////////////////////////////////////////////////////

#define PAUSE_VAL 1

const BYTE MonthLen[ 12 ] =
{
	31, 28, 31, 30,
	31, 30, 31, 31,
	30, 31, 30, 31
};

//---------------------------------------------------------------------------

static bool Configured( void )
{
	if( !( RCC->BDCR & BIT( 8 ) ) )					// if no LSE selected
		return false;
		
	if( !( RCC->BDCR & RCC_BDCR_LSEON ) )			// if no LSE enabled
		return false;

	if( !( RCC->BDCR & RCC_BDCR_LSERDY ) )
		return false;

	if( !( RCC->BDCR & RCC_BDCR_RTCEN ) )			// if RTC not enabled
		return false;

	return true;
}

//---------------------------------------------------------------------------

void RTC_Init( void )
{
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;	// Enable PWR 

	if( Configured() )
		return;

	PWR->CR   |= PWR_CR_DBP;				// Access to RTC and backup SRAM enable
		
	RCC->BDCR |= RCC_BDCR_BDRST;			// Reset selection
	RCC->BDCR &= ~RCC_BDCR_BDRST;

    PWR->CR |= PWR_CR_DBP;                  // Disable the write protection for RTC registers
	  
    RTC->CRL |= RTC_CRL_CNF;                // Enter Configuration mode

	delay_ms( PAUSE_VAL );

	// RTC clock source selection
	RCC->BDCR |= BIT( 8 );					// Set LSE ( Low Speed External )
		
	RCC->BDCR |= RCC_BDCR_LSEON;			// Enable LSE
	RCC->BDCR |= RCC_BDCR_RTCEN;			// Enable RTC if not
    
    while( !( RCC->BDCR & RCC_BDCR_LSERDY ) ); // Wait for LSE clock ready
    // TODO: add timer for wait LSE clock ready (and send message to PC or LED flash)

    RTC->CRL &= ~RTC_CRL_CNF;                // Exit Configuration mode

	PWR->CR &= ~PWR_CR_DBP;                  // Lock RTC and backup SRAM access
}

////////////////////////////////////////////////////////////////////////////////

bool CheckIfCalendarIsInitilized( void )
{
    if( !( RTC->CRL & RTC_CRL_RTOFF ) )
        return( 0 );    // Calendar has not been initialized
    return( 1 );        // Calendar has been initialized
}

////////////////////////////////////////////////////////////////////////////////

static BYTE GetMonthLen( BYTE Year, BYTE Month )
{
	BYTE AddValue = 0;
	
	if( ( Month == 2 ) && ( ( Year % 4 ) == 0 ) ) // if month is february and year is leap
		AddValue = 1;

	if( ( Month < 1 ) || ( Month > 12 ) )
		return 0;

	return MonthLen[ Month ] + AddValue;
}

//---------------------------------------------------------------------------

static DWORD CalcTimeValue( _RTC* Src )
{
	DWORD Tens, Units, Value = 0;

    // Set Hour
	Tens  = Src->Hour / 10;
	Units = Src->Hour - ( Tens * 10 );
	Value |= ( Tens << 20 ) | ( Units << 16 );

    // Set Minute
	Tens  = Src->Minute / 10;
	Units = Src->Minute - ( Tens * 10 );
	Value |= ( Tens << 12 ) | ( Units <<  8 );

    // Set Seconds
	Tens  = Src->Second / 10;
	Units = Src->Second - ( Tens * 10 );
	Value |= ( Tens << 4 ) | Units;


	if( Src->PM )
		Value |= BIT( 22 );

	return Value;
}

//---------------------------------------------------------------------------

static DWORD CalcDateValue( _RTC* Src )
{
	DWORD Tens, Units, Value = 0;

	// Set Year
	Tens  = Src->Year / 10;
	Units = Src->Year - ( Tens * 10 );
	Value |= ( Tens << 20 ) | ( Units << 16 );

	// Set Month
	Tens  = Src->Month / 10;
	Units = Src->Month - ( Tens * 10 );

	if( Tens )
		Value |= BIT( 12 );

	Value |= Units << 8;

	// Set Date
	Tens  = Src->Date / 10;
	Units = Src->Date - ( Tens * 10 );
	Value |= ( Tens << 4 ) | Units;

	// Set Day of Week
	Units = Src->WeekDay;
	Value |= ( Units & 0x07 ) << 13;

	return Value;
}

//---------------------------------------------------------------------------

void GetDateTime( _RTC* Dest )
{
	BYTE Tens, Units;

	if( NULL == Dest )
		return;

//	if( RTC->TR & BIT( 22 ) )
//		Dest->PM = true;
//	else
//		Dest->PM = false;

	// Get Hour
	Tens  = ( RTC->TR >> 20 ) & 0x03;
	Units = ( RTC->TR >> 16 ) & 0x0f;
	Dest->Hour = ( Tens * 10 ) + Units;

	// Get Minutes
	Tens  = ( RTC->TR >> 12 ) & 0x07;
	Units = ( RTC->TR >>  8 ) & 0x0f;
	Dest->Minute = ( Tens * 10 ) + Units;

	// Get Seconds
	Tens  = ( RTC->TR >>  4 ) & 0x07;
	Units = ( RTC->TR >>  0 ) & 0x0f;
	Dest->Second = ( Tens * 10 ) + Units;

	// Get Year
	Tens  = ( RTC->DR >> 20 ) & 0x0f;
	Units = ( RTC->DR >> 16 ) & 0x0f; 
	Dest->Year = ( Tens * 10 ) + Units;

	// Get Week Day ( 0 - forbidden 1-7 Monday - Sunday ) 
	Units = ( RTC->DR >> 13 ) & 0x07; 
	Dest->WeekDay = Units;

	// Get Month
	Tens  = ( RTC->DR >> 12 ) & 0x01;
	Units = ( RTC->DR >>  8 ) & 0x0f; 
	Dest->Month = ( Tens * 10 ) + Units;

	// Get Date
	Tens  = ( RTC->DR >>  4 ) & 0x03;
	Units = ( RTC->DR >>  0 ) & 0x0f; 
	Dest->Date = ( Tens * 10 ) + Units;
}

//---------------------------------------------------------------------------

void SetDateTime( _RTC* Src )
{
	PWR->CR |= PWR_CR_DBP;					// Access to RTC and backup SRAM enable

	RTC->WPR = 0xCA;                        // Disable the write protection for RTC registers
	RTC->WPR = 0x53;
	  
	RTC->ISR |= RTC_ISR_INIT;				// Enter Initialization mode
    while( !( RTC->ISR & RTC_ISR_INITF ) ); // Wait for - Calendar registers update is allowed
    // TODO: add timer for wait LSE clock ready (and send message to PC or LED flash)

	//PauseMs( PAUSE_VAL );

    RTC->TR = CalcTimeValue( Src );
	RTC->DR = CalcDateValue( Src );

	RTC->ISR &= ~RTC_ISR_INIT;				// Exit Initialization mode
    while( ( RTC->ISR & RTC_ISR_INITF ) );  // Wait for - Calendar registers update is not allowed
    // TODO: add timer for wait LSE clock ready (and send message to PC or LED flash)

	RTC->WPR = 0xff;						// Lock RTC and backup SRAM access
}

//---------------------------------------------------------------------------

BYTE CheckValidDateTime( _RTC* Src )
{
	if( ( Src->Year < 17 ) || ( Src->Year > 99 ) )
	{
		return 1;
	}

	if( ( Src->Month < 1 ) || ( Src->Month > 12 ) )
	{
		return 2;
	}

	if( ( Src->Date < 1 ) || ( Src->Date > GetMonthLen( Src->Year, Src->Month ) ) )
	{
		return 3;
	}

	if( ( Src->WeekDay < 1 ) || ( Src->WeekDay > 7 ) )
	{
		return 4;
	}

	if( ( Src->Hour   > 23 ) ||
		( Src->Minute > 59 ) ||
		( Src->Second > 59 ) )
	{
		return 5;
	}

    return 0;
}

//---------------------------------------------------------------------------

void SetDefaultDateTime( void )
{
    _RTC Rtc;

    Rtc.Year = 17;
    Rtc.Month = 1;
    Rtc.Date = 1;
    Rtc.WeekDay = 1;
    Rtc.Hour = 0;
    Rtc.Minute = 0;
    Rtc.Second = 0;
    SetDateTime( &Rtc );
}