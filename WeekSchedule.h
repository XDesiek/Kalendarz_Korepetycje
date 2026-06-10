#ifndef WEEKSCHEDULE_H
#define WEEKSCHEDULE_H

#include "DaySchedule.h"
#include <vector>
#include <ctime>

class WeekSchedule {
public:
    WeekSchedule(time_t week = 0);

    void fillSchedule();
    DaySchedule& getDay(int dayIndex);

    time_t getWeekStart() const { return week; }

private:
    std::vector<DaySchedule> days;
    time_t week;
};

#endif
