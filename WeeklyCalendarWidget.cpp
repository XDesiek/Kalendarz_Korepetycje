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
#include <QScrollArea>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QFrame>
#include <LessonTile.h>


// ════════════════════════════════════════════════════════════════════════════
//  WeeklyCalendarWidget
// ════════════════════════════════════════════════════════════════════════════

WeeklyCalendarWidget::WeeklyCalendarWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setSpacing(2);
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

void WeeklyCalendarWidget::setLesson(int day, int hour, const LessonInfo &info)
{
    auto key = QPair<int,int>(day, hour);
    if (m_tiles.contains(key))
        m_tiles[key]->setLesson(info);
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
}

void WeeklyCalendarWidget::buildGrid()
{
    if (m_grid) {
        m_scroll->takeWidget();
        delete m_grid;
        m_tiles.clear();
    }

    m_grid = new QWidget;
    m_layout = new QGridLayout(m_grid);
    m_layout->setSpacing(1);
    m_layout->setContentsMargins(4, 4, 4, 4);

    const int days  = m_dayNames.size();
    const int hours = m_endHour - m_startHour;

    // Corner
    m_layout->addWidget(new QLabel, 0, 0);

    // Day headers
    for (int d = 0; d < days; ++d) {
        auto *hdr = new QLabel(m_dayNames[d]);
        hdr->setAlignment(Qt::AlignCenter);
        m_layout->addWidget(hdr, 0, d + 1);
        m_layout->setColumnStretch(d + 1, 1);
    }

    // Hour rows
    for (int h = 0; h < hours; ++h) {
        int absHour = m_startHour + h;

        auto *timeLbl = new QLabel(QString("%1:00").arg(absHour, 2, 10, QChar('0')));
        timeLbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        timeLbl->setFixedWidth(40);
        m_layout->addWidget(timeLbl, h + 1, 0);

        for (int d = 0; d < days; ++d) {
            auto *tile = new LessonTile(d, absHour, m_grid);
            auto  key  = QPair<int,int>(d, absHour);
            m_tiles[key] = tile;

            connect(tile, &LessonTile::lessonChanged,
                    this, &WeeklyCalendarWidget::lessonEdited);

            m_layout->addWidget(tile, h + 1, d + 1);
            m_layout->setRowMinimumHeight(h + 1, 56);
        }
    }

    m_scroll->setWidget(m_grid);
}
