#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // samo ustawianie glownych parametrow okna oraz wlaczanie calej aplikacji
    QApplication app(argc, argv);

    MainWindow win;
    win.setWindowTitle("System Zarządzania Korepetycjami");
    win.resize(1200, 800);
    win.show();

    return app.exec();
}
