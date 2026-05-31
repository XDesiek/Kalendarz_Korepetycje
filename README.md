# Weekly Calendar Qt Widget

A polished, interactive weekly timetable widget for Qt 6 (also compatible with Qt 5 with minor adjustments).

## Features

- **Hourly grid** — Mon–Fri columns, configurable time range (default 07:00–21:00)
- **Click to edit** — double-click any cell to open an add/edit dialog
- **Lesson data** — subject, teacher, room, and a custom accent color per lesson
- **Color picker** — each lesson gets its own accent color via `QColorDialog`
- **Remove lesson** — "Remove" button in the editor clears the cell
- **Hover effects** — subtle highlight + "+" indicator on empty cells
- **Scrollable** — long time ranges scroll within the widget
- **Programmatic API** — set and query lessons from code

## Project Structure

```
WeeklyCalendar/
├── CMakeLists.txt          # CMake build (Qt 6)
├── WeeklyCalendar.pro      # qmake build (Qt 5 / 6)
├── main.cpp                # Demo application with sample data
├── WeeklyCalendarWidget.h  # Public API
└── WeeklyCalendarWidget.cpp
```

## Build (CMake / Qt 6)

```bash
cd WeeklyCalendar
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build build
./build/WeeklyCalendar
```

## Build (qmake / Qt 5 or 6)

```bash
cd WeeklyCalendar
qmake WeeklyCalendar.pro
make          # or nmake / jom on Windows
./WeeklyCalendar
```

## API Quick Reference

```cpp
// Create
auto *cal = new WeeklyCalendarWidget(parent);

// Set time range (optional, default 7–21)
cal->setHourRange(8, 18);

// Use custom day names (optional)
cal->setDayNames({"Mon","Tue","Wed","Thu","Fri","Sat"});

// Add a lesson programmatically
cal->setLesson(0 /*day*/, 10 /*hour*/, {
    "Mathematics",
    "Dr. Smith",
    "Room 101",
    QColor("#6366f1")
});

// Read back
LessonInfo info = cal->lesson(0, 10);

// React to user edits
connect(cal, &WeeklyCalendarWidget::lessonEdited,
        [](int day, int hour, const LessonInfo &info) {
            qDebug() << day << hour << info.subject;
        });

// Clear all
cal->clearAll();
```

## Qt 5 Compatibility Note

Replace `QEnterEvent` with `QEvent` in `enterEvent` signature and adjust
`enterEvent(QEvent*)` accordingly — Qt 5 uses `QEvent` for that override.
"# Kalendarz_Korepetycje" 
"# Kalendarz_Korepetycje" 
