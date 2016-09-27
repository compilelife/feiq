#ifndef FEIQWIN_H
#define FEIQWIN_H

#include "recvtextedit.h"
#include "sendtextedit.h"
#include "qsettings.h"
#include "fellowlistwidget.h"
#include "feiqlib/feiqmodel.h"
#include "settings.h"

class MainWindow;
class IPlugin;

/**
 * @brief The FeiqWin class
 * 其实就是MainWindow的代言人，用来隔离插件和MainWindow，方便控制MainWindow对插件的可见性
 */
class FeiqWin
{
public:
    FeiqWin();

public:
    RecvTextEdit* recvTextEdit();
    SendTextEdit* sendTextEdit();
    FellowListWidget* fellowListWidget();
    const FeiqModel* feiqModel();
    Settings* settings();

public:
    int getUnreadCount();

private:
    void init(MainWindow* mainWin);
    void unInit();
    void loadPlugins();

private:
    friend class MainWindow;
    MainWindow* mMainWin;
    QList<IPlugin*> mPlugins;
};

#endif // FEIQWIN_H
