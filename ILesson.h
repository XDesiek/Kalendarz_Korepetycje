#ifndef ILESSON_H
#define ILESSON_H

#include "ICalendarEntry.h"

class ILesson : public ICalendarEntry {
public:
    virtual ~ILesson() = default;

    bool overlaps(ICalendarEntry &other) override;
    time_t getEndTime();

protected:
    int    id;
    time_t timestamp;
    int    duration;
};

#endif
