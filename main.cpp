#include "mainwindow.h"
#include <QApplication>
#include "feiqwin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/default/res/icon.png"));

    MainWindow w;
    FeiqWin feiqWin;
    w.setFeiqWin(&feiqWin);

    w.show();
    return a.exec();
}
