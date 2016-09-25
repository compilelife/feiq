#ifndef UDPCOMMU_H
#define UDPCOMMU_H

#include <string>
#include <functional>
#include <vector>
using namespace std;

#define MAX_RCV_SIZE 4096
typedef function<void (const string& ip, vector<char> &data)> UdpRecvHandler;

class UdpCommu
{
public:
    UdpCommu();
public:
    /**
     * @brief bindTo 绑定到本地端口
     * @param port 端口号
     * @return 是否绑定成功
     */
    bool bindTo(int port);

    /**
     * @brief sentTo 往目标地址发送数据
     * @param ip 目标ip
     * @param port 目标端口
     * @param data 数据指针
     * @param size 数据大小
     * @return >=0发送成功的字节数，-1发送失败
     */
    int sentTo(const string &ip, int port, const void *data, int size);

    /**
     * @brief startAsyncRecv 开始异步接收数据
     * @param handler 处理函数
     * @return 是否启动成功
     */
    bool startAsyncRecv(UdpRecvHandler handler);

    /**
     * @brief close 关闭udp通信
     */
    void close();

    /**
     * @brief getBoundMac 获取udp通信绑定的网卡
     * @return 绑定的网卡地址
     */
    string getBoundMac();
public:
    /**
     * @brief getErrMsg 获取最近一次错误的错误信息
     * @return 最近一次的错误信息
     */
    string getErrMsg();

private:
    void recvThread();
    bool mAsyncMode=false;

private:
    string mErrMsg="";
    int mSocket=-1;
    UdpRecvHandler mRecvHandler=nullptr;
};

#endif // UDPCOMMU_H
