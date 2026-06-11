#ifndef LESSONSTUDENT_H
#define LESSONSTUDENT_H

#include "ILesson.h"
// Reprezentuje lekcję korepetycji z prywatnym uczniem
class LessonStudent : public ILesson {
public:
    LessonStudent(int id, int studentID, int paymentID,
                  time_t timestamp, int duration);

    LessonStudent() {}// domyślny konstruktor potrzebny przy wczytywaniu z CSV

    QString getText() override;
    time_t getTime() override;
    bool checkIfPaid();

    // Gettery
    int getStudentId() const { return studentID; }
    int getPaymentId() const { return paymentID; }

    // Settery
    void setStudentId(int v) { studentID = v; }
    void setPaymentId(int v) { paymentID = v; }

private:
    int studentID = 0; // ID ucznia powiązanego z lekcją
    int paymentID = 0; // ID płatności (0 = nieopłacona)
};

#endif
