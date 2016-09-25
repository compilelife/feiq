#ifndef IFEIQVIEW_H
#define IFEIQVIEW_H

#include <memory>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

#include "fellow.h"
#include "content.h"
#include "filetask.h"
#include "post.h"

enum class ViewEventType {
    FELLOW_UPDATE,
    MESSAGE,
    SEND_TIMEO,
};

class ViewEvent
{
public:
    virtual ~ViewEvent(){}
public:
    decltype(Post::now()) when = Post::now();
    ViewEventType what;
};

class FellowViewEvent : public ViewEvent
{
public:
    FellowViewEvent(){
        what = ViewEventType::FELLOW_UPDATE;
    }
    shared_ptr<Fellow> fellow;
};

class SendTimeoEvent : public FellowViewEvent
{
public:
    SendTimeoEvent(){
        what = ViewEventType::SEND_TIMEO;
    }

    shared_ptr<Content> content;
};

class MessageViewEvent : public FellowViewEvent
{
public:
    MessageViewEvent(){
        what = ViewEventType::MESSAGE;
    }
    vector<shared_ptr<Content>> contents;
};

class IFeiqView : public IFileTaskObserver
{
public:
    virtual ~IFeiqView(){}
    virtual void onEvent(shared_ptr<ViewEvent> event) = 0;
};

#endif // IFEIQVIEW_H
