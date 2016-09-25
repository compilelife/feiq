#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <functional>

class TcpServer
{
public:
    TcpServer();
    typedef std::function<void (int socket)> ClientHandler;
public:
    bool start(int port);
    void whenNewClient(ClientHandler onClientConnected);
    void stop();
private:
    void keepAccept();
private:
    ClientHandler mClientHandler;
    bool mStarted=false;
    int mSocket;
};

#endif // TCPSERVER_H
