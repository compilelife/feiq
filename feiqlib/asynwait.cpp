#include "asynwait.h"
#include <thread>
#include <unistd.h>

AsynWait::AsynWait()
{

}

void AsynWait::start(int precision)
{
    if (mStarted == true)
        return;

    mStarted=true;
    mPrecision = precision;

    thread thd(&AsynWait::run, this);
    mThd.swap(thd);
}

void AsynWait::stop()
{
    mStarted=false;
    mThd.join();
}

void AsynWait::addWaitPack(IdType packetId, AsynWait::OnWaitTimeout onTimeout, int msTimeo)
{
    WaitPack pack;
    pack.id = packetId;
    pack.handler = onTimeout;
    pack.timeo = time_point_cast<milliseconds>(system_clock::now()) + milliseconds(msTimeo);

    mPacksMutex.lock();
    mWaitPacks.push_back(pack);
    mPacksMutex.unlock();
}

void AsynWait::clearWaitPack(IdType packetId)
{
    mPacksMutex.lock();
    mWaitPacks.remove_if([packetId](WaitPack pack){
        return pack.id = packetId;
    });
    mPacksMutex.unlock();
}

void AsynWait::run()
{
    list<WaitPack> timeos;
    while (mStarted) {
        usleep(mPrecision*1000);
        timeos.clear();

        auto cur = system_clock::now();
        mPacksMutex.lock();
        mWaitPacks.remove_if([&cur, &timeos](const WaitPack& pack){
            if (cur > pack.timeo)
            {
                timeos.push_back(pack);
                return true;
            }
            return false;
        });
        mPacksMutex.unlock();

        if (!timeos.empty())
        {
            for (auto& pack : timeos)
                pack.handler(pack.id);
        }
    }
}

