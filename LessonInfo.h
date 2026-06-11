#ifndef LESSONLNFO_H
#define LESSONLNFO_H

#include <QString>
#include <QColor>

class LessonTile;

struct LessonInfo
{
    QString subject;
    QString teacher;
    QString room;
    int     durationHours = 1;
    QColor  color = QColor(240, 240, 240);

    bool isEmpty() const
    {
        return subject.isEmpty();
    }
};

#endif