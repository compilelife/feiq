#include "tcpserver.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>

using namespace std;

TcpServer::TcpServer()
{

}

bool TcpServer::start(int port)
{
    if (mStarted)
        return true;

    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocket == -1)
    {
        perror("create tcp socket failed: %s");
        return false;
    }

    int val = 1;
    setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    int ret = ::bind(mSocket, (sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind failed");
        return false;
    }

    ret = listen(mSocket, 5);
    if (ret == -1)
    {
        perror("listen failed");
        return false;
    }

    mStarted=true;
    thread thd(&TcpServer::keepAccept, this);
    thd.detach();

    return true;
}

void TcpServer::whenNewClient(TcpServer::ClientHandler onClientConnected)
{
    mClientHandler = onClientConnected;
}

void TcpServer::stop()
{
    mStarted = false;
    close(mSocket);
}

void TcpServer::keepAccept()
{
    while (mStarted) {
        sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int ret = ::accept(mSocket, (sockaddr*)&addr, &len);
        if (ret < 0)
        {
            perror("failed to accept");
            break;
        }

        if (mClientHandler)
        {
            mClientHandler(ret);
        }
    }
}
