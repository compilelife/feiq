#include "notificationimpl.h"
#include <QDebug>

@interface NotificationImpl () <NSUserNotificationCenterDelegate>
@end

@implementation NotificationImpl

-(instancetype)init
{
    self = [super init];

    if(self)
    {
        id=0;
        [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:self];
    }

    return self;
}

-(long)show:(NSString*)title message:(NSString*)msg extra:(NSString*)who
{
    NSUserNotification* notification = [[NSUserNotification alloc] init];
    notification.title = title;
    if (who != nil && who.length > 0)
    {
        notification.userInfo=@{@"extra":who};
        notification.hasReplyButton = true;
    }
    int myid = ++id;
    notification.identifier =  [NSString stringWithFormat:@"%d",myid];
    notification.informativeText = msg;

    [[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification: notification];

    return (long)myid;
}

-(void)hideAll
{
    [[NSUserNotificationCenter defaultUserNotificationCenter] removeAllDeliveredNotifications];
}

-(BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresent:(NSUserNotification *)notification
{
    return YES;
}
-(void)userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification
{
    NSString* who = [notification.userInfo objectForKey:@"extra"];
    int myid = notification.identifier.intValue;
    if (notification.activationType == NSUserNotificationActivationTypeContentsClicked){
        clickCallback(callbackArg, who);
    }
    else if (notification.activationType == NSUserNotificationActivationTypeReplied){
        replyCallback(callbackArg, myid, who, notification.response.string);
    }
}

@end
