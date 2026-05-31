#include "Student.h"

Student::Student(int id, const QString &firstName, const QString &lastName,
                 const QString &phone, const QString &communicator,
                 const QString &notes)
    : id(id), firstName(firstName), lastName(lastName),
      phone(phone), communicator(communicator), notes(notes)
{}

QString Student::getFullName()
{
    return firstName + " " + lastName;
}
