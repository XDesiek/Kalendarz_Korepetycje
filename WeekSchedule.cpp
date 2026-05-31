#include "WeekSchedule.h"

WeekSchedule::WeekSchedule(time_t week)
    : week(week)
{
    // tworzymy 7 dni zaczynając od poniedziałku tygodnia
    for (int i = 0; i < 7; ++i) {
        days.emplace_back(week + i * 86400);
    }
}

void WeekSchedule::fillSchedule()
{
    // wypełnianie danych — wywoływane przez CalendarEngine
}

DaySchedule& WeekSchedule::getDay(int dayIndex)
{
    return days[dayIndex];
}
