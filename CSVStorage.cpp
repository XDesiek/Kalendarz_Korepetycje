#include "CSVStorage.h"
#include "LessonStudent.h"
#include "LessonUSOS.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

CSVStorage::CSVStorage(const QString &dataDir)
    : m_dataDir(dataDir) //stworzenie listy inicjalizacyjnej
{
    QDir().mkpath(m_dataDir);
    m_studentsPath = m_dataDir + "/students.csv";
    m_paymentsPath = m_dataDir + "/payments.csv";
    m_studentLessonsPath = m_dataDir + "/student_lessons.csv";
    m_usosLessonsPath = m_dataDir + "/usos_lessons.csv";
}

bool CSVStorage::saveStudents(const std::vector<Student> &students) {
    QFile file(m_studentsPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8); //sprawia, że polskie znaki będą zapisane poprawnie
    out << "id,firstName,lastName,phone,communicator,notes\n";
    for (const Student &s : students) {
        out << s.getId() << "," << escapeField(s.getFirstName()) << "," << escapeField(s.getLastName()) << ","
            << escapeField(s.getPhone()) << "," << escapeField(s.getCommunicator()) << "," << escapeField(s.getNotes()) << "\n";
    }
    return true;
}

std::vector<Student> CSVStorage::loadStudents() {
    std::vector<Student> result;
    QFile file(m_studentsPath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) return result;
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8); //sprawia, że polskie znaki będą wyświetlane poprawnie
    bool header = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed(); //ucina spacje z końców linijek
        if (line.isEmpty()) continue;
        if (header)
        {
            header = false;
            continue;
        }
        QStringList f = parseCsvLine(line);
        if (f.size() < 6) continue;
        Student s;
        s.setId(f[0].toInt()); s.setFirstName(unescapeField(f[1])); s.setLastName(unescapeField(f[2]));
        s.setPhone(unescapeField(f[3])); s.setCommunicator(unescapeField(f[4])); s.setNotes(unescapeField(f[5]));
        result.push_back(s);
    }
    return result;
}

bool CSVStorage::savePayments(const std::vector<Payment> &payments) {
    QFile file(m_paymentsPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8); //sprawia, że polskie znaki będą zapisane poprawnie
    out << "id,lessonIds,amount,date,method\n";
    for (const Payment &p : payments) {
        QStringList ids;
        for (int lid : p.getLessonIds()) ids << QString::number(lid);
        out << p.getId() << "," << escapeField(ids.join(";")) << "," << p.getAmount() << ","
            << p.getDate().toString(Qt::ISODate) << "," << escapeField(p.getMethod()) << "\n";
    }
    return true;
}

std::vector<Payment> CSVStorage::loadPayments() {
    std::vector<Payment> result;
    QFile file(m_paymentsPath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) return result;
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8); //sprawia, że polskie znaki będą wyświetlane poprawnie
    bool header = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed(); //ucina spacje z końców linijek
        if (line.isEmpty()) continue;
        if (header) { header = false; continue; }
        QStringList f = parseCsvLine(line);
        if (f.size() < 5) continue;
        Payment p; p.setId(f[0].toInt());
        std::vector<int> ids;
        QString idsStr = unescapeField(f[1]);
        if (!idsStr.isEmpty())
        {
            for (const QString &s : idsStr.split(";")) if (!s.isEmpty()) ids.push_back(s.toInt());
        }
        p.setLessonIds(ids); p.setAmount(f[2].toDouble()); p.setDate(QDate::fromString(f[3], Qt::ISODate)); p.setMethod(unescapeField(f[4]));
        result.push_back(p);
    }
    return result;
}

bool CSVStorage::saveStudentLessons(const std::vector<std::shared_ptr<ILesson>> &lessons) {
    QFile file(m_studentLessonsPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8); //sprawia, że polskie znaki będą zapisane poprawnie
    out << "id,timestamp,duration,studentId,paymentId\n";

    for (const auto &lesson : lessons) {
        if (auto ls = std::dynamic_pointer_cast<LessonStudent>(lesson)) //upewnia się do którego pliku zapisać dane
        {
            out << ls->getId() << "," << ls->getTimestamp() << "," << ls->getDuration() << ","
                << ls->getStudentId() << "," << ls->getPaymentId() << "\n";
        }
    }
    return true;
}

