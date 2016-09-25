#ifndef FEIQCOMMU_H
#define FEIQCOMMU_H

#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include <array>
#include "post.h"
#include "protocol.h"
#include "udpcommu.h"
#include <atomic>
#include "encoding.h"
#include "tcpsocket.h"
#include "tcpserver.h"
#include "uniqueid.h"
using namespace std;

struct VersionInfo
{
    string mac;
};

/**
 * @brief 提供基础的feiq通信功能，对udp、tcp封装，负责消息的打包、解包
 */
class FeiqCommu
{
public:
    typedef function<void (unique_ptr<TcpSocket>, int packetNo, int fileId, int offset)> FileServerHandler;
    FeiqCommu();

public:
    void setMyHost(string host);
    void setMyName(string name);
    void addRecvProtocol(RecvProtocol* protocol);

public:
    /**
     * @brief start 启动feiq通信
     * @return 是否启动成功，如果失败，返回具体失败原因
     */
    pair<bool, string> start();
    void stop();

    /**
     * @brief send 发送sender打包的内容
     * @param ip 发给谁
     * @param sender 要发送什么
     * @return 发送成功，返回发送包ID，否则返回-1，并设置失败原因
     */
    pair<IdType, string> send(const string& ip, SendProtocol& sender);

    /**
     * @brief requestFileData 请求好友开始发送文件数据
     * @param ip 向谁请求
     * @param file 要请求的文件
     * @return 如果请求成功，返回tcp连接，据此获取数据，否则返回nullptr
     */
    unique_ptr<TcpSocket> requestFileData(const string& ip, const FileContent &file, int offset);

    /**
     * @brief setFileServerHandler 设置文件服务的处理
     * @param fileServerHandler 参数：客户端socket连接，请求的文件id，请求的数据偏移
     */
    void setFileServerHandler(FileServerHandler fileServerHandler);
public:
    static bool dumpRaw(vector<char> &data, Post &post);
    static VersionInfo dumpVersionInfo(const string& version);
private:
    void onRecv(const string& ip, vector<char> &data);
    vector<char> pack(SendProtocol& sender, IdType *packetId = nullptr);
    void onTcpClientConnected(int socket);
private:
    vector<RecvProtocol*> mRecvPrtocols;
    UdpCommu mUdp;
    string mHost="";
    string mName="";
    string mVersion="";
    UniqueId mPacketNo;
    string mMac;
    TcpServer mTcpServer;
    FileServerHandler mFileServerHandler;
};

#endif // FEIQCOMMU_H
