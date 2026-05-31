#ifndef PAYMENT_H
#define PAYMENT_H

#include <QString>
#include <QDate>
#include <vector>

class Payment {
public:
    Payment(int id, double amount, const QDate &date, const QString &method);

    bool    verify();
    QString getStatus();

    void addLessonId(int lessonId);

private:
    int              id;
    std::vector<int> lessonIds;
    double           amount;
    QDate            date;
    QString          method;
};

#endif
