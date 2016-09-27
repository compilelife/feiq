#include "uniqueid.h"
#include <limits.h>

UniqueId::UniqueId()
{
    mId = 0;
}

IdType UniqueId::get()
{
    auto id = ++mId;
    if (id >= ULONG_LONG_MAX)
        mId=1;

    return mId;
}
