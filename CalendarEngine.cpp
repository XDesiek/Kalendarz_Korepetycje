#include "CalendarEngine.h"
#include "LessonStudent.h"
#include "LessonUSOS.h"
#include <QFile>
#include <QTextStream>

CalendarEngine::CalendarEngine()
    : callendar(0), currentWeek(0), view(nullptr)
{}

void CalendarEngine::loadWeek(time_t week)
{
    currentWeek = week;
    callendar   = WeekSchedule(week);
}

void CalendarEngine::loadFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        // parsowanie CSV — format do ustalenia przy implementacji
        // przykład: type,id,day,hour,duration,...
        Q_UNUSED(line);
    }

    file.close();
    updateWidget();
}

void CalendarEngine::updateWidget()
{
    if (view) {
        view->setSchedule(callendar);
        view->update();
    }
}
