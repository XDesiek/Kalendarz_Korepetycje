#ifndef PAYMENT_H
#define PAYMENT_H

#include <QString>
#include <QDate>
#include <vector>

class Payment {
public:
    Payment(int id, double amount, const QDate &date, const QString &method);
    Payment() : id(0), amount(0.0) {}

    bool verify();
    QString getStatus();
    void addLessonId(int lessonId);

    int getId() const { return id; }
    double getAmount() const { return amount; }
    QDate getDate() const { return date; }
    QString getMethod() const { return method; }
    const std::vector<int>& getLessonIds()const { return lessonIds; }

    void setId(int v) { id = v; }
    void setAmount(double v) { amount = v; }
    void setDate(const QDate &v) { date = v; }
    void setMethod(const QString &v) { method = v; }
    void setLessonIds(std::vector<int> v) { lessonIds = std::move(v); }

private:
    int id;
    std::vector<int> lessonIds;
    double amount;
    QDate date;
    QString method;
};

#endif
