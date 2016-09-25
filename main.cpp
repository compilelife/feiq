#include "mainwindow.h"
#include <QApplication>
#include <QWindow>

//待测:
//*无当前交谈用户，且有未读消息时，未读消息能否正确置顶？

//应用图标
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/default/res/icon.png"));
    MainWindow w;
    w.show();
    return a.exec();
}

//表情->html
//查通讯录功能
//查ip占用功能
//美化用户列表
