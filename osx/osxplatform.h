#ifndef OSXPLATFORM_H
#define OSXPLATFORM_H

#include "../platformdepend.h"
#include "notification.h"

class OsxPlatform : public IPlatform
{
public:
    OsxPlatform();

public:
    void showNotify(const QString& title, const QString& content) override;
    void hideAllNotify() override;

    void setBadgeNumber(int number) override;

private:
    Notification mNotify;
};

#endif // OSXPLATFORM_H
