#ifndef ICALENDARVIEW_H
#define ICALENDARVIEW_H

#include "ICalendarEntry.h"
#include "WeekSchedule.h"

class ICalendarView {
public:
    virtual ~ICalendarView() = default;

    virtual void            setSchedule(WeekSchedule &schedule) = 0;
    virtual void            update()                            = 0;
    virtual ICalendarEntry* getSelectedEntry()                  = 0;
};

#endif
