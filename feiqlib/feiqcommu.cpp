#include "feiqcommu.h"
#include "udpcommu.h"
#include "ipmsg.h"
#include <arpa/inet.h>
#include "fellow.h"
#include <sstream>
#include <QDebug>
#include <limits.h>
#include "utils.h"

FeiqCommu::FeiqCommu()
{
}

void FeiqCommu::setMyHost(string host){mHost=host;}

void FeiqCommu::setMyName(string name){
    mName=name;
    std::replace(mName.begin(), mName.end(), HLIST_ENTRY_SEPARATOR, (char)HOSTLIST_DUMMY);
}

void FeiqCommu::addRecvProtocol(RecvProtocol *protocol){
    mRecvPrtocols.push_back(protocol);
}

pair<bool, string> FeiqCommu::start()
{
    if (!mUdp.bindTo(IPMSG_PORT))
    {
        return {false, "bind failed:"+mUdp.getErrMsg()};
    }

    if (!mUdp.startAsyncRecv(
            std::bind(&FeiqCommu::onRecv, this, placeholders::_1,placeholders::_2)
            )
        )
    {
        mUdp.close();
        return {false, "start aysnc recv failed:"+mUdp.getErrMsg()};
    }

    if (!mTcpServer.start(IPMSG_PORT)){
        mUdp.close();
        return {false, "无法启动文件服务"};
    }
    mTcpServer.whenNewClient(std::bind(&FeiqCommu::onTcpClientConnected, this, placeholders::_1));

    //其他字段是什么意思呢？
    mMac = mUdp.getBoundMac();
    mVersion = "1_lbt6_0#128#"+mMac+"#0#0#0#4001#9";
    return {true, ""};
}

void FeiqCommu::stop()
{
    mUdp.close();
}

pair<IdType, string> FeiqCommu::send(const string &ip, SendProtocol &sender)
{
    //打包
    IdType packetNo=0;
    auto out = pack(sender, &packetNo);

    //发送
    auto ret = mUdp.sentTo(ip, IPMSG_PORT, out.data(), out.size());
    if (ret < 0)
        return {0, mUdp.getErrMsg()};

    return {packetNo, ""};
}

class SendRequestFile : public SendProtocol
{
public:
    int filetype=IPMSG_FILE_REGULAR;
    int fileid;
    int offset=0;
    int packetNo;

    int cmdId() override {return filetype == IPMSG_FILE_DIR ? IPMSG_GETDIRFILES : IPMSG_GETFILEDATA;}
    void write(ostream& os) override
    {
        char sep = HLIST_ENTRY_SEPARATOR;
        os<<std::hex<<packetNo<<sep
         <<fileid<<sep
        <<offset<<sep;
    }
};

unique_ptr<TcpSocket> FeiqCommu::requestFileData(const string &ip,
                                const FileContent& file, int offset)
{
    unique_ptr<TcpSocket> client(new TcpSocket());
    if (!client->connect(ip, IPMSG_PORT))
        return nullptr;

    SendRequestFile requestSender;
    requestSender.packetNo = file.packetNo;
    requestSender.fileid = file.fileId;
    requestSender.offset = offset;
    auto request = pack(requestSender);

    int ret = client->send(request.data(), request.size());
    if (ret < 0)
        return nullptr;

    return client;
}

void FeiqCommu::setFileServerHandler(FileServerHandler fileServerHandler)
{
    mFileServerHandler = fileServerHandler;
}

void FeiqCommu::onRecv(const string &ip, vector<char> &data)
{
    auto post = make_shared<Post>();
    post->from->setIp(ip);

    //解析
    if (!dumpRaw(data, *post))
        return;

    //尝试获取mac
    auto info = dumpVersionInfo(post->from->version());
    post->from->setMac(info.mac);

    //屏蔽自己的包
    if (mMac == post->from->getMac()//匹配mac
        && mName == post->from->getName())//再匹配名字，以防mac获取失败
    {
        return;
    }

    //除非收到下线包，否则都认为在线
    post->from->setOnLine(true);

    //调用协议处理
    for (auto& handler : mRecvPrtocols)
    {
        if (handler->read(post))
            break;
    }
}

