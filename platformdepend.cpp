#include "platformdepend.h"

#ifdef Q_OS_OSX
#include "osx/osxplatform.h"
#endif

class MockPlatform : public IPlatform
{
public:
    long showNotify(const QString& title, const QString& content, const QString& data)
    {
        (void)title;
        (void)content;
        (void)data;
        return 0;
    }
    void hideAllNotify()
    {

    }

    void setBadgeNumber(int number)
    {
        (void)number;
    }
};

PlatformDepend::PlatformDepend()
{
#ifdef Q_OS_OSX
    mImpl = new OsxPlatform();
#else
    mImpl = new MockPlatform();
#endif
}

PlatformDepend::~PlatformDepend()
{
    delete mImpl;
}

PlatformDepend &PlatformDepend::instance()
{
    static PlatformDepend me;
    return me;
}

long PlatformDepend::showNotify(const QString &title, const QString &content, const QString &fellowIp)
{
    return mImpl->showNotify(title, content, fellowIp);
}

void PlatformDepend::hideAllNotify()
{
    mImpl->hideAllNotify();
}

void PlatformDepend::setBadgeNumber(int number)
{
    mImpl->setBadgeNumber(number);
}

void PlatformDepend::setMainWnd(MainWindow *mainWnd)
{
    IPlatform::setMainWnd(mainWnd);
    mImpl->setMainWnd(mainWnd);
}
