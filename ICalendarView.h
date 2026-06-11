#ifndef ICALENDARVIEW_H
#define ICALENDARVIEW_H

#include "ICalendarEntry.h"
#include "WeekSchedule.h"

// Interfejs widoku kalendarza — CalendarEngine komunikuje się z GUI przez ten interfejs
class ICalendarView {
public:
    virtual ~ICalendarView() = default;

    virtual void setSchedule(WeekSchedule &schedule) = 0;
    // przekazuje dane tygodnia do widoku
    virtual void update() = 0;
    // odświeża widok
    virtual ICalendarEntry* getSelectedEntry() = 0;
    // odświeża widok
};

#endif
