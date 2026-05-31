#include "ILesson.h"

time_t ILesson::getEndTime()
{
    return timestamp + duration * 60;
}

bool ILesson::overlaps(ICalendarEntry &other)
{
    ILesson *otherLesson = dynamic_cast<ILesson*>(&other);
    if (!otherLesson) return false;

    time_t thisStart  = getTime();
    time_t thisEnd    = getEndTime();
    time_t otherStart = otherLesson->getTime();
    time_t otherEnd   = otherLesson->getEndTime();

    return thisStart < otherEnd && otherStart < thisEnd;
}
