#ifndef OSXNOTIFICATION_H
#define OSXNOTIFICATION_H

#include <QString>

class Notification
{
public:
    explicit Notification();
    void show(const QString& title, const QString& content);
    void hideAll();
};

#endif // OSXNOTIFICATION_H
