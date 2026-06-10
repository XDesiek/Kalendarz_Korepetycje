#include "WeekSchedule.h"

WeekSchedule::WeekSchedule(time_t week)
    : week(week)
{
    for (int i = 0; i < 7; ++i) {
        days.emplace_back(week + i * 86400);
    }
}

DaySchedule& WeekSchedule::getDay(int dayIndex)
{
    return days[dayIndex];
}
