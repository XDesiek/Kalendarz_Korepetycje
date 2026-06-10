#ifndef DAYSCHEDULE_H
#define DAYSCHEDULE_H

#include "ICalendarEntry.h"
#include <vector>
#include <ctime>

class DaySchedule {
public:
    DaySchedule(time_t date);

    void addEntry(ICalendarEntry *entry);
    ICalendarEntry* getEntry(int hour);

private:
    std::vector<ICalendarEntry*> dayEntries;
    time_t date;
};

#endif
