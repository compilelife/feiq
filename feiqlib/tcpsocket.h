#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <string>
using namespace std;

class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(int socket);
    ~TcpSocket();

public:
    bool connect(const string& ip, int port);
    void disconnect();

public:
    int send(const void* data, int size);
    /**
     * @brief recv 阻塞等待数据
     * @param data 接收缓冲区
     * @param size 最大接收字节数
     * @param msTimeout 接收超时（毫秒）
     * @return 接收到的字节数；超时返回-1，其他错误返回-2
     */
    int recv(void* data, int size, int msTimeout = 1000);

private:
    int mSocket=-1;
    string mPeerIp;
};

#endif // TCPSOCKET_H
