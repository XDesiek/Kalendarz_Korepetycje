#ifndef LESSONSTUDENT_H
#define LESSONSTUDENT_H

#include "ILesson.h"

class LessonStudent : public ILesson {
public:
    LessonStudent(int id, int studentID, int paymentID,
                  time_t timestamp, int duration);

    LessonStudent() {}

    QString getText() override;
    time_t getTime() override;
    bool checkIfPaid();

    //Funkcje służące odczytu potrzebnych danych
    int getStudentId() const { return studentID; }
    int getPaymentId() const { return paymentID; }

    //Funkcje służące wpisywania nowych danych do lekcji
    void setStudentId(int v) { studentID = v; }
    void setPaymentId(int v) { paymentID = v; }

private:
    int studentID = 0;
    int paymentID = 0;
};

#endif
