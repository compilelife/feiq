#include "filetask.h"

FileTask::FileTask()
{

}

FileTask::FileTask(shared_ptr<FileContent> fileContent, FileTaskType type)
    :mContent(fileContent), mType(type)
{
    mNotifySize = fileContent->size/100;//每1%通知一次
    const int minNotifySize = 102400;//至少变化了100k才通知
    if (mNotifySize < minNotifySize)
        mNotifySize = minNotifySize;
}

void FileTask::setObserver(IFileTaskObserver *observer)
{
    mObserver = observer;
}

void FileTask::setProcess(int val)
{
    mProcess = val;
    if (mProcess - mLastProcess >= mNotifySize)
    {
        mLastProcess = mProcess;
        mObserver->onProgress(this);
    }
}

void FileTask::setState(FileTaskState val, const string &msg)
{
    mState = val;
    mMsg = msg;
    mObserver->onStateChanged(this);
}

void FileTask::setFellow(shared_ptr<Fellow> fellow)
{
    mFellow = fellow;
}

void FileTask::cancel()
{
    mCancelPending=true;
}

bool FileTask::hasCancelPending()
{
    return mCancelPending;
}

shared_ptr<Fellow> FileTask::fellow() const
{
    return mFellow;
}

int FileTask::getProcess() const
{
    return mProcess;
}

FileTaskState FileTask::getState() const
{
    return mState;
}

string FileTask::getDetailInfo() const
{
    return mMsg;
}

shared_ptr<FileContent> FileTask::getContent() const
{
    return mContent;
}

FileTaskType FileTask::type() const
{
    return mType;
}
