#ifndef CALENDARENGINE_H
#define CALENDARENGINE_H

#include "WeekSchedule.h"
#include "ICalendarView.h"
#include "CSVStorage.h"
#include "Student.h"
#include "Payment.h"
#include "ILesson.h"
#include "LessonMerger.h"

#include <QString>
#include <vector>
#include <memory>
#include <ctime>

class CalendarEngine {
public:
    CalendarEngine();

    void init(const QString &dataDir = ".");
    void setView(ICalendarView *v);

    void loadWeek(time_t week);
    void nextWeek();
    void prevWeek();

    //Funkcje zwracające listy bez ich kopiowania
    const std::vector<Student>& getStudents() const { return m_students; }
    const std::vector<Payment>& getPayments() const { return m_payments; }
    const std::vector<std::shared_ptr<ILesson>>& getLessons() const { return m_lessons;  }

    void addStudent(const Student &student);
    bool addPayment(const Payment &payment);
    void addLesson(std::shared_ptr<ILesson> lesson);

    void removeStudent(int studentId);
    void removePayment(int paymentId);
    void removeLesson(int lessonId);

    void updateStudent(const Student &student);
    void updatePayment(const Payment &payment);
    void updateLesson(std::shared_ptr<ILesson> lesson);

    void updateWidget();
    void loadFromFile(const QString &path);

private:
    //Listy przechowujące pobrane informacje z plików
    std::vector<Student> m_students;
    std::vector<Payment> m_payments;

    std::vector<std::shared_ptr<ILesson>> m_studentLessons;
    std::vector<std::shared_ptr<ILesson>> m_usosLessons;
    std::vector<std::shared_ptr<ILesson>> m_lessons;

    WeekSchedule callendar; // Struktura przechowująca gotowy układ zajęć dla jednego, wybranego tygodnia
    time_t currentWeek; // Znacznik czasu reprezentujący wyświetlany tydzień w aplikacji
    ICalendarView *view; // Wskaźnik na interfejs graficzny, dzięki któremu silnik wie, co ma odświeżyć

    CSVStorage m_storage; // Obiekt odpowiedzialny za fizyczny zapis i odczyt plików CSV z dysku
    LessonMerger m_merger; //Obiekt sortujący i łączący plan z usosa i z korepetycjami

    void rebuildSchedule();
};

#endif // CALENDARENGINE_H
