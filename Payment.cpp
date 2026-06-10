#include "Payment.h"

Payment::Payment(int id, double amount, const QDate &date, const QString &method)
    : id(id), amount(amount), date(date), method(method) //tworzenie listy inicjalizacyjnej
{}

void Payment::addLessonId(int lessonId)
{
    lessonIds.push_back(lessonId);
}

bool Payment::verify()
{
    return amount > 0 && !lessonIds.empty();
}

QString Payment::getStatus()
{
    if (amount <= 0) return "unpaid";
    if (!verify()) return "partial";
    return "paid";
}
