#include "osxplatform.h"
#include <QtMac>

OsxPlatform::OsxPlatform()
{

}

long OsxPlatform::showNotify(const QString &title, const QString &content, const QString & fellowIp)
{
    return mNotify.show(title, content, fellowIp);
}

void OsxPlatform::hideAllNotify()
{
    mNotify.hideAll();
}

void OsxPlatform::setBadgeNumber(int number)
{
    if (number == 0)
        QtMac::setBadgeLabelText("");
    else
        QtMac::setBadgeLabelText(QString::number(number));
}

void OsxPlatform::setMainWnd(MainWindow *mainWnd)
{
    IPlatform::setMainWnd(mainWnd);
    mNotify.setMainWnd(mainWnd);
}
