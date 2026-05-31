#ifndef LESSONUSOS_H
#define LESSONUSOS_H

#include "ILesson.h"

class LessonUSOS : public ILesson {
public:
    LessonUSOS(int id, time_t timestamp, int duration,
               const QString &subject, const QString &roomNumber, int groupID);

    QString getText() override;
    time_t  getTime() override;

private:
    QString roomNumber;
    int     groupID;
    QString subject;
};

#endif
