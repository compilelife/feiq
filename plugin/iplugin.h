#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "../feiqwin.h"

class IPlugin
{
public:
    virtual ~IPlugin(){}
    virtual void init(FeiqWin* feiqWin)
    {
        mFeiq = feiqWin;
    }

    virtual void unInit() = 0;

protected:
    FeiqWin* mFeiq;
};

#endif // IPLUGIN_H
