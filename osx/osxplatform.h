#ifndef OSXPLATFORM_H
#define OSXPLATFORM_H

#include "../platformdepend.h"
#include "notification.h"

class OsxPlatform : public IPlatform
{
public:
    OsxPlatform();

public:
    long showNotify(const QString& title, const QString& content, const QString &fellowIp) override;
    void hideAllNotify() override;

    void setBadgeNumber(int number) override;

    void setMainWnd(MainWindow *mainWnd) override;
private:
    Notification mNotify;
};

#endif // OSXPLATFORM_H
