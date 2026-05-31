#include "LessonTile.h"
#include "qevent.h"
#include "qpainter.h"

// ════════════════════════════════════════════════════════════════════════════
//  LessonTile
// ════════════════════════════════════════════════════════════════════════════

LessonTile::LessonTile(int day, int hour, QWidget *parent)
    : QWidget(parent), m_day(day), m_hour(hour)
{
    setMinimumSize(130, 56);
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

// void LessonTile::mouseDoubleClickEvent(QMouseEvent *event)
// {
//     if (event->button() == Qt::LeftButton)
//         openEditor();
//     QWidget::mouseDoubleClickEvent(event);
// }

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

    if (m_lesson.isEmpty()) {
        // Empty: default window background
        p.drawRect(r.adjusted(0, 0, -1, -1));
    } else {

        // TWORZENIE TEKSTU KALENDARZA-------------------------------------------------------------------------------------------------------------------------

        p.drawRect(r.adjusted(0, 0, -1, -1));

        // Text — default font, default text color
        QRect textR = r.adjusted(4, 4, -4, -4);

        QFont boldFont = p.font();
        boldFont.setBold(true);
        p.setFont(boldFont);
        p.drawText(textR, Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,
                   m_lesson.subject);


        QString details;
        if (!m_lesson.teacher.isEmpty()) details += m_lesson.teacher;
        if (!m_lesson.room.isEmpty())
            details += (details.isEmpty() ? "" : "  |  ") + m_lesson.room;
        if (!details.isEmpty()) {
            QRect detR = textR.adjusted(0, 18, 0, 0);
            p.drawText(detR, Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap, details);
        }
    }
}

// void LessonTile::openEditor()
// {
//     QDialog dlg(this);
//     dlg.setWindowTitle(m_lesson.isEmpty() ? "Add Lesson" : "Edit Lesson");
//     dlg.setMinimumWidth(280);

//     auto *form = new QFormLayout;
//     form->setContentsMargins(10, 10, 10, 10);

//     auto *subjectEdit = new QLineEdit(m_lesson.subject);
//     subjectEdit->setPlaceholderText("e.g. Mathematics");
//     auto *teacherEdit = new QLineEdit(m_lesson.teacher);
//     teacherEdit->setPlaceholderText("e.g. Dr. Smith");
//     auto *roomEdit    = new QLineEdit(m_lesson.room);
//     roomEdit->setPlaceholderText("e.g. Room 204");

//     form->addRow("Subject:", subjectEdit);
//     form->addRow("Teacher:", teacherEdit);
//     form->addRow("Room:",    roomEdit);

//     auto *btnOk     = new QPushButton("OK");
//     auto *btnCancel = new QPushButton("Cancel");
//     auto *btnClear  = new QPushButton("Remove Lesson");
//     auto *btnRow    = new QHBoxLayout;
//     btnRow->addWidget(btnClear);
//     btnRow->addStretch();
//     btnRow->addWidget(btnCancel);
//     btnRow->addWidget(btnOk);

//     btnOk->setDefault(true);

//     QObject::connect(btnOk,     &QPushButton::clicked, &dlg, &QDialog::accept);
//     QObject::connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
//     QObject::connect(btnClear,  &QPushButton::clicked, [&](){
//         subjectEdit->clear();
//         teacherEdit->clear();
//         roomEdit->clear();
//         dlg.accept();
//     });

//     auto *root = new QVBoxLayout(&dlg);
//     root->addLayout(form);
//     root->addLayout(btnRow);

//     subjectEdit->setFocus();

//     if (dlg.exec() == QDialog::Accepted) {
//         LessonInfo info;
//         info.subject = subjectEdit->text().trimmed();
//         info.teacher = teacherEdit->text().trimmed();
//         info.room    = roomEdit->text().trimmed();
//         setLesson(info);
//         emit lessonChanged(m_day, m_hour, info);
//     }
// }