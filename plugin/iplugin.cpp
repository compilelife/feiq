#include "iplugin.h"

IPlugin::~IPlugin()
{

}

void IPlugin::init(FeiqWin *feiqWin)
{
    mFeiq = feiqWin;
}

PluginManager::PluginManager()
{

}

PluginManager &PluginManager::instance()
{
    static PluginManager me;
    return me;
}
