#ifndef ICALENDARENTRY_H
#define ICALENDARENTRY_H

#include <QString>
#include <ctime>

class ICalendarEntry {
public:
    virtual ~ICalendarEntry() = default;

    virtual QString getText() = 0;
    virtual time_t  getTime() = 0;
    virtual bool overlaps(ICalendarEntry &other) = 0;
};

#endif
