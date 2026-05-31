#ifndef LESSONSTUDENT_H
#define LESSONSTUDENT_H

#include "ILesson.h"

class LessonStudent : public ILesson {
public:
    LessonStudent(int id, int studentID, int paymentID,
                  time_t timestamp, int duration);

    QString getText() override;
    time_t  getTime() override;

    bool checkIfPaid();

private:
    int studentID;
    int paymentID;
};

#endif
