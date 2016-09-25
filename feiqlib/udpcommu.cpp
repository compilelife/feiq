#include "udpcommu.h"
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if_dl.h>
#include <sys/sysctl.h>
#include <array>

#define setFailedMsgAndReturnFalse(msg) \
    {mErrMsg = msg;\
    return false;}

#define setErrnoMsgAndReturnFalse()\
    {mErrMsg = strerror(errno);\
    return false;}

#define setErrnoMsg()   mErrMsg = strerror(errno);

UdpCommu::UdpCommu(){}

bool UdpCommu::bindTo(int port)
{
    if (mSocket != -1)
        setFailedMsgAndReturnFalse("已经初始化");

    //创建socket
    mSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if (mSocket == -1)
        setErrnoMsgAndReturnFalse();

    auto ret = -1;
    //允许广播
    auto broadcast = 1;
    ret = setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(int));
    if (ret == -1)
        setErrnoMsgAndReturnFalse();

    //地址复用
    auto reuse = 1;
    ret = setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    if (ret == -1)
        setErrnoMsgAndReturnFalse();

    //绑定
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    ret = ::bind(mSocket, (sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
        setErrnoMsgAndReturnFalse();

    return true;
}

int UdpCommu::sentTo(const string& ip, int port, const void *data, int size)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    auto ret = ::sendto(mSocket, data, size, 0, (sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
        setErrnoMsg();

    return ret;
}

bool UdpCommu::startAsyncRecv(UdpRecvHandler handler)
{
    if (handler == nullptr)
        setFailedMsgAndReturnFalse("handler不能为空")

    if (mSocket == -1)
        setFailedMsgAndReturnFalse("请先初始化socket");

    mRecvHandler = handler;
    if (!mAsyncMode)
    {
        mAsyncMode = true;
        std::thread t(&UdpCommu::recvThread, this);
        t.detach();
    }

    return true;
}

void UdpCommu::close()
{
    if (mSocket == -1)
        return;

    ::close(mSocket);
    mSocket = -1;
    mAsyncMode=false;
}

string UdpCommu::getBoundMac()
{
    //osx未定义SIOCGIFHWADDR,写死获取en0
    int         mib[6];
    size_t len=0;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;

    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        perror("if_nametoindex error");
        return "";
    }

    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        perror("sysctl 1 error");
        return "";
    }

    unique_ptr<char[]> buf(new char[len]);
    if (sysctl(mib, 6, buf.get(), &len, NULL, 0) < 0) {
        perror("sysctl 2 error");
        return "";
    }

    ifm = (struct if_msghdr *)buf.get();
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);

    char macStr[20]={0};
    snprintf(macStr, sizeof(macStr), "%02x%02x%02x%02x%02x%02x", *ptr, *(ptr+1), *(ptr+2),
           *(ptr+3), *(ptr+4), *(ptr+5));

    return macStr;
}

string UdpCommu::getErrMsg()
{
    return mErrMsg;
}

void UdpCommu::recvThread()
{
    timeval timeo = {3,0};
    auto ret = setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeval));
    if (ret != 0){
        printf("faield to set recv timeo\n");
        mAsyncMode=false;
        return;
    }

    std::array<char,MAX_RCV_SIZE> buf;
    sockaddr_in addr;
    socklen_t len = sizeof(addr);

    while (mSocket != -1) {
        buf.fill(0);
        memset(&addr, 0, len);

        auto size = recvfrom(mSocket, buf.data(), MAX_RCV_SIZE, 0, (sockaddr*)&addr, &len);
        if (size < 0)
        {
            if (errno == EAGAIN || errno == ETIMEDOUT)
                continue;

            printf("error occur:%s\n", strerror(errno));
            break;
        }

        auto ip = inet_ntoa(addr.sin_addr);
        vector<char> data(std::begin(buf), std::begin(buf)+size);
        mRecvHandler(ip, data);
    }

    printf("end recv thread\n");
    mAsyncMode=false;
}
