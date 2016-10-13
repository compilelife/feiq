#ifndef OSXNOTIFICATION_H
#define OSXNOTIFICATION_H

#include <QString>
#include <QObject>

class MainWindow;
class Notification
{
public:
    explicit Notification();
    int show(const QString& title, const QString& content, const QString &fellowIp);
    void hideAll();

public:
    void setMainWnd(MainWindow* mainWnd);
};

#endif // OSXNOTIFICATION_H