std::vector<std::shared_ptr<ILesson>> CSVStorage::loadStudentLessons() {
    std::vector<std::shared_ptr<ILesson>> result;
    QFile file(m_studentLessonsPath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) return result;
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8); //sprawia, że polskie znaki będą wyświetlane poprawnie
    bool header = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed(); //ucina spacje z końców linijek
        if (line.isEmpty() || header) { header = false; continue; }
        QStringList f = parseCsvLine(line);
        if (f.size() < 5) continue;

        auto ls = std::make_shared<LessonStudent>();
        ls->setId(f[0].toInt());
        ls->setTimestamp(static_cast<time_t>(f[1].toLongLong()));
        ls->setDuration(f[2].toInt());
        ls->setStudentId(f[3].toInt());
        ls->setPaymentId(f[4].toInt());
        result.push_back(ls);
    }
    return result;
}

bool CSVStorage::saveUsosLessons(const std::vector<std::shared_ptr<ILesson>> &lessons) {
    QFile file(m_usosLessonsPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8); //sprawia, że polskie znaki będą zapisane poprawnie
    out << "id,timestamp,duration,subject,roomNumber,groupId\n";

    for (const auto &lesson : lessons) {
        if (auto lu = std::dynamic_pointer_cast<LessonUSOS>(lesson)) {
            out << lu->getId() << "," << lu->getTimestamp() << "," << lu->getDuration() << ","
                << escapeField(lu->getSubject()) << "," << escapeField(lu->getRoomNumber()) << "," << lu->getGroupId() << "\n";
        }
    }
    return true;
}

std::vector<std::shared_ptr<ILesson>> CSVStorage::loadUsosLessons() {
    std::vector<std::shared_ptr<ILesson>> result;
    QFile file(m_usosLessonsPath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) return result;
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8); //sprawia, że polskie znaki będą wyświetlane poprawnie
    bool header = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed(); //ucina spacje z końców linijek
        if (line.isEmpty() || header) { header = false; continue; }
        QStringList f = parseCsvLine(line);
        if (f.size() < 6) continue;

        auto lu = std::make_shared<LessonUSOS>();
        lu->setId(f[0].toInt());
        lu->setTimestamp(static_cast<time_t>(f[1].toLongLong()));
        lu->setDuration(f[2].toInt());
        lu->setSubject(unescapeField(f[3]));
        lu->setRoomNumber(unescapeField(f[4]));
        lu->setGroupId(f[5].toInt());
        result.push_back(lu);
    }
    return result;
}

QString CSVStorage::escapeField(const QString &field)
{
    if (field.contains(',') || field.contains('"') || field.contains('\n'))
        return "\"" + QString(field).replace("\"", "\"\"") + "\"";
    return field;
}
QString CSVStorage::unescapeField(const QString &field)
{
    if (field.startsWith('"') && field.endsWith('"'))
        return field.mid(1, field.size() - 2).replace("\"\"", "\"");
    return field;
}
QStringList CSVStorage::parseCsvLine(const QString &line) {
    QStringList fields; QString current; bool inQuotes = false;
    for (int i = 0; i < line.size(); ++i)
    {
        QChar c = line[i];
        if (inQuotes)
        {
            if (c == '"')
            {
                if (i + 1 < line.size() && line[i+1] == '"') { current += '"'; ++i; }
                else { inQuotes = false; }
            }
            else { current += c; }
        }
        else
        {
            if (c == '"') inQuotes = true;
            else if (c == ',')
            {
                fields.append(current); current.clear();
            }
            else current += c;
        }
    }
    fields.append(current);
    return fields;
}

QString CSVStorage::parseICSField(const QString &line, const QString &key) {
    if (line.startsWith(key))
        return line.mid(key.length()).trimmed();
    return QString();
}

