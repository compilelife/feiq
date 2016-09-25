#ifndef FEIQMODEL_H
#define FEIQMODEL_H

#include "fellow.h"
#include <memory>
#include <list>
#include <mutex>
#include "filetask.h"
#include "uniqueid.h"
using namespace std;

class FeiqModel
{
public:
    FeiqModel();

public:
    void addFellow(shared_ptr<Fellow> fellow);
    shared_ptr<Fellow> getFullInfoOf(shared_ptr<Fellow> fellow);
    shared_ptr<Fellow> findFirstFellowOf(const string& ip);
    list<shared_ptr<Fellow>> searchFellow(const string& text);
    shared_ptr<Fellow> getShared(const Fellow* fellow);

public:
    shared_ptr<FileTask> addDownloadTask(shared_ptr<Fellow> fellow, shared_ptr<FileContent> fileContent);
    shared_ptr<FileTask> addUploadTask(shared_ptr<Fellow> fellow, shared_ptr<FileContent> fileContent);
    shared_ptr<FileTask> findTask(IdType packetNo, IdType fileId, FileTaskType type = FileTaskType::Upload);
    list<shared_ptr<FileTask>> searchTask(function<bool(const FileTask&)> predict);
    void removeFileTask(function<bool (const FileTask&)> predict);

private:
    list<shared_ptr<Fellow>> mFellows;
    list<shared_ptr<FileTask>> mFileTasks;
    mutex mFellowLock;
    mutex mFileTaskLock;
};

#endif // FEIQMODEL_H
