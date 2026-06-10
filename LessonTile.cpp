#include "LessonTile.h"
#include <QMouseEvent>
#include <QPainter>
#include <QEnterEvent>


LessonTile::LessonTile(int day, int hour, QWidget *parent)
    : QWidget(parent), m_day(day), m_hour(hour)
{
    setMinimumSize(130, 70);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void LessonTile::setLesson(const LessonInfo &info)
{
    m_lesson = info;
    update();
}

void LessonTile::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked(m_day, m_hour);
    QWidget::mousePressEvent(event);
}

void LessonTile::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    update();
    QWidget::enterEvent(event);
}

void LessonTile::leaveEvent(QEvent *event)
{
    m_hovered = false;
    update();
    QWidget::leaveEvent(event);
}

void LessonTile::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QRect r = rect();

    // wypełnienie tłem
    if (!m_lesson.isEmpty()) {
        QColor bg = m_hovered
                        ? m_lesson.color.lighter(115)  // rozjaśnij przy hover
                        : m_lesson.color;
        p.fillRect(r, bg);
    } else {
        p.fillRect(r, Qt::transparent);
    }

    p.drawRect(r.adjusted(0, 0, -1, -1));

    if (!m_lesson.isEmpty()) {
        QRect textR = r.adjusted(4, 4, -4, -4);

        QFont boldFont = p.font();
        boldFont.setBold(true);
        p.setFont(boldFont);

        QRect subjectRect;
        p.drawText(textR, Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,
                   m_lesson.subject, &subjectRect);

        QString details;
        if (!m_lesson.teacher.isEmpty()) details += m_lesson.teacher;
        if (!m_lesson.room.isEmpty())
            details += (details.isEmpty() ? "" : "\n") + m_lesson.room;

        if (!details.isEmpty()) {
            p.setFont(this->font());

            QRect detR = textR.adjusted(0, subjectRect.height() + 4, 0, 0);
            p.drawText(detR, Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap, details);
        }
    }
}
