#ifndef WEEKSCHEDULE_H
#define WEEKSCHEDULE_H

#include "DaySchedule.h"
#include <vector>
#include <ctime>

class WeekSchedule {
public:
    WeekSchedule(time_t week);

    void        fillSchedule();
    DaySchedule& getDay(int dayIndex);

private:
    std::vector<DaySchedule> days;
    time_t                   week;
};

#endif
