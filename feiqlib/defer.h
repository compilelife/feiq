#ifndef DEFER_H
#define DEFER_H

#include <functional>
using namespace std;

class Defer
{
public:
    Defer(function<void ()> deleter);
    ~Defer();
private:
    function<void ()> mDeleter;
};

#endif // DEFER_H
