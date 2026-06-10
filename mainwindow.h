#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ICalendarView.h"
#include "CalendarEngine.h"
#include "WeeklyCalendarWidget.h"
#include "WeekSchedule.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public ICalendarView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void odswiezListeUczniow();
    void setSchedule(WeekSchedule &schedule) override;
    void update() override;
    ICalendarEntry* getSelectedEntry() override;

private slots:
    void on_btnWczytajUsos_clicked();
    void on_btnZapisz_clicked();
    void on_btnDodajZajecia_clicked();
    void on_btnDodajUcznia_clicked();
    void on_btnDodajPlatnosc_clicked();
    void on_btnListaUczniow_clicked();
    void on_btnListaPlatnosci_clicked();
    void on_btnPoprzedniTydzien_clicked();
    void on_btnNastepnyTydzien_clicked();

private:
    Ui::MainWindow *ui;
    WeeklyCalendarWidget *m_calendar;
    CalendarEngine m_engine;
};

#endif // MAINWINDOW_H
