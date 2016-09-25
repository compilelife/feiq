#include "msgqueuethread.h"
#include <thread>

MsgQueueThread::MsgQueueThread()
{

}

void MsgQueueThread::setHandler(MsgQueueThread::Handler handler)
{

}

void MsgQueueThread::start()
{
    if (mRun)
        return;

    mRun=true;
    thread thd(&MsgQueueThread::loop, this);
    thd.detach();
}

void MsgQueueThread::stop()
{
    mRun=false;
    unique_lock<mutex> lock(mQueueMutex);
    mQueueCnd.notify_all();
}
