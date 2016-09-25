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

QSettings *FeiqWin::settings()
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
        plugin->init(this);
}

void FeiqWin::unInit()
{
    for (auto plugin : mPlugins)
    {
        plugin->unInit();
        delete plugin;
    }
    mPlugins.clear();
}

void FeiqWin::loadPlugins()
{
    mPlugins.append(new UnreadChecker());
}
