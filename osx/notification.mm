#include "notification.h"

#import <cocoa/Cocoa.h>

Notification::Notification()
{

}

void Notification::show(const QString &title, const QString &content)
{
    NSUserNotification *userNotification = [[[NSUserNotification alloc] init] autorelease];
    userNotification.title = title.toNSString();
    userNotification.informativeText = content.toNSString();
    NSUserNotificationCenter* center = [NSUserNotificationCenter defaultUserNotificationCenter];
    [center deliverNotification:userNotification];
}

void Notification::hideAll()
{
    [[NSUserNotificationCenter defaultUserNotificationCenter] removeAllDeliveredNotifications];
}
