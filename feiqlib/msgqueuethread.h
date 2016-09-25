#ifndef MSGQUEUETHREAD_H
#define MSGQUEUETHREAD_H

#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <thread>
using namespace std;

//TODO:实现移到cpp中
template<class Msg>
class MsgQueueThread
{
    typedef function<void (shared_ptr<Msg>)> Handler;
public:
    void setHandler(Handler handler)
    {
        mHandler = handler;
    }

    void start()
    {
        if (mRun)
            return;

        mRun=true;
        thread thd(&MsgQueueThread::loop, this);
        mThread.swap(thd);
    }

    void stop()
    {
        if (!mRun)
            return;

        mRun=false;
        unique_lock<mutex> lock(mQueueMutex);
        while(!mQueue.empty())
            mQueue.pop();
        mQueueCnd.notify_all();
        lock.unlock();
        mThread.join();
    }

    void sendMessage(shared_ptr<Msg> msg)
    {
        unique_lock<mutex> lock(mQueueMutex);
        mQueue.push(msg);
        mQueueCnd.notify_one();
    }

private:
    void loop()
    {
        while (mRun) {
            unique_lock<mutex> lock(mQueueMutex);
            if (mQueue.empty())
                mQueueCnd.wait(lock);
            if (!mRun)
                return;

            auto msg = mQueue.front();
            mQueue.pop();
            lock.unlock();//队列已经没有利用价值了，放了它

            if (mHandler)
                mHandler(msg);
        }

    }

private:
    condition_variable mQueueCnd;
    mutex mQueueMutex;
    queue<shared_ptr<Msg>> mQueue;
    bool mRun=false;
    Handler mHandler;
    thread mThread;
};

#endif // MSGQUEUETHREAD_H
