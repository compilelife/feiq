#ifndef FILETASK_H
#define FILETASK_H


#include "content.h"
#include <memory>
#include <functional>
#include "fellow.h"
#include <string>
using namespace std;

enum class FileTaskType{
    Download,
    Upload
};

enum class FileTaskState{
    NotStart,
    Running,
    Finish,
    Error,
    Canceled
};

class FileTask;
class IFileTaskObserver
{
public:
    virtual void onStateChanged(FileTask* fileTask) = 0;
    virtual void onProgress(FileTask* fileTask) = 0;
};

class FileTask
{
public:
    FileTask();
    FileTask(shared_ptr<FileContent> fileContent, FileTaskType type);
    void setObserver(IFileTaskObserver* observer);
public:
    void setProcess(int val);
    void setState(FileTaskState val, const string& msg="");
    void setFellow(shared_ptr<Fellow> fellow);
    void cancel();
    bool hasCancelPending();
public:
    shared_ptr<Fellow> fellow() const;
    int getProcess() const;
    FileTaskState getState() const;
    string getDetailInfo() const;
    shared_ptr<FileContent> getContent() const;
    FileTaskType type() const;
    string getTaskTypeDes() const;
private:
    shared_ptr<Fellow> mFellow;//要发送给的用户，或文件来自该用户
    int mProcess=0;
    FileTaskState mState = FileTaskState::NotStart;
    shared_ptr<FileContent> mContent;
    IFileTaskObserver* mObserver;
    FileTaskType mType = FileTaskType::Upload;
    string mMsg;
    bool mCancelPending=false;
    int mNotifySize;
    int mLastProcess=0;
};

#endif // FILETASK_H
