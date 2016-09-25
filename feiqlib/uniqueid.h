#ifndef UNIQUEID_H
#define UNIQUEID_H

#include <atomic>
using namespace std;

typedef unsigned long long IdType;
/**
 * @brief The UniqueId class 返回大于0小于MAX_ULONG的数，多线程安全，自动归0
 */
class UniqueId
{
public:
    UniqueId();
public:
    IdType get();
private:
    atomic_ullong mId;
};

#endif // UNIQUEID_H
