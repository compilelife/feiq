#ifndef PLATFORMDEPEND_H
#define PLATFORMDEPEND_H

#include <QString>
class MainWindow;

class IPlatform
{
public:
    virtual ~IPlatform(){}

    virtual long showNotify(const QString& title, const QString& content, const QString & fellowIp) = 0;
    virtual void hideAllNotify() = 0;

    virtual void setBadgeNumber(int number) = 0;

    virtual void setMainWnd(MainWindow* mainWnd)
    {
        Q_UNUSED(mainWnd);
    }
};

class PlatformDepend : public IPlatform
{
private:
    PlatformDepend();
    ~PlatformDepend();

public:
    static PlatformDepend& instance();

public:
    long showNotify(const QString& title, const QString& content, const QString & fellowIp) override;
    void hideAllNotify() override;

    void setBadgeNumber(int number) override;

    void setMainWnd(MainWindow* mainWnd) override;
private:
    IPlatform* mImpl;
};

#endif // PLATFORMDEPEND_H
