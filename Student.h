#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student {
public:
    Student(int id, const QString &firstName, const QString &lastName,
            const QString &phone, const QString &communicator,
            const QString &notes);

    Student() : id(0) {}

    QString getFullName() const;

    int getId() const { return id; }
    QString getFirstName() const { return firstName; }
    QString getLastName() const { return lastName; }
    QString getPhone() const { return phone; }
    QString getCommunicator() const { return communicator; }
    QString getNotes() const { return notes; }

    void setId(int v) { id = v; }
    void setFirstName(const QString &v) { firstName = v; }
    void setLastName(const QString &v) { lastName = v; }
    void setPhone(const QString &v) { phone = v; }
    void setCommunicator(const QString &v) { communicator = v; }
    void setNotes(const QString &v) { notes = v; }

private:
    int id;
    QString firstName;
    QString lastName;
    QString phone;
    QString communicator;
    QString notes;
};

#endif
