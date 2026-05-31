#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student {
public:
    Student(int id, const QString &firstName, const QString &lastName,
            const QString &phone, const QString &communicator,
            const QString &notes);

    QString getFullName();

private:
    int     id;
    QString firstName;
    QString lastName;
    QString phone;
    QString communicator;
    QString notes;
};

#endif
