#include "WeeklyCalendarWidget.h"

#include <QApplication>
#include <QDialog>
#include <QEnterEvent>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QFrame>
#include "LessonTile.h"

WeeklyCalendarWidget::WeeklyCalendarWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setSpacing(0);
    outerLayout->setContentsMargins(4, 4, 4, 4);

    m_scroll = new QScrollArea;
    m_scroll->setWidgetResizable(true);
    outerLayout->addWidget(m_scroll);

    buildGrid();
}

void WeeklyCalendarWidget::setHourRange(int startHour, int endHour)
{
    m_startHour = startHour;
    m_endHour   = endHour;
    buildGrid();
}

void WeeklyCalendarWidget::setDayNames(const QStringList &names)
{
    m_dayNames = names;
    buildGrid();
}

void WeeklyCalendarWidget::buildGrid()
{
    // usuń stary widget
    if (m_grid) {
        m_scroll->takeWidget();
        delete m_grid;
        m_tiles.clear();
        m_timeLabels.clear();
        m_dayLabels.clear();

    }

    const int days  = m_dayNames.size();
    const int hours = m_endHour - m_startHour;

    // m_grid to kontener — używamy go tylko żeby QScrollArea miał co pokazać
    m_grid = new QWidget;
    m_grid->setMinimumSize(
        TIME_W + days * (130 + SPACING),
        HEADER_H + hours * (ROW_H + SPACING)
        );

    // nagłówki dni
    for (int d = 0; d < days; ++d) {
        auto *lbl = new QLabel(m_dayNames[d], m_grid);
        lbl->setAlignment(Qt::AlignCenter);
        m_dayLabels.append(lbl);
    }

    // etykiety godzin + poziome linie
    for (int h = 0; h <= hours; ++h) {
        int absHour = m_startHour + h;
        auto *lbl = new QLabel(QString("%1:00").arg(absHour, 2, 10, QChar('0')), m_grid);
        lbl->setAlignment(Qt::AlignRight | Qt::AlignTop);
        lbl->setFixedWidth(TIME_W - 4);
        m_timeLabels.append(lbl);
    }

    // kafelki lekcji — pozycjonowane absolutnie
    for (int h = 0; h < hours; ++h) {
        int absHour = m_startHour + h;
        for (int d = 0; d < days; ++d) {
            auto *tile = new LessonTile(d, absHour, m_grid);
            auto key = QPair<int,int>(d, absHour);
            m_tiles[key] = tile;
            connect(tile, &LessonTile::lessonChanged,
                    this, &WeeklyCalendarWidget::lessonEdited);
        }
    }

    m_scroll->setWidget(m_grid);
    repositionTiles();
}

void WeeklyCalendarWidget::repositionTiles()
{
    if (!m_grid) return;

    const int days  = m_dayNames.size();
    const int hours = m_endHour - m_startHour;

    // szerokość kolumny dnia — równo dzielimy dostępną przestrzeń
    int totalW   = qMax(m_grid->width(), TIME_W + days * (130 + SPACING));
    int colW     = (totalW - TIME_W - SPACING) / days;

    // aktualizuj minimalny rozmiar gridu
    m_grid->setMinimumSize(
        TIME_W + days * (colW + SPACING),
        HEADER_H + hours * (ROW_H + SPACING)
        );

    // pozycje nagłówków dni
    for (int d = 0; d < days && d < m_dayLabels.size(); ++d) {
        int x = TIME_W + d * (colW + SPACING);
        m_dayLabels[d]->setGeometry(x, 0, colW, HEADER_H);
    }

    // pozycje etykiet godzin
    for (int h = 0; h <= hours && h < m_timeLabels.size(); ++h) {
        int y = HEADER_H + h * (ROW_H + SPACING);
        m_timeLabels[h]->setGeometry(0, y, TIME_W - 4, 16);
    }

    // pozycje kafelków — każdy dostaje swój absolutny (x, y, w, h)
    for (int h = 0; h < hours; ++h) {
        int absHour = m_startHour + h;
        int y = HEADER_H + h * (ROW_H + SPACING);
        for (int d = 0; d < days; ++d) {
            auto key = QPair<int,int>(d, absHour);
            if (!m_tiles.contains(key)) continue;
            LessonTile *tile = m_tiles[key];
            int x = TIME_W + d * (colW + SPACING);
            // wysokość zależy od durationHours zapisanego w LessonInfo
            int span = qMax(1, tile->lesson().durationHours);
            int h_px = span * (ROW_H + SPACING) - SPACING;
            tile->setGeometry(x, y, colW, h_px);
            tile->raise();
        }
    }
}

void WeeklyCalendarWidget::setLesson(int day, int hour, const LessonInfo &info)
{
    auto key = QPair<int,int>(day, hour);
    if (!m_tiles.contains(key)) return;
    m_tiles[key]->setLesson(info);
    repositionTiles();  // przerysuj po każdej zmianie
}

LessonInfo WeeklyCalendarWidget::lesson(int day, int hour) const
{
    auto key = QPair<int,int>(day, hour);
    return m_tiles.contains(key) ? m_tiles[key]->lesson() : LessonInfo{};
}

void WeeklyCalendarWidget::clearAll()
{
    for (auto *tile : m_tiles)
        tile->setLesson(LessonInfo{});
    repositionTiles();
}