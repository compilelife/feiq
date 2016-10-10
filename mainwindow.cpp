#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <QDateTime>
#include <QtMac>
#include "addfellowdialog.h"
#include <QProcess>
#include "platformdepend.h"
#include "feiqwin.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<shared_ptr<ViewEvent>>("ViewEventSharedPtr");

    connect(this, SIGNAL(showErrorAndQuit(QString)), this, SLOT(onShowErrorAndQuit(QString)));

    //加载配置
    auto settingFilePath = QDir::home().filePath(".feiq_setting.ini");
    mSettings = new Settings(settingFilePath, QSettings::IniFormat);
    mSettings->setIniCodec(QTextCodec::codecForName("UTF-8"));
    mTitle = mSettings->value("app/title", "mac飞秋").toString();
    setWindowTitle(mTitle);

    //初始化搜索对话框
    mSearchFellowDlg = new SearchFellowDlg(this);
    connect(mSearchFellowDlg, SIGNAL(onFellowSelected(const Fellow*)),
            this, SLOT(finishSearch(const Fellow*)));

    connect(ui->actionRefreshFellows, SIGNAL(triggered(bool)), this, SLOT(refreshFellowList()));
    connect(ui->actionAddFellow, SIGNAL(triggered(bool)), this, SLOT(addFellow()));

    mSearchFellowDlg->setSearchDriver(std::bind(&MainWindow::fellowSearchDriver, this, placeholders::_1));

    //初始化文件管理对话框
    mDownloadFileDlg = new FileManagerDlg(this);
    mDownloadFileDlg->setEngine(&mFeiq);
    connect(this, SIGNAL(statChanged(FileTask*)), mDownloadFileDlg, SLOT(statChanged(FileTask*)));
    connect(this, SIGNAL(progressChanged(FileTask*)), mDownloadFileDlg, SLOT(progressChanged(FileTask*)));

    //初始化好友列表
    mFellowList.bindTo(ui->fellowListWidget);
    connect(&mFellowList, SIGNAL(select(const Fellow*)), this, SLOT(openChartTo(const Fellow*)));

    //初始化接收文本框
    mRecvTextEdit = ui->recvEdit;
    connect(mRecvTextEdit, SIGNAL(navigateToFileTask(IdType,IdType,bool)), this, SLOT(navigateToFileTask(IdType,IdType,bool)));

    //初始化发送文本框
    mSendTextEdit = ui->sendEdit;
    connect(mSendTextEdit, SIGNAL(acceptDropFiles(QList<QFileInfo>)), this, SLOT(sendFiles(QList<QFileInfo>)));
    if (mSettings->value("app/send_by_enter", true).toBool())
    {
        connect(mSendTextEdit, SIGNAL(enterPressed()), this, SLOT(sendText()));
        connect(mSendTextEdit, SIGNAL(ctrlEnterPressed()), mSendTextEdit, SLOT(newLine()));
    }
    else
    {
        connect(mSendTextEdit, SIGNAL(ctrlEnterPressed()), this, SLOT(sendText()));
        connect(mSendTextEdit, SIGNAL(enterPressed()), mSendTextEdit, SLOT(newLine()));
    }

    //初始化Emoji对话框
    mChooseEmojiDlg = new ChooseEmojiDlg(this);
    connect(ui->actionInsertEmoji, SIGNAL(triggered(bool)), this, SLOT(openChooseEmojiDlg()));
    connect(mChooseEmojiDlg, SIGNAL(choose(QString)),mSendTextEdit, SLOT(insertPlainText(QString)));

    //初始化菜单
    connect(ui->actionSearchFellow, SIGNAL(triggered(bool)), this, SLOT(openSearchDlg()));
    connect(ui->actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettings()));
    connect(ui->actionOpendl, SIGNAL(triggered(bool)), this, SLOT(openDownloadDlg()));
    connect(ui->actionSendText, SIGNAL(triggered(bool)), this, SLOT(sendText()));
    connect(ui->actionSendKnock, SIGNAL(triggered(bool)), this, SLOT(sendKnock()));
    connect(ui->actionSendFile, SIGNAL(triggered(bool)), this, SLOT(sendFile()));

    //初始化飞秋引擎
    connect(this, SIGNAL(feiqViewEvent(shared_ptr<ViewEvent>)), this, SLOT(handleFeiqViewEvent(shared_ptr<ViewEvent>)));

    //后台初始化通信
    std::thread thd(&MainWindow::initFeiq, this);
    thd.detach();
}

