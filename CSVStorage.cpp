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
