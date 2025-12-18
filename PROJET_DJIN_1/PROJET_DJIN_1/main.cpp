#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <QtDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(1480,760);
    w.setWindowTitle("ASQII");
    w.setWindowIcon(QIcon(":/images/logo_asqii.png"));
    w.show();




    return a.exec();
}
