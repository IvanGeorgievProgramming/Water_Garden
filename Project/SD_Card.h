#ifndef _SD_CARD_H
#define _SD_CARD_H

typedef struct Data_Water_Expense_s{
    uint32_t start_warm_expense_time;
    uint32_t end_warm_expense_time;
    uint32_t start_cold_expense_time;
    uint32_t end_cold_expense_time;
    float    current_warm_water;
    float    current_cold_water;
    bool     full_struct;
}Data_Water_Expense_t;


////////////////////////////////////////////////////////////////////////////////
extern char WriteOut[5];
extern float current_warm_water, current_cold_water;
extern struct RTC_DateTime;

////////////////////////////////////////////////////////////////////////////////
void Save_Data();

////////////////////////////////////////////////////////////////////////////////
#endif  // _SD_CARD