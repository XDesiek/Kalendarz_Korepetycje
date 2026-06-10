#include "CalendarEngine.h"
#include "LessonStudent.h"
#include "LessonUSOS.h"
#include <QDebug>
#include <algorithm>

//konstruktor ustalający wartości początkowe obiektów
CalendarEngine::CalendarEngine()
    : callendar(0), currentWeek(0), view(nullptr), m_storage(".")
{}

void CalendarEngine::init(const QString &dataDir)
{
    m_storage = CSVStorage(dataDir);

    m_students = m_storage.loadStudents();
    m_payments = m_storage.loadPayments();

    // Odczyt z dwóch osobnych plików
    m_studentLessons = m_storage.loadStudentLessons();
    m_usosLessons = m_storage.loadUsosLessons();

    // Łączymy w jedną, chronologiczną listę
    m_lessons = m_merger.mergeAndSort(m_studentLessons, m_usosLessons);

    qDebug() << "CalendarEngine::init — wczytano:"
             << m_students.size() << "uczniów,"
             << m_studentLessons.size() << "lekcji korepetycji,"
             << m_usosLessons.size()  << "zajęć USOS";

    currentWeek = time(nullptr); //pobiera aktualny czas z zegara komputera
    callendar = WeekSchedule(currentWeek);
    rebuildSchedule();

    updateWidget();
}

//zapisuje do wskaźnika adres okna głównego i każe my sie zaktualizować
void CalendarEngine::setView(ICalendarView *v) {
    view = v;
    updateWidget();
}

void CalendarEngine::loadWeek(time_t week) {
    currentWeek = week;
    callendar = WeekSchedule(week);
    rebuildSchedule();
    updateWidget();
}
void CalendarEngine::nextWeek() { loadWeek(currentWeek + 7 * 24 * 3600); }
void CalendarEngine::prevWeek() { loadWeek(currentWeek - 7 * 24 * 3600); }

void CalendarEngine::addStudent(const Student &student) {
    m_students.push_back(student);
    m_storage.saveStudents(m_students);
}

bool CalendarEngine::addPayment(const Payment &payment) {

    // walidacja — czy lekcja istnieje i czy nie jest już opłacona
    for (int lessonId : payment.getLessonIds()) {
        bool found = false;
        for (auto &lesson : m_studentLessons) {
            auto ls = std::dynamic_pointer_cast<LessonStudent>(lesson);
            if (ls && ls->getId() == lessonId) {
                found = true;
                if (ls->checkIfPaid()) return false; // już opłacona
                break;
            }
        }
        if (!found) return false; // lekcja o tym ID nie istnieje
    }

    // wszystko OK — zapisz płatność
    m_payments.push_back(payment);
    m_storage.savePayments(m_payments);

    // powiąż lekcje z tą płatnością
    for (int lessonId : payment.getLessonIds()) {
        for (auto &lesson : m_studentLessons) {
            auto ls = std::dynamic_pointer_cast<LessonStudent>(lesson);
            if (ls && ls->getId() == lessonId) {
                ls->setPaymentId(payment.getId());
            }
        }
    }

    m_storage.saveStudentLessons(m_studentLessons);
    rebuildSchedule();
    updateWidget();
    return true;
}

bool CalendarEngine::addLesson(std::shared_ptr<ILesson> lesson) {

    // sprawdź konflikt ze wszystkimi istniejącymi lekcjami
    for (const auto &existing : m_lessons) {
        if (lesson->overlaps(*existing)) {
            return false;
        }
    }

    if (std::dynamic_pointer_cast<LessonStudent>(lesson)) {
        m_studentLessons.push_back(lesson);
        m_storage.saveStudentLessons(m_studentLessons);
    }
    else if (std::dynamic_pointer_cast<LessonUSOS>(lesson)) {
        m_usosLessons.push_back(lesson);
        m_storage.saveUsosLessons(m_usosLessons);
    }

    m_lessons = m_merger.mergeAndSort(m_studentLessons, m_usosLessons);
    rebuildSchedule();
    updateWidget();
    return true;
}

//ogolnie to ta funkcje nizej i reszte tych zakomentowanych mozna wywalic bo sa useless i nie uzywamy ich nigdzie ======================

// void CalendarEngine::removeLesson(int lessonId)
// {
//     auto isMatch = [lessonId](const std::shared_ptr<ILesson>& l) { return l->getId() == lessonId; };

