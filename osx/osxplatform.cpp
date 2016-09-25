#include "osxplatform.h"
#include <QtMac>

OsxPlatform::OsxPlatform()
{

}

void OsxPlatform::showNotify(const QString &title, const QString &content)
{
    mNotify.show(title, content);
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
