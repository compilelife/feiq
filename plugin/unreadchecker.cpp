#include "unreadchecker.h"
#include <QTimerEvent>
#include "platformdepend.h"

#define PLUGIN_NAME "unread_checker"
REGISTER_PLUGIN(PLUGIN_NAME, UnreadChecker)

UnreadChecker::UnreadChecker()
{
}

void UnreadChecker::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mUnreadTimerId)
    {
        auto count = mFeiq->getUnreadCount();
        if (count > 0)
            PlatformDepend::instance().showNotify("未读提醒", QString("还有%1条未读消息").arg(count));
    }
}

void UnreadChecker::init(FeiqWin *feiqWin)
{
    IPlugin::init(feiqWin);

    auto settings = mFeiq->settings();
    mUnreadTimerInterval = settings->value(PLUGIN_NAME"/timer", "0").toInt();
    if (mUnreadTimerInterval > 0)
        mUnreadTimerId = startTimer(mUnreadTimerInterval*1000, Qt::VeryCoarseTimer);
}

void UnreadChecker::unInit()
{
    if (mUnreadTimerId > 0)
        killTimer(mUnreadTimerId);
}
