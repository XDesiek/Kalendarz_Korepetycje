QT       += core widgets

TARGET   = WeeklyCalendar
TEMPLATE = app
CONFIG += c++17 console

HEADERS += \
    LessonInfo.h \
    LessonTile.h \
    WeeklyCalendarWidget.h \
    ICalendarEntry.h \
    ILesson.h \
    ICalendarView.h \
    LessonStudent.h \
    LessonUSOS.h \
    Student.h \
    Payment.h \
    DaySchedule.h \
    WeekSchedule.h \
    CalendarEngine.h

SOURCES += \
    main.cpp \
    LessonInfo.cpp \
    LessonTile.cpp \
    WeeklyCalendarWidget.cpp \
    ILesson.cpp \
    LessonStudent.cpp \
    LessonUSOS.cpp \
    Student.cpp \
    Payment.cpp \
    DaySchedule.cpp \
    WeekSchedule.cpp \
    CalendarEngine.cpp