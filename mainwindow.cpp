#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LessonStudent.h"
#include "LessonUSOS.h"
#include "Student.h"
#include "Payment.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(ui->calendarContainer);
    layout->setContentsMargins(0, 0, 0, 0);

    m_calendar = new WeeklyCalendarWidget(this);
    layout->addWidget(m_calendar);

    ui->inputData->setDisplayFormat("dd.MM.yyyy");
    ui->inputData->setDate(QDate::currentDate());

    m_engine.setView(this);
    m_engine.init(".");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSchedule(WeekSchedule &schedule)
{
    time_t weekTime = schedule.getWeekStart();
    QDateTime dt;
    dt.setSecsSinceEpoch(weekTime);
    QDate date = dt.date();

    int dayOfWeek = date.dayOfWeek();
    QDate monday = date.addDays(-(dayOfWeek - 1));

    QStringList dayNames;
    QStringList dniTygodnia = {"Poniedziałek", "Wtorek", "Środa", "Czwartek", "Piątek", "Sobota", "Niedziela"};

    for (int i = 0; i < 7; ++i) {
        QDate current = monday.addDays(i);
        QString header = dniTygodnia[i] + "\n" + current.toString("dd.MM.yyyy");
        dayNames << header;
    }

    m_calendar->setDayNames(dayNames);

    m_calendar->clearAll();

    for (int day = 0; day < 7; ++day) {
        for (int hour = 7; hour < 21; ++hour) {
            ICalendarEntry* entry = schedule.getDay(day).getEntry(hour);
            if (entry) {
                LessonInfo info;

                if (auto ls = dynamic_cast<LessonStudent*>(entry)) {
                    info.subject = "Korepetycje";
                    info.teacher = "Uczeń ID: " + QString::number(ls->getStudentId());
                    info.room    = (ls->checkIfPaid() ? "Opłacone" : "Nieopłacone");
                } else if (auto lu = dynamic_cast<LessonUSOS*>(entry)) {
                    info.subject = lu->getSubject();
                    info.teacher = "Grupa: " + QString::number(lu->getGroupId());
                    info.room    = "Sala: " + lu->getRoomNumber();
                } else {
                    info.subject = entry->getText();
                }

                m_calendar->setLesson(day, hour, info);
            }
        }
    }
}

void MainWindow::update()
{
    QMainWindow::update();
}

ICalendarEntry* MainWindow::getSelectedEntry()
{
    return nullptr;
}

void MainWindow::on_btnWczytajUsos_clicked()
{
    m_engine.init(".");
    QMessageBox::information(this, "Sukces", "Wczytano i odświeżono pliki kalendarza.");
}

void MainWindow::on_btnZapisz_clicked()
{
    QMessageBox::information(this, "Zapisano", "Wszystkie dane są aktualne w plikach CSV.");
}

void MainWindow::on_btnDodajZajecia_clicked()
{
    QDate dataZajec = ui->inputData->date();
    QString godzinaStr = ui->inputGodzina->text().trimmed();
    QString idUczniaStr = ui->inputIdUcznia->text().trimmed();

    if (godzinaStr.isEmpty() || idUczniaStr.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Wszystkie pola w sekcji 'Dodaj Zajęcia' muszą być wypełnione!");
        return;
    }

    bool okGodzina, okId;
    int godzina  = godzinaStr.toInt(&okGodzina);
    int idUcznia = idUczniaStr.toInt(&okId);

    if (!okGodzina || godzina < 7 || godzina > 20) {
        QMessageBox::warning(this, "Błąd", "Godzina musi być poprawną liczbą z zakresu od 7 do 20!");
        return;
    }

    if (!okId || idUcznia <= 0) {
        QMessageBox::warning(this, "Błąd", "ID Ucznia musi być poprawną liczbą dodatnią!");
        return;
    }

    bool uczenIstnieje = false;
    for (const Student &s : m_engine.getStudents()) {
        if (s.getId() == idUcznia) {
            uczenIstnieje = true;
            break;
        }
    }

    if (!uczenIstnieje) {
        QMessageBox::warning(this, "Błąd", "Uczeń o podanym ID nie istnieje w bazie danych! Najpierw dodaj ucznia.");
        return;
    }

    QDateTime lessonTime(dataZajec, QTime(godzina, 0));
    time_t timestamp = lessonTime.toSecsSinceEpoch();

    int noweIdLekcji = 1;
    for (const auto& l : m_engine.getLessons()) {
        if (l->getId() >= noweIdLekcji) noweIdLekcji = l->getId() + 1;
    }

    auto noweKorepetycje = std::make_shared<LessonStudent>(
        noweIdLekcji, idUcznia, 0, timestamp, 60
        );

    m_engine.addLesson(noweKorepetycje);

    ui->inputGodzina->clear();
    ui->inputIdUcznia->clear();

    QMessageBox::information(this, "Sukces", "Dodano nowe zajęcia do kalendarza!");
}

