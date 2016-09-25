#ifndef CONTENT_H
#define CONTENT_H

#include <string>
#include "protocol.h"
#include "uniqueid.h"
#include <sys/stat.h>
#include "ipmsg.h"
#include "utils.h"
#include "parcelable.h"
using namespace std;

enum class ContentType{Text, Knock, File, Image, Id};



/**
 * @brief 消息内容
 */
class Content : public Parcelable
{
public:
    IdType packetNo;
    void setPacketNo(string val){
        packetNo = stoul(val);
    }
    void setPacketNo(IdType val){
        packetNo = val;
    }

    virtual ~Content(){}
    ContentType type() const {return mType;}

protected:
    ContentType mType;

public:
    virtual void writeTo(Parcel& out) const override
    {
        out.write(mType);
        out.write(packetNo);
    }
    virtual void readFrom(Parcel& in) override
    {
        in.read(mType);
        in.read(packetNo);
    }
};

class IdContent : public Content
{
public:
    IdContent(){mType = ContentType::Id;}
    IdType id;
};

class TextContent : public Content
{
public:
    TextContent(){mType = ContentType::Text;}
    string text;
    string format;

public:
    virtual void writeTo(Parcel& out) const override
    {
        Content::writeTo(out);
        out.writeString(text);
        out.writeString(format);
    }

    virtual void readFrom(Parcel& in) override
    {
        Content::readFrom(in);
        in.readString(text);
        in.readString(format);
    }
};

class FileContent : public Content
{
public:
    FileContent(){mType = ContentType::File;}
    IdType fileId;
    string filename;
    string path;//保存路径或要发送的文件的路径
    int size = 0;
    int modifyTime = 0;
    int fileType = 0;

public:
    static unique_ptr<FileContent> createFileContentToSend(const string& filePath)
    {
        static UniqueId mFileId;
        struct stat fInfo;
        auto ret = stat(filePath.c_str(), &fInfo);
        if (ret != 0)
            return nullptr;

        unique_ptr<FileContent> file(new FileContent());
        file->fileId = mFileId.get();
        file->path = filePath;
        file->filename = getFileNameFromPath(filePath);
        if (S_ISREG(fInfo.st_mode))
            file->fileType = IPMSG_FILE_REGULAR;
        else if (S_ISREG(fInfo.st_mode))
            file->fileType = IPMSG_FILE_DIR;
        else
            return nullptr;//先不支持其他类型
        file->size = fInfo.st_size;
        file->modifyTime = fInfo.st_mtimespec.tv_sec;

        return file;
    }
};

class KnockContent: public Content
{
public:
    KnockContent(){mType = ContentType::Knock;}
};

class ImageContent: public Content
{
public:
    ImageContent(){mType = ContentType::Image;}
    string id;
};

class ContentParcelFactory
{
public:
    static unique_ptr<Content> createFromParcel(Parcel& in)
    {
        //先读取父类信息
        Content content;
        auto pos = in.mark();
        content.readFrom(in);
        in.unmark(pos);

        //根据类型读取剩余数据
        Content* ptr = nullptr;
        switch (content.type()) {
        case ContentType::Text:
            ptr = new TextContent;
            break;
        case ContentType::Knock:
            ptr = new KnockContent;
            break;
        case ContentType::File:
            ptr = new FileContent;
            break;
        case ContentType::Image:
            ptr = new ImageContent;
            break;
        case ContentType::Id:
            ptr = new IdContent;
            break;
        default:
            break;
        }

        if (ptr)
            ptr->readFrom(in);

        return unique_ptr<Content>(ptr);
    }
};

#endif // CONTENT_H
