#include <QApplication>
#include <QMainWindow>
#include "WeeklyCalendarWidget.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow win;
    win.setWindowTitle("Weekly Timetable");
    win.resize(1100, 700);

    auto *cal = new WeeklyCalendarWidget;
    win.setCentralWidget(cal);

    // Demo lessons
    cal->setLesson(0, 8,  {"Mathematics",        "Dr. Smith",   "Room 101"});
    cal->setLesson(0, 9,  {"Mathematics",        "Dr. Smith",   "Room 101"});
    cal->setLesson(0, 11, {"Physics",            "Prof. Jones", "Lab 3"});
    cal->setLesson(0, 13, {"English Literature", "Ms. Carter",  "Room 215"});

    cal->setLesson(1, 8,  {"History",            "Mr. Brown",   "Room 108"});
    cal->setLesson(1, 10, {"Chemistry",          "Dr. Lee",     "Lab 1"});
    cal->setLesson(1, 12, {"Art",                "Ms. Novak",   "Studio"});

    cal->setLesson(2, 9,  {"Biology",            "Dr. White",   "Lab 2"});
    cal->setLesson(2, 11, {"Mathematics",        "Dr. Smith",   "Room 101"});
    cal->setLesson(2, 13, {"Geography",          "Mr. Green",   "Room 212"});

    cal->setLesson(3, 8,  {"Physics",            "Prof. Jones", "Lab 3"});
    cal->setLesson(3, 10, {"English Literature", "Ms. Carter",  "Room 215"});
    cal->setLesson(3, 12, {"Computer Science",   "Mr. Turing",  "PC Lab"});

    cal->setLesson(4, 9,  {"Chemistry",          "Dr. Lee",     "Lab 1"});
    cal->setLesson(4, 11, {"History",            "Mr. Brown",   "Room 108"});
    cal->setLesson(4, 13, {"Computer Science",   "Mr. Turing",  "PC Lab"});

    win.show();
    return app.exec();
}
