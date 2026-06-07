#include "LessonUSOS.h"

LessonUSOS::LessonUSOS(int id, time_t timestamp, int duration,
                       const QString &subject, const QString &roomNumber, int groupID)
{
    this->id = id;
    this->timestamp = timestamp;
    this->duration = duration;
    this->subject = subject;
    this->roomNumber = roomNumber;
    this->groupID = groupID;
}

QString LessonUSOS::getText()
{
    return subject;
}

time_t LessonUSOS::getTime()
{
    return timestamp;
}
