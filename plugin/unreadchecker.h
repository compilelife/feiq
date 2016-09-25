#ifndef UNREADCHECKER_H
#define UNREADCHECKER_H

#include "iplugin.h"
#include <QObject>

class UnreadChecker : public QObject, public IPlugin
{
    Q_OBJECT

public:
    UnreadChecker();
    void init(FeiqWin* feiqWin) override;
    void unInit() override;

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    int mUnreadTimerInterval;
    int mUnreadTimerId=-1;
};

#endif // UNREADCHECKER_H
