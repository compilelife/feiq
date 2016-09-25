#ifndef POST_H
#define POST_H

#include <memory>
#include <vector>
#include <list>
#include <string>
#include "content.h"
#include <chrono>

using namespace std;
using namespace std::chrono;
class Fellow;

/**
 * @brief The Post struct
 * 定义一次好友发来的请求/数据包
 */
class Post
{
public:
    static time_point<system_clock, milliseconds> now(){
        return time_point_cast<milliseconds>(system_clock::now());
    }

    decltype(now()) when = Post::now();
    vector<char> extra;

    string packetNo;
    IdType cmdId;

    shared_ptr<Fellow> from;
    vector<shared_ptr<Content>> contents;

    Post()
    {
        from = make_shared<Fellow>();
    }

    void addContent(shared_ptr<Content> content)
    {
        content->setPacketNo(packetNo);
        contents.push_back(content);
    }
};

#endif // POST_H
