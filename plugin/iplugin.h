#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "../feiqwin.h"
#include <unordered_map>
#include <iostream>

class IPlugin
{
public:
    virtual ~IPlugin();
    virtual void init(FeiqWin* feiqWin);

    virtual void unInit() = 0;

protected:
    FeiqWin* mFeiq;
};

class PluginManager
{
private:
    PluginManager();
public:
    static PluginManager& instance();
public:
    std::unordered_map<const char*, IPlugin *> allPlugins;
};

#define REGISTER_PLUGIN(name, PluginCls)\
__attribute__((constructor)) void register##PluginCls()\
{\
    PluginManager::instance().allPlugins[name]=new PluginCls();\
}
#endif // IPLUGIN_H
