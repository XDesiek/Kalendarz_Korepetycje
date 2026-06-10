cmake_minimum_required(VERSION 3.16)
project(WeeklyCalendar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(WeeklyCalendar
    main.cpp
        CalendarEngine.cpp
        DaySchedule.cpp
        ILesson.cpp
        LessonInfo.cpp
        LessonStudent.cpp
        LessonTile.cpp
        LessonUSOS.cpp
        Payment.cpp
        Student.cpp
        WeeklyCalendarWidget.cpp
        WeekSchedule.cpp
        CSVStorage.cpp
        lessonmerger.cpp

        CSVStorage.cpp
        # Pliki nagłówkowe (.h)
        CalendarEngine.h
        DaySchedule.h
        ICalendarEntry.h
        ICalendarView.h
        ILesson.h
        LessonInfo.h
        LessonStudent.h
        LessonTile.h
        LessonUSOS.h
        Payment.h
        Student.h
        WeeklyCalendarWidget.h
        CSVStorage.h
        WeekSchedule.h
        lessonmerger.h

)

target_link_libraries(WeeklyCalendar PRIVATE Qt6::Core Qt6::Widgets)