MainWindow::~MainWindow()
{
    mFeiq.stop();
    mSettings->sync();
    delete mSettings;
    delete mSearchFellowDlg;
    delete mDownloadFileDlg;
    delete mChooseEmojiDlg;
    delete ui;
}

void MainWindow::setFeiqWin(FeiqWin *feiqWin)
{
    mFeiqWin = feiqWin;
    mFeiqWin->init(this);
}

void MainWindow::enterEvent(QEvent *event)
{
    auto fellow = mRecvTextEdit->curFellow();
    if (fellow)
    {
        flushUnread(fellow);
        updateUnread(fellow);
    }

    PlatformDepend::instance().hideAllNotify();
}

void MainWindow::openChartTo(const Fellow *fellow)
{
    mFellowList.top(*fellow);
    mRecvTextEdit->setCurFellow(fellow);
    setWindowTitle(mTitle + " - 与"+fellow->getName().c_str()+"会话中");
    flushUnread(fellow);
    updateUnread(fellow);
}

shared_ptr<Fellow> MainWindow::checkCurFellow()
{
    auto fellow = mFeiq.getModel().getShared(mRecvTextEdit->curFellow());
    if (fellow == nullptr)
    {
        mRecvTextEdit->addWarning("这是要发给谁？");
    }

    return fellow;
}

void MainWindow::showResult(pair<bool, string> ret, const Content* content)
{
    if (ret.first)
        mRecvTextEdit->addMyContent(content, QDateTime::currentDateTime().currentMSecsSinceEpoch());
    else
        mRecvTextEdit->addWarning(ret.second.c_str());
}

void MainWindow::onStateChanged(FileTask *fileTask)
{
    if (fileTask->getState()==FileTaskState::Finish)
    {
        auto title = QString(fileTask->getTaskTypeDes().c_str())+"完成";
        PlatformDepend::instance().showNotify(title, fileTask->getContent()->filename.c_str());
    }
    else if (fileTask->getState()==FileTaskState::Error)
    {
        auto title = QString(fileTask->getTaskTypeDes().c_str())+"失败";
        auto content = QString(fileTask->getContent()->filename.c_str());
        content += "\n";
        content += fileTask->getDetailInfo().c_str();
        PlatformDepend::instance().showNotify(title, content);
    }

    if (mDownloadFileDlg->isVisible())
    {
        emit statChanged(fileTask);
    }
}

void MainWindow::onProgress(FileTask *fileTask)
{
    if (mDownloadFileDlg->isVisible())
    {
        emit progressChanged(fileTask);
    }
}

void MainWindow::onEvent(shared_ptr<ViewEvent> event)
{
    emit feiqViewEvent(event);
}

void MainWindow::onShowErrorAndQuit(const QString &text)
{
    QMessageBox::warning(this, "出错了，为什么？你猜！", text, "退出应用");

    QApplication::exit(-1);
}

void MainWindow::handleFeiqViewEvent(shared_ptr<ViewEvent> event)
{
    if (event->what == ViewEventType::FELLOW_UPDATE)
    {
        auto e = static_cast<FellowViewEvent*>(event.get());
        mFellowList.update(*(e->fellow.get()));
    }
    else if (event->what == ViewEventType::SEND_TIMEO || event->what == ViewEventType::MESSAGE)
    {
        //地球人都知道这个分支中的ViewEvent继承自FellowViewEvent
        auto e = static_cast<FellowViewEvent*>(event.get());
        auto fellow = e->fellow.get();

        if (isActiveWindow())
        {//窗口可见，处理当前用户消息，其他用户消息则放入通知队列
            if (fellow == mRecvTextEdit->curFellow())
            {
                readEvent(event.get());
            }
            else
            {
                mUnreadEvents[fellow].push_back(event);
                updateUnread(fellow);
            }
        }
        else
        {//窗口不可见，放入未读队列并通知
            mUnreadEvents[fellow].push_back(event);
            updateUnread(fellow);
            notifyUnread(event.get());
        }
    }
}

void MainWindow::refreshFellowList()
{
    mFeiq.sendImOnLine();
}

