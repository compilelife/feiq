#include "uniqueid.h"
#include <limits.h>

UniqueId::UniqueId()
{
    mId = 0;
}

IdType UniqueId::get()
{
    auto id = ++mId;
    if (id >= INT_MAX || id < 0)
        mId=1;

    return mId;
}
