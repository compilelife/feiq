#include "feiqwin.h"
#include "mainwindow.h"
#include "plugin/iplugin.h"

#include "plugin/unreadchecker.h"

FeiqWin::FeiqWin()
{

}

RecvTextEdit *FeiqWin::recvTextEdit()
{
    return mMainWin->mRecvTextEdit;
}

SendTextEdit *FeiqWin::sendTextEdit()
{
    return mMainWin->mSendTextEdit;
}

FellowListWidget *FeiqWin::fellowListWidget()
{
    return &(mMainWin->mFellowList);
}

const FeiqModel *FeiqWin::feiqModel()
{
    return &(mMainWin->mFeiq.getModel());
}

Settings *FeiqWin::settings()
{
    return mMainWin->mSettings;
}

int FeiqWin::getUnreadCount()
{
    return mMainWin->getUnreadCount();
}

void FeiqWin::init(MainWindow *mainWin)
{
    mMainWin = mainWin;

    loadPlugins();

    for (auto plugin : mPlugins)
    {
        plugin->setFeiqWin(this);
        plugin->init();
    }
}

void FeiqWin::unInit()
{
    for (auto plugin : mPlugins)
    {
        plugin->unInit();
    }
    mPlugins.clear();
}

void FeiqWin::loadPlugins()
{
    auto gAllPlugins = PluginManager::instance().allPlugins;
    cout<<"listing all supported plugins:"<<endl;
    for (auto iter : gAllPlugins)
    {
        cout<<iter.first;
        if (mMainWin->mSettings->value(QString(iter.first)+"/enable", "1").toBool())
        {
            cout<<"    enable";
            mPlugins.append(iter.second);
        }
        else
        {
            cout<<"    disable";
        }
        cout<<endl;
    }
}