void MainWindow::addFellow()
{
    AddFellowDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        auto ip = dlg.getIp();
        userAddFellow(ip);
    }
}

void MainWindow::openChooseEmojiDlg()
{
    mChooseEmojiDlg->exec();
}

void MainWindow::sendFiles(QList<QFileInfo> files)
{
    auto fellow = checkCurFellow();
    if (!fellow)
        return;

    for (auto file : files)
    {
        if (file.isFile())
        {
            sendFile(file.absoluteFilePath().toStdString());
        }
        else
        {
            mRecvTextEdit->addWarning("不支持发送："+file.absoluteFilePath());
        }
    }
}

void MainWindow::userAddFellow(QString ip)
{
    //创建好友
    auto fellow = make_shared<Fellow>();
    fellow->setIp(ip.toStdString());
    fellow->setOnLine(true);
    mFeiq.getModel().addFellow(fellow);

    //添加到列表
    auto& ref = *(fellow.get());
    mFellowList.update(ref);
    mFellowList.top(ref);

    //发送在线
    mFeiq.sendImOnLine(fellow->getIp());
}

void MainWindow::notifyUnread(const ViewEvent *event)
{
    if (event->what == ViewEventType::SEND_TIMEO)
    {
        auto e = static_cast<const SendTimeoEvent*>(event);
        auto fellow = e->fellow.get();
        showNotification(fellow, "发送超时:"+simpleTextOf(e->content.get()));
    }
    else if (event->what == ViewEventType::MESSAGE)
    {
        auto e = static_cast<const MessageViewEvent*>(event);
        auto fellow = e->fellow.get();
        for (auto content : e->contents)
        {
            showNotification(fellow, simpleTextOf(content.get()));
        }
    }
}

void MainWindow::showNotification(const Fellow *fellow, const QString &text)
{
    QString content(text);
    if (content.length()>20)
        content = content.left(20)+"...";
    PlatformDepend::instance().showNotify(QString(fellow->getName().c_str())+":", content);
}

void MainWindow::navigateToFileTask(IdType packetNo, IdType fileId, bool upload)
{
    auto task = mFeiq.getModel().findTask(packetNo, fileId, upload ? FileTaskType::Upload : FileTaskType::Download);
    openDownloadDlg();
    mDownloadFileDlg->select(task.get());
}

void MainWindow::sendFile(std::string filepath)
{
    auto content = FileContent::createFileContentToSend(filepath);
    auto fellow = checkCurFellow();
    if (!fellow)
        return;

    if (content == nullptr)
    {
        mRecvTextEdit->addWarning("获取文件"+QString(filepath.c_str())+"的信息失败，不发送");
    }
    else
    {
        auto fileContent = shared_ptr<FileContent>(std::move(content));
        auto ret = mFeiq.send(fellow, fileContent);
        showResult(ret, fileContent.get());
    }
}

void MainWindow::sendFile()
{
    auto fellow = checkCurFellow();
    if (!fellow)
        return;

    //文件多选
    QFileDialog fdlg(this, "选择要发送的文件");
    fdlg.setFileMode(QFileDialog::ExistingFiles);
    if (fdlg.exec() == QDialog::Accepted)
    {
        auto list = fdlg.selectedFiles();
        auto count = list.count();
        for (int i = 0; i < count; i++)
        {
            auto path = list.at(i);
            sendFile(path.toStdString());
        }
    }
}

void MainWindow::sendKnock()
{
    auto fellow = checkCurFellow();
    if (fellow)
    {
        auto content = make_shared<KnockContent>();
        auto ret = mFeiq.send(fellow, content);
        showResult(ret, content.get());
    }
}

void MainWindow::sendText()
{
    auto text = mSendTextEdit->toPlainText();
    if (text.isEmpty())
    {
        mRecvTextEdit->addWarning("发送空文本是不科学的，驳回");
        return;
    }


    auto fellow = checkCurFellow();
    if (fellow)
    {
        auto content = make_shared<TextContent>();
        content->text = text.toStdString();
        auto ret = mFeiq.send(fellow, content);
        showResult(ret, content.get());
        mSendTextEdit->clear();
    }
}

void MainWindow::finishSearch(const Fellow *fellow)
{
    mFellowList.top(*fellow);
    openChartTo(fellow);
}

