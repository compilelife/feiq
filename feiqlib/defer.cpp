#include "defer.h"

Defer::Defer(function<void ()> deleter)
    :mDeleter(deleter)
{

}

Defer::~Defer()
{
    if (mDeleter)
        mDeleter();
}