std::vector<std::shared_ptr<LessonUSOS>> CSVStorage::loadFromICS(const QString &filePath) {
    std::vector<std::shared_ptr<LessonUSOS>> result;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return result;
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // sklejamy linie kontynuowane (ICS łamie długie linie przez spację na początku)
    QStringList lines;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty() && (line[0] == ' ' || line[0] == '\t')) {
            // linia kontynuowana — doklejamy do poprzedniej
            if (!lines.isEmpty())
                lines.last() += line.trimmed();
        } else {
            lines.append(line.trimmed());
        }
    }

    QString summary, dtstart, dtend, description;
    bool inEvent = false;
    int nextId = 1;

    for (const QString &line : lines) {
        if (line == "BEGIN:VEVENT") {
            inEvent = true;
            summary = dtstart = dtend = description = "";
            continue;
        }
        if (line == "END:VEVENT") {
            inEvent = false;

            if (dtstart.isEmpty() || summary.isEmpty()) continue;

            // parsuj datę i godzinę z formatu 20260610T101500
            QString dt = dtstart;
            if (dt.contains(":")) dt = dt.section(":", 1); // usuń "VALUE=DATE-TIME:"
            // format: YYYYMMDDThhmmss
            int year  = dt.mid(0, 4).toInt();
            int month = dt.mid(4, 2).toInt();
            int day   = dt.mid(6, 2).toInt();
            int hour  = dt.mid(9, 2).toInt();
            int min   = dt.mid(11, 2).toInt();
            // zaokrąglenie do pełnej godziny
            if (min >= 30) hour++;

            // czas trwania
            int durationMin = 60; // domyślnie
            if (!dtend.isEmpty()) {
                QString de = dtend;
                if (de.contains(":")) de = de.section(":", 1);
                int ehour = de.mid(9, 2).toInt();
                int emin  = de.mid(11, 2).toInt();
                int startTotalMin = hour * 60 + min;
                int endTotalMin   = ehour * 60 + emin;
                durationMin = endTotalMin - startTotalMin;
                if (durationMin <= 0) durationMin = 60;
            }

            // wyciągnij salę z DESCRIPTION
            QString room;
            int groupId = 0;

            for (const QString &descLine : description.split("\\n")) {
                QString dl = descLine.trimmed();
                if (dl.startsWith("Sala:")) {
                    room = dl.mid(5).trimmed();
                    // NIE rób break — kontynuuj, żeby dojść do URL-a
                }
                // wyciągnij gr_nr z URL-a w DESCRIPTION
                if (dl.contains("gr_nr=")) {
                    int idx = dl.indexOf("gr_nr=");
                    QString after = dl.mid(idx + 6); // pomiń "gr_nr="
                    // gr_nr kończy się na "&" albo na końcu stringa
                    int end = after.indexOf('&');
                    QString grNrStr = (end != -1) ? after.left(end) : after;
                    groupId = grNrStr.toInt();
                }
            }

            // wyczyść SUMMARY — usuń prefix "WYK - ", "LAB - ", "PRO - " itp.
            QString subjectName = summary;
            // usuń prefix słownikowy (3 wielkie litery + " - ")
            if (subjectName.length() > 5 &&
                subjectName[3] == ' ' && subjectName[4] == '-') {
                subjectName = subjectName.mid(6).trimmed();
            }
            // obsłuż format {'pl': '...', 'en': '...'}
            if (subjectName.startsWith("{'pl':")) {
                int start = subjectName.indexOf("'", 7) + 1;
                int end   = subjectName.indexOf("'", start);
                if (start > 0 && end > start)
                    subjectName = subjectName.mid(start, end - start);
            }

            QDateTime lessonDT(QDate(year, month, day), QTime(hour, 0));
            time_t ts = lessonDT.toSecsSinceEpoch();

            auto lesson = std::make_shared<LessonUSOS>();
            lesson->setId(nextId++);
            lesson->setTimestamp(ts);
            lesson->setDuration(durationMin);
            lesson->setSubject(subjectName);
            lesson->setRoomNumber(room);
            lesson->setGroupId(groupId);
            result.push_back(lesson);
            continue;
        }

        if (!inEvent) continue;

        if (line.startsWith("SUMMARY:"))
            summary = line.mid(8);
        else if (line.startsWith("DTSTART"))
            dtstart = line.mid(line.indexOf(':') + 1);
        else if (line.startsWith("DTEND"))
            dtend = line.mid(line.indexOf(':') + 1);
        else if (line.startsWith("DESCRIPTION:"))
            description = line.mid(12);
    }

    return result;
}