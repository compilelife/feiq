#include "notification.h"
#include "notificationimpl.h"
#include <QDebug>
#include "../mainwindow.h"

static NotificationImpl* impl = [[NotificationImpl alloc] init];

static void clickCallback(void* arg, NSString* fellowIp)
{
    MainWindow* mainWnd = (MainWindow*)arg;
    const char* ip = [fellowIp UTF8String];
    mainWnd->onNotifyClicked(ip);
}

static void replyCallback(void* arg, long notifyId, NSString* fellowIp, NSString* str)
{
    const char* reply = [str UTF8String];
    const char* ip = [fellowIp UTF8String];

    MainWindow* mainWnd = (MainWindow*)arg;
    mainWnd->onNotifyReplied(notifyId, ip, reply);
}

Notification::Notification()
{

}

int Notification::show(const QString &title, const QString &content, const QString& fellowIp)
{
    NSString* objcTitle = title.toNSString();
    NSString* objcMessage = content.toNSString();
    return [impl show: objcTitle message: objcMessage extra: fellowIp.toNSString()];
}

void Notification::hideAll()
{
    [impl hideAll];
}

void Notification::setMainWnd(MainWindow *mainWnd)
{
    impl->clickCallback = clickCallback;
    impl->replyCallback = replyCallback;
    impl->callbackArg = mainWnd;
}