void MainWindow::openSettings()
{
    QMessageBox::information(this, "设置", "设置文件在:"+mSettings->fileName()+"\n重启后生效",
                             QMessageBox::Ok);
}

void MainWindow::openSearchDlg()
{
    mSearchFellowDlg->exec();
}

void MainWindow::openDownloadDlg()
{
    mDownloadFileDlg->show();
    mDownloadFileDlg->raise();
}

vector<const Fellow *> MainWindow::fellowSearchDriver(const QString &text)
{
    auto fellows = mFeiq.getModel().searchFellow(text.toStdString());
    vector<const Fellow*> result;
    for (auto fellow : fellows)
    {
        result.push_back(fellow.get());
    }
    return result;
}

void MainWindow::initFeiq()
{
    //配置飞秋
    auto name = mSettings->value("user/name").toString();
    if (name.isEmpty())
    {
        emit showErrorAndQuit("请先打开【"+mSettings->fileName()+"】设置用户名(user/name)");
        return;
    }

    mFeiq.setMyName(name.toStdString());
    mFeiq.setMyHost(mSettings->value("user/host","feiq by cy").toString().toStdString());

    auto customGrroup = mSettings->value("network/custom_group", "").toString();
    if (!customGrroup.isEmpty())
    {
        auto list = customGrroup.split("|");
        for (int i = 0; i < list.size(); i++)
        {
            QString ipPrefix = list[i];
            if (ipPrefix.endsWith("."))
            {
                for (int j = 2; j < 254; j++)
                {
                    auto ip = ipPrefix+QString::number(j);
                    mFeiq.addToBroadcast(ip.toStdString());
                }
            }
        }
    }

    mFeiq.setView(this);

    mFeiq.enableIntervalDetect(60);

    //启动飞秋
    auto ret = mFeiq.start();
    if (!ret.first)
    {
        emit showErrorAndQuit(ret.second.c_str());
    }

    qDebug()<<"feiq started";
}

void MainWindow::updateUnread(const Fellow *fellow)
{
    auto it = mUnreadEvents.find(fellow);
    if (it != mUnreadEvents.end())
    {
        auto count = (*it).second.size();
        if (count == 0)
        {
            mFellowList.mark(*fellow, "");
        }
        else
        {
            mFellowList.mark(*fellow, QString::number(count));
        }
    }

    setBadgeNumber(getUnreadCount());
}

int MainWindow::getUnreadCount()
{
    auto begin = mUnreadEvents.begin();
    auto end = mUnreadEvents.end();
    auto count = 0;
    for (auto it = begin; it != end; it++)
    {
        count += it->second.size();
    }
    return count;
}

void MainWindow::flushUnread(const Fellow *fellow)
{
    auto it = mUnreadEvents.find(fellow);
    if (it != mUnreadEvents.end())
    {
        auto& list = (*it).second;
        while (!list.empty())
        {
            auto event = list.front();
            readEvent(event.get());
            list.pop_front();
        }
    }
}

void MainWindow::readEvent(const ViewEvent *event)
{
    if (event->what == ViewEventType::SEND_TIMEO)
    {
        auto e = static_cast<const SendTimeoEvent*>(event);
        auto simpleText = simpleTextOf(e->content.get());
        if (simpleText.length()>20){
            simpleText = simpleText.left(20)+"...";
        }
        mRecvTextEdit->addWarning("发送超时:"+simpleText);
    }
    else if (event->what == ViewEventType::MESSAGE)
    {
        auto e = static_cast<const MessageViewEvent*>(event);
        auto time =  e->when.time_since_epoch().count();
        for (auto content : e->contents)
        {
            mRecvTextEdit->addFellowContent(content.get(), time);
        }
    }
}

void MainWindow::setBadgeNumber(int number)
{
    PlatformDepend::instance().setBadgeNumber(number);
}

QString MainWindow::simpleTextOf(const Content *content)
{
    switch (content->type()) {
    case ContentType::Text:
        return static_cast<const TextContent*>(content)->text.c_str();
        break;
    case ContentType::File:
        return static_cast<const FileContent*>(content)->filename.c_str();
    case ContentType::Knock:
        return "窗口抖动";
    default:
        return "***";
        break;
    }
}
