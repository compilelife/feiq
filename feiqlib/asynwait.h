#ifndef ASYNWAIT_H
#define ASYNWAIT_H

#include "uniqueid.h"
#include <functional>
#include <list>
#include <mutex>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

class AsynWait
{
public:
    typedef function<void (IdType)> OnWaitTimeout;
    AsynWait();
    void start(int precision=200);
    void stop();

public:
    void addWaitPack(IdType packetId, OnWaitTimeout onTimeout, int msTimeo);
    void clearWaitPack(IdType packetId);

private:
    void run();
private:
    struct WaitPack{
        IdType id;
        OnWaitTimeout handler;
        time_point<system_clock, milliseconds> timeo;
    };

    list<WaitPack> mWaitPacks;
    mutex mPacksMutex;
    bool mStarted=false;
    int mPrecision;
    thread mThd;
};

#endif // ASYNWAIT_H
