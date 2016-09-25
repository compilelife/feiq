#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <ostream>
using namespace std;

class Post;

class SendProtocol
{
public:
    virtual int cmdId() = 0;
    virtual void write(ostream& os) = 0;
};

class RecvProtocol
{
public:
    /**
     * @brief read 解析收到的数据
     * @param post 当前好友请求，整个解析链中被不断读写完善，在解析链最后得到一个完整的Post
     * @return true 已完成解析，无需后续的解析链，false 继续解析
     */
    virtual bool read(shared_ptr<Post> post) = 0;
};

#endif // PROTOCOL_H
