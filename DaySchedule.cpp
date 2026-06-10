#include "DaySchedule.h"

DaySchedule::DaySchedule(time_t date)
    : date(date) //stworzenie listy inicjalizacyjnej
{}

void DaySchedule::addEntry(ICalendarEntry *entry)
{
    dayEntries.push_back(entry);
}

ICalendarEntry* DaySchedule::getEntry(int hour)
{
    for (ICalendarEntry *entry : dayEntries) {
        time_t entryTime = entry->getTime();
        tm t = *localtime(&entryTime);
        if (t.tm_hour == hour)
            return entry;
    }
    return nullptr;
}
