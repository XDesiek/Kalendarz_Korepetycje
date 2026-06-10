#ifndef CSVSTORAGE_H
#define CSVSTORAGE_H

#include <QString>
#include <QStringList>
#include <vector>
#include <memory>

#include "Student.h"
#include "Payment.h"
#include "ILesson.h"

class CSVStorage {
public:
    explicit CSVStorage(const QString &dataDir = "."); //blokuje przypadkowa zmiane zwykłego tekstu na obiekt

    bool saveStudents(const std::vector<Student> &students);
    std::vector<Student> loadStudents();

    bool savePayments(const std::vector<Payment> &payments);
    std::vector<Payment> loadPayments();

    bool saveStudentLessons(const std::vector<std::shared_ptr<ILesson>> &lessons);
    std::vector<std::shared_ptr<ILesson>> loadStudentLessons();

    bool saveUsosLessons(const std::vector<std::shared_ptr<ILesson>> &lessons);
    std::vector<std::shared_ptr<ILesson>> loadUsosLessons();

    //funkcje zwracające ścieżki do naszych plików
    QString studentsPath() const {return m_studentsPath;}
    QString paymentsPath() const {return m_paymentsPath;}
    QString studentLessonsPath() const {return m_studentLessonsPath;}
    QString usosLessonsPath() const {return m_usosLessonsPath; }

private:
    QString m_dataDir;
    QString m_studentsPath;
    QString m_paymentsPath;
    QString m_studentLessonsPath;
    QString m_usosLessonsPath;

    //zabezpieczają pliki przed zepsuciem, np. jak użytkownik wpisze w notatkach przecinek
    static QString escapeField(const QString &field);
    static QString unescapeField(const QString &field);
    static QStringList parseCsvLine(const QString &line);
};

#endif // CSVSTORAGE_H
