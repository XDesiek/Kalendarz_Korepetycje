#ifndef LESSONTILE_H
#define LESSONTILE_H
#include "LessonInfo.h"
#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QMap>
#include <QPair>
#include <QWidget>
#include <QMouseEvent>

// ─── Lesson Tile ─────────────────────────────────────────────────────────────

class LessonTile : public QWidget {
    Q_OBJECT
public:
    explicit LessonTile(int day, int hour, QWidget *parent = nullptr);

    void setLesson(const LessonInfo &info);
    LessonInfo lesson() const { return m_lesson; }

    int day()  const { return m_day; }
    int hour() const { return m_hour; }

signals:
    void clicked(int day, int hour);
    void lessonChanged(int day, int hour, const LessonInfo &info);


private:


    void mousePressEvent(QMouseEvent *event) override;
    // void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void setFrameStyle();
    void openEditor();

    int        m_day;
    int        m_hour;
    LessonInfo m_lesson;
    bool       m_hovered = false;
};

#endif
