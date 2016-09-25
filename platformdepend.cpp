#include "platformdepend.h"

#ifdef Q_OS_OSX
#include "osx/osxplatform.h"
#endif

class MockPlatform : public IPlatform
{
public:
    void showNotify(const QString& title, const QString& content)
    {
        (void)title;
        (void)content;
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

void PlatformDepend::showNotify(const QString &title, const QString &content)
{
    mImpl->showNotify(title, content);
}

void PlatformDepend::hideAllNotify()
{
    mImpl->hideAllNotify();
}

void PlatformDepend::setBadgeNumber(int number)
{
    mImpl->setBadgeNumber(number);
}
