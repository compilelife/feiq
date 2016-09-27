#include "iplugin.h"

IPlugin::~IPlugin()
{

}

void IPlugin::setFeiqWin(FeiqWin *feiqWin)
{
    mFeiq = feiqWin;
}

void IPlugin::init()
{
}

void IPlugin::unInit()
{

}

PluginManager::PluginManager()
{

}

PluginManager &PluginManager::instance()
{
    static PluginManager me;
    return me;
}
