#include "feiqmodel.h"
#include <functional>

FeiqModel::FeiqModel()
{

}

void FeiqModel::addFellow(shared_ptr<Fellow> fellow)
{
    lock_guard<mutex> guard(mFellowLock);
    mFellows.push_back(fellow);
}

shared_ptr<Fellow> FeiqModel::getFullInfoOf(shared_ptr<Fellow> fellow)
{
    lock_guard<mutex> guard(mFellowLock);
    auto predict = [&fellow](shared_ptr<Fellow> tmp){return fellow->isSame(*tmp);};
    auto found = std::find_if(mFellows.begin(), mFellows.end(), predict);
    return found == mFellows.end() ? nullptr : *found;
}

shared_ptr<Fellow> FeiqModel::findFirstFellowOf(const string &ip)
{
    lock_guard<mutex> guard(mFellowLock);
    auto predict = [&ip](shared_ptr<Fellow> tmp){return tmp->getIp() == ip;};
    auto found = std::find_if(mFellows.begin(), mFellows.end(), predict);
    return found == mFellows.end() ? nullptr : *found;
}

list<shared_ptr<Fellow> > FeiqModel::searchFellow(const string &text)
{
    lock_guard<mutex> guard(mFellowLock);
    list<shared_ptr<Fellow>> fellows;
    if (text.empty())
    {
        fellows = mFellows;
    }
    else
    {
        for (shared_ptr<Fellow> fellow : mFellows)
        {
            if (fellow->getName().find(text) != string::npos
                || fellow->getHost().find(text) != string::npos
                || fellow->getIp().find(text) != string::npos)
                fellows.push_back(fellow);
        }
    }

    return fellows;
}

shared_ptr<Fellow> FeiqModel::getShared(const Fellow *fellow)
{
    if (fellow == nullptr)
        return nullptr;

    lock_guard<mutex> guard(mFellowLock);
    for (shared_ptr<Fellow> f : mFellows)
    {
        if (f.get() == fellow)
            return f;
    }

    return nullptr;
}

shared_ptr<FileTask> FeiqModel::addDownloadTask(shared_ptr<Fellow> fellow, shared_ptr<FileContent> fileContent)
{
    lock_guard<mutex> guard(mFileTaskLock);
    auto task = make_shared<FileTask>(fileContent, FileTaskType::Download);
    task->setFellow(fellow);
    mFileTasks.push_back(task);
    return task;
}

shared_ptr<FileTask> FeiqModel::addUploadTask(shared_ptr<Fellow> fellow, shared_ptr<FileContent> fileContent)
{
    lock_guard<mutex> guard(mFileTaskLock);
    auto task = make_shared<FileTask>(fileContent, FileTaskType::Upload);
    task->setFellow(fellow);
    mFileTasks.push_back(task);
    return task;
}

void FeiqModel::removeFileTask(function<bool (const FileTask&)> predict)
{
    lock_guard<mutex> g(mFileTaskLock);
    mFileTasks.remove_if([predict](shared_ptr<FileTask> t){
        return predict(*t);
    });
}

shared_ptr<FileTask> FeiqModel::findTask(IdType packetNo, IdType fileId, FileTaskType type)
{
    lock_guard<mutex> g(mFileTaskLock);
    for (auto task : mFileTasks) {
        if (task->type() != type)
            continue;

        auto content = task->getContent();
        if (content->fileId == fileId && content->packetNo == packetNo)
            return task;
    }

    return nullptr;
}

list<shared_ptr<FileTask> > FeiqModel::searchTask(function<bool (const FileTask &)> predict)
{
    lock_guard<mutex> g(mFileTaskLock);
    list<shared_ptr<FileTask>> allTask;

    for (auto task : mFileTasks)
        if (predict(*(task.get())))
            allTask.push_back(task);

    return allTask;
}
