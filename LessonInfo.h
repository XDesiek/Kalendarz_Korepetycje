#ifndef LESSONLNFO_H
#define LESSONLNFO_H

#include "qobject.h"
class LessonTile;

// ─── Data ────────────────────────────────────────────────────────────────────

// TU BEDZIEMY ZMIENIAC LIESSON INFO NA NASZE RZECZY

struct LessonInfo {
    QString subject;
    QString teacher;
    QString room;

    bool isEmpty() const { return subject.isEmpty(); }
};

#endif