vector<char> FeiqCommu::pack(SendProtocol &sender, IdType* packetId)
{
    //搜集数据
    char sep  = HLIST_ENTRY_SEPARATOR;
    auto packetNo = mPacketNo.get();
    auto cmdId = sender.cmdId();

    //拼接消息头
    stringstream os;
    os<<mVersion<<sep<<packetNo<<sep<<mName<<sep<<mHost<<sep<<cmdId<<sep;

    //组装消息
    sender.write(os);
    os.put(0);

    os.seekg(0, os.end);
    auto len = os.tellg();
    os.seekg(0, os.beg);

    vector<char> buf(len);
    os.read(buf.data(), len);

    if (packetId != nullptr)
        *packetId = packetNo;
    return buf;
}

void FeiqCommu::onTcpClientConnected(int socket)
{
    if (mFileServerHandler)
    {
        //接收请求
        unique_ptr<TcpSocket> client(new TcpSocket(socket));
        std::array<char,MAX_RCV_SIZE> buf;
        int ret = client->recv(buf.data(), MAX_RCV_SIZE);
        if (ret <= 0)
            return;

        //解析请求
        vector<char> request(ret);
        std::copy(buf.begin(), buf.begin()+ret, request.begin());

        Post post;
        if (!dumpRaw(request, post))
            return;

        auto values = splitAllowSeperator(post.extra.begin(), post.extra.end(), HLIST_ENTRY_SEPARATOR);
        if (values.size() < 3)
            return;

        int packetNo = stoi(values[0], 0, 16);
        int fileId = stoi(values[1], 0, 16);
        int offset = stoi(values[2], 0, 16);

        //处理请求
        mFileServerHandler(std::move(client), packetNo, fileId, offset);
    }
}

bool FeiqCommu::dumpRaw(vector<char>& data, Post& post)
{
    auto ptr = data.begin();
    auto last = data.end();

    //取出协议的前5项
    array<string, 5> value;
    auto count = 0uL;
    while (count < value.size()) {
        auto found = std::find(ptr, last, HLIST_ENTRY_SEPARATOR);
        if (found == last)
            break;

        auto size = std::distance(ptr, found);
        if (size == 0)
        {
            value[count]="";
        }
        else
        {
            vector<char> buf(size+1);
            std::copy(ptr, found, buf.begin());
            buf.push_back(0);
            std::replace(buf.begin(), buf.end(), HOSTLIST_DUMMY, HLIST_ENTRY_SEPARATOR);
            value[count]=toString(buf);//TODO:是否有编码问题？
        }

        ptr=found+1;
        ++count;
    }

    //协议错误
    if (count < value.size())
        return false;

    //解析
    post.from->setVersion(value[0]);
    post.packetNo = value[1];
    if (!post.from)
        post.from=make_shared<Fellow>();
    post.from->setPcName(value[2]);
    post.from->setHost(value[3]);
    post.cmdId = stoull(value[4]);

    //取出extra部分
    if (ptr != last)
    {
        auto size = std::distance(ptr, last);
        vector<char> buf(size);
        std::copy(ptr, last, buf.begin());
        post.extra = buf;
    }

    return true;
}

VersionInfo FeiqCommu::dumpVersionInfo(const string &version)
{
    const char sep = '#';
    VersionInfo info;

    auto tail = version.end();
    auto head = version.begin();
    //1
    auto begin = std::find(head, tail, sep);
    if (begin == tail)
        return info;

    //2=begin
    begin = std::find(begin+1, tail, sep);
    if (begin == tail)
        return info;
    ++begin;

    //3=end
    auto end = std::find(begin, tail, sep);
    if (end == tail)
        return info;

    info.mac = version.substr(distance(head,begin), distance(begin,end));

    return info;
}

