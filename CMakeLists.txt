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
    WeeklyCalendarWidget.cpp
    WeeklyCalendarWidget.h
)

target_link_libraries(WeeklyCalendar PRIVATE Qt6::Core Qt6::Widgets)
