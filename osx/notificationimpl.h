#ifndef NOTIFICATIONIMPL_H
#define NOTIFICATIONIMPL_H

#ifdef __cplusplus
extern "C"{
#endif

#import <Foundation/Foundation.h>

typedef void(*ClickCallback)(void*, NSString*);
typedef void(*ReplyCallback)(void*, long, NSString*, NSString*);

@interface NotificationImpl : NSObject
    {
        @public
        ClickCallback clickCallback;
        ReplyCallback replyCallback;
        void* callbackArg;

        @private
        int id;
    }
    -(instancetype)init;
    -(long)show: (NSString*)title message:(NSString*)msg extra:(NSString*)who;
    -(void)hideAll;
@end

#ifdef __cplusplus
}
#endif

#endif // NOTIFICATIONIMPL_H