void MainWindow::on_btnDodajUcznia_clicked()
{
    QString imie        = ui->inputImie->text().trimmed();
    QString nazwisko    = ui->inputNazwisko->text().trimmed();
    QString telefon     = ui->inputTelefon->text().trimmed();
    QString komunikator = ui->inputKomunikator->text().trimmed();

    if (imie.isEmpty() || nazwisko.isEmpty() || telefon.isEmpty() || komunikator.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Wszystkie pola w sekcji 'Dodaj Ucznia' muszą być wypełnione!");
        return;
    }

    bool isPhoneValid = true;
    if (telefon.length() != 9) {
        isPhoneValid = false;
    } else {
        for (QChar c : telefon) {
            if (!c.isDigit()) {
                isPhoneValid = false;
                break;
            }
        }
    }

    if (!isPhoneValid) {
        QMessageBox::warning(this, "Błąd", "Numer telefonu musi składać się z dokładnie 9 cyfr!");
        return;
    }

    int idUcznia = 1;
    for (const Student &s : m_engine.getStudents()) {
        if (s.getId() >= idUcznia) idUcznia = s.getId() + 1;
    }

    Student nowyUczen(idUcznia, imie, nazwisko, telefon, komunikator, "");
    m_engine.addStudent(nowyUczen);

    ui->inputImie->clear();
    ui->inputNazwisko->clear();
    ui->inputTelefon->clear();
    ui->inputKomunikator->clear();

    QMessageBox::information(this, "Sukces", "Uczeń został pomyślnie dodany!");
}

void MainWindow::on_btnDodajPlatnosc_clicked()
{
    QString kwotaStr    = ui->inputKwota->text().trimmed();
    QString metoda      = ui->inputMetoda->text().trimmed();
    QString idLekcjiStr = ui->inputIdLekcjiPlatnosc->text().trimmed();

    if (kwotaStr.isEmpty() || metoda.isEmpty() || idLekcjiStr.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Wszystkie pola w sekcji 'Dodaj Płatność' muszą być wypełnione!");
        return;
    }

    bool okKwota, okIdLekcji;
    double kwota = kwotaStr.toDouble(&okKwota);
    int idOpłacanejLekcji = idLekcjiStr.toInt(&okIdLekcji);

    if (!okKwota || kwota <= 0) {
        QMessageBox::warning(this, "Błąd", "Podaj poprawną kwotę większą od 0 (użyj kropki dla ułamków)!");
        return;
    }

    if (!okIdLekcji || idOpłacanejLekcji <= 0) {
        QMessageBox::warning(this, "Błąd", "ID opłacanej lekcji musi być poprawną liczbą dodatnią!");
        return;
    }

    int idPlatnosci = 1;
    for (const Payment &p : m_engine.getPayments()) {
        if (p.getId() >= idPlatnosci) idPlatnosci = p.getId() + 1;
    }

    Payment nowaPlatnosc(idPlatnosci, kwota, QDate::currentDate(), metoda);
    nowaPlatnosc.addLessonId(idOpłacanejLekcji);

    m_engine.addPayment(nowaPlatnosc);

    ui->inputKwota->clear();
    ui->inputMetoda->clear();
    ui->inputIdLekcjiPlatnosc->clear();

    QMessageBox::information(this, "Sukces", "Płatność została zaksięgowana!");
}

void MainWindow::on_btnListaUczniow_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Baza Uczniów");
    dialog.resize(700, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    const auto& uczniowie = m_engine.getStudents();

    QTableWidget *tabela = new QTableWidget(uczniowie.size(), 6, &dialog);
    tabela->setHorizontalHeaderLabels({"ID", "Imię", "Nazwisko", "Telefon", "Komunikator", "Notatki"});
    tabela->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabela->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int wiersz = 0;
    for (const Student &s : uczniowie) {
        tabela->setItem(wiersz, 0, new QTableWidgetItem(QString::number(s.getId())));
        tabela->setItem(wiersz, 1, new QTableWidgetItem(s.getFirstName()));
        tabela->setItem(wiersz, 2, new QTableWidgetItem(s.getLastName()));
        tabela->setItem(wiersz, 3, new QTableWidgetItem(s.getPhone()));
        tabela->setItem(wiersz, 4, new QTableWidgetItem(s.getCommunicator()));
        tabela->setItem(wiersz, 5, new QTableWidgetItem(s.getNotes()));
        wiersz++;
    }

    layout->addWidget(tabela);
    dialog.exec();
}

void MainWindow::on_btnListaPlatnosci_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Historia Płatności");
    dialog.resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    const auto& platnosci = m_engine.getPayments();

    QTableWidget *tabela = new QTableWidget(platnosci.size(), 5, &dialog);
    tabela->setHorizontalHeaderLabels({"ID Płatności", "Kwota", "Data", "Metoda", "ID Opłaconych Lekcji"});
    tabela->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabela->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int wiersz = 0;
    for (const Payment &p : platnosci) {
        tabela->setItem(wiersz, 0, new QTableWidgetItem(QString::number(p.getId())));
        tabela->setItem(wiersz, 1, new QTableWidgetItem(QString::number(p.getAmount(), 'f', 2) + " PLN"));
        tabela->setItem(wiersz, 2, new QTableWidgetItem(p.getDate().toString("dd.MM.yyyy")));
        tabela->setItem(wiersz, 3, new QTableWidgetItem(p.getMethod()));

        QStringList idLekcjiList;
        for (int id : p.getLessonIds()) {
            idLekcjiList << QString::number(id);
        }
        tabela->setItem(wiersz, 4, new QTableWidgetItem(idLekcjiList.join(", ")));

        wiersz++;
    }

    layout->addWidget(tabela);
    dialog.exec();
}

void MainWindow::on_btnPoprzedniTydzien_clicked() { m_engine.prevWeek(); }
void MainWindow::on_btnNastepnyTydzien_clicked()  { m_engine.nextWeek(); }
