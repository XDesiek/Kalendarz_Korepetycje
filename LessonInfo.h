#ifndef LESSONLNFO_H
#define LESSONLNFO_H

#include "qobject.h"
class LessonTile;

struct LessonInfo
{
    QString subject;
    QString teacher;
    QString room;

    bool isEmpty() const
    {
        return subject.isEmpty();
    }
};

#endif
