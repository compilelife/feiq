#include "tcpsocket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>

TcpSocket::TcpSocket()
{

}

TcpSocket::TcpSocket(int socket)
{
    mSocket = socket;
    if (mSocket != -1)
    {
        sockaddr_in addr;
        socklen_t len = sizeof(addr);
        auto ret = getpeername(mSocket, (sockaddr*)&addr, &len);
        if (ret == 0)
        {
            mPeerIp = inet_ntoa(addr.sin_addr);
        }
    }
}

TcpSocket::~TcpSocket()
{
    disconnect();
}

bool TcpSocket::connect(const string &ip, int port)
{
    if (mSocket != -1)
        return true;

    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocket == -1)
    {
        perror("faield to create socket");
        return false;
    }

    sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int ret = ::connect(mSocket, (sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("failed to connect");
        close(mSocket);
        return false;
    }

    mPeerIp = ip;
    return true;
}

void TcpSocket::disconnect()
{
    if (mSocket != -1)
    {
        close(mSocket);
        mPeerIp="";
    }
}

int TcpSocket::send(const void *data, int size)
{
    int sent = 0;
    auto pdata = static_cast<const char*>(data);

    while (sent < size)
    {
        int ret = ::send(mSocket, pdata+sent, size-sent, 0);
        if (ret == -1 )
        {
            if (errno != EAGAIN)
            {
                perror("tcp send failed");
                return -1;
            }
            continue;
        }

        sent+=ret;
    }

    return sent;
}

int TcpSocket::recv(void *data, int size, int msTimeout)
{
    timeval tv = {msTimeout/1000, (msTimeout%1000)*1000};
    setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int ret = ::recv(mSocket, data, size, 0);
    if (ret == -1)
    {
        if (errno == ETIMEDOUT || errno == EAGAIN)
            return -1;
        else
        {
            perror("recv failed");
            return -2;
        }
    }

    return ret;
}
