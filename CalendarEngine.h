#ifndef CALENDARENGINE_H
#define CALENDARENGINE_H

#include "WeekSchedule.h"
#include "ICalendarView.h"
#include <QString>
#include <ctime>

class CalendarEngine {
public:
    CalendarEngine();

    void updateWidget();
    void loadFromFile(const QString &path);
    void loadWeek(time_t week);

private:
    WeekSchedule    callendar;
    time_t          currentWeek;
    ICalendarView  *view;
};

#endif
