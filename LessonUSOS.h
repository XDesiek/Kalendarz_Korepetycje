#ifndef LESSONUSOS_H
#define LESSONUSOS_H

#include "ILesson.h"
// Reprezentuje zajęcia zaimportowane z planu USOS
class LessonUSOS : public ILesson {
public:
    LessonUSOS(int id, time_t timestamp, int duration,
               const QString &subject, const QString &roomNumber, int groupID);

    // Konstruktor domyślny potrzebny przez CSVStorage (load)
    LessonUSOS() {}

    QString getText() override;
    time_t  getTime() override;

    // Gettery
    QString getRoomNumber() const { return roomNumber; }
    int     getGroupId()    const { return groupID; }
    QString getSubject()    const { return subject; }

    // Settery
    void setRoomNumber(const QString &v) { roomNumber = v; }
    void setGroupId(int v)               { groupID = v; }
    void setSubject(const QString &v)    { subject = v; }

private:
    QString roomNumber;
    int     groupID = 0;
    QString subject;
};

#endif