//     auto itStudent = std::remove_if(m_studentLessons.begin(), m_studentLessons.end(), isMatch);
//     if (itStudent != m_studentLessons.end())
//     {
//         m_studentLessons.erase(itStudent, m_studentLessons.end());
//         m_storage.saveStudentLessons(m_studentLessons);
//     }
//     else
//     {
//         auto itUsos = std::remove_if(m_usosLessons.begin(), m_usosLessons.end(), isMatch);
//         if (itUsos != m_usosLessons.end()){
//             m_usosLessons.erase(itUsos, m_usosLessons.end());
//             m_storage.saveUsosLessons(m_usosLessons);
//         }
//     }
//     m_lessons = m_merger.mergeAndSort(m_studentLessons, m_usosLessons);
//     rebuildSchedule();
//     updateWidget();
// }

// void CalendarEngine::updateLesson(std::shared_ptr<ILesson> lesson) {
//     if (std::dynamic_pointer_cast<LessonStudent>(lesson)) {
//         for (auto &l : m_studentLessons) {
//             if (l->getId() == lesson->getId()) {
//                 l = lesson;
//                 break;
//             }
//         }
//         m_storage.saveStudentLessons(m_studentLessons);
//     } else if (std::dynamic_pointer_cast<LessonUSOS>(lesson)) {
//         for (auto &l : m_usosLessons) {
//             if (l->getId() == lesson->getId()) {
//                 l = lesson; break;
//             }
//         }
//         m_storage.saveUsosLessons(m_usosLessons);
//     }

//     m_lessons = m_merger.mergeAndSort(m_studentLessons, m_usosLessons);
//     rebuildSchedule();
//     updateWidget();
// }

// void CalendarEngine::removeStudent(int studentId)
// {
//     m_students.erase(std::remove_if(m_students.begin(), m_students.end(),
//                                     [studentId](const Student &s){ return s.getId() == studentId; }), m_students.end());
//     m_storage.saveStudents(m_students);
// }

// void CalendarEngine::removePayment(int paymentId)
// {
//     m_payments.erase(std::remove_if(m_payments.begin(), m_payments.end(),
//                                     [paymentId](const Payment &p){ return p.getId() == paymentId; }), m_payments.end());
//     m_storage.savePayments(m_payments);
// }

// void CalendarEngine::updateStudent(const Student &student)
// {
//     for (Student &s : m_students) if (s.getId() == student.getId())
//         {
//             s = student;
//             break;
//         }
//     m_storage.saveStudents(m_students);
// }

// void CalendarEngine::updatePayment(const Payment &payment)
// {
//     for (Payment &p : m_payments) if (p.getId() == payment.getId())
//         {
//             p = payment;
//             break;
//         }
//     m_storage.savePayments(m_payments);
// }

void CalendarEngine::loadFromFile(const QString &path) {
    Q_UNUSED(path); // mówi programowi aby nie dawał ostrzeżenia przy buildowaniu o tym, że nie używamy zmiennej
    updateWidget();
}

void CalendarEngine::updateWidget() {
    if (view)
    {
        view->setSchedule(callendar);
        view->update();
    }
}

void CalendarEngine::rebuildSchedule() {
    callendar = WeekSchedule(currentWeek);

    struct tm *tmPtr = localtime(&currentWeek); //zmiana formatu unixowego (sekundy od 1970 roku) na normalny format (dni, miesiące, godziny i minuty)
    //zerujemy wszystkie wartosci do 0 aby nie ignorowało nam porannych lekcji
    tmPtr->tm_hour = 0;
    tmPtr->tm_min = 0;
    tmPtr->tm_sec = 0;
    time_t currentMidnight = mktime(tmPtr); //zamiana naszej zmodyfikowanej struktury z wyzerowanymi godzinami z powrotem na sekundy

    int dow = tmPtr->tm_wday;
    int daysToMon = (dow == 0) ? -6 : (1 - dow);

    time_t monday = currentMidnight + daysToMon * 24 * 3600;
    time_t sunday = monday + 7 * 24 * 3600;

    for (auto &lesson : m_lessons)
    {
        time_t lt = lesson->getTimestamp();
        if (lt >= monday && lt < sunday)
        {
            struct tm *lTm = localtime(&lt);
            int lDow = lTm->tm_wday;
            int dayIdx = (lDow == 0) ? 6 : (lDow - 1);
            callendar.getDay(dayIdx).addEntry(lesson.get());
        }
    }
}
