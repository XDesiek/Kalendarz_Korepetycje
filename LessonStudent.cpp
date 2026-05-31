#include "LessonStudent.h"

LessonStudent::LessonStudent(int id, int studentID, int paymentID,
                             time_t timestamp, int duration)
{
    this->id        = id;
    this->studentID = studentID;
    this->paymentID = paymentID;
    this->timestamp = timestamp;
    this->duration  = duration;
}

QString LessonStudent::getText()
{
    return QString("Lesson [student %1]").arg(studentID);
}

time_t LessonStudent::getTime()
{
    return timestamp;
}

bool LessonStudent::checkIfPaid()
{
    // paymentID == 0 oznacza brak przypisanej płatności
    return paymentID != 0;
}
