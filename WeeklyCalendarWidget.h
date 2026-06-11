#pragma once

#include "LessonInfo.h"
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QMap>
#include <QPair>
#include <QList>

class LessonTile;

class WeeklyCalendarWidget : public QWidget {
    Q_OBJECT
public:
    explicit WeeklyCalendarWidget(QWidget *parent = nullptr);

    void setHourRange(int startHour, int endHour);
    void setLesson(int day, int hour, const LessonInfo &info);
    LessonInfo lesson(int day, int hour) const;
    void clearAll();
    void setDayNames(const QStringList &names);

signals:
    void lessonEdited(int day, int hour, const LessonInfo &info);

private:
    void buildGrid();
    void repositionTiles();

    QScrollArea *m_scroll  = nullptr;
    QWidget     *m_grid    = nullptr;
    QGridLayout *m_layout  = nullptr;

    int m_startHour = 7;
    int m_endHour   = 21;
    static constexpr int ROW_H    = 70;
    static constexpr int TIME_W   = 45;
    static constexpr int HEADER_H = 40;
    static constexpr int SPACING  = 1;

    QStringList m_dayNames = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

    QMap<QPair<int,int>, LessonTile*> m_tiles;
    QList<QLabel*> m_timeLabels;
    QList<QLabel*> m_dayLabels;
};