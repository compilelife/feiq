#ifndef PLATFORMDEPEND_H
#define PLATFORMDEPEND_H

#include <QString>
class IPlatform
{
public:
    virtual ~IPlatform(){}

    virtual void showNotify(const QString& title, const QString& content) = 0;
    virtual void hideAllNotify() = 0;

    virtual void setBadgeNumber(int number) = 0;
};

class PlatformDepend : public IPlatform
{
private:
    PlatformDepend();
    ~PlatformDepend();

public:
    static PlatformDepend& instance();

public:
    void showNotify(const QString& title, const QString& content) override;
    void hideAllNotify() override;

    void setBadgeNumber(int number) override;

private:
    IPlatform* mImpl;
};

#endif // PLATFORMDEPEND_H
