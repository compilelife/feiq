#include "settings.h"

Settings::Settings(const QString &fileName, QSettings::Format format, QObject *parent)
    :QSettings(fileName, format, parent)
{

}

QVariant Settings::value(const QString &key, const QVariant &defaultValue, bool cacheDefault)
{
    //如果配置中没有该项，则以默认值创建，方便用户知道有那些配置项可用
    if (!contains(key) && cacheDefault)
    {
        if (!defaultValue.isValid() || defaultValue.isNull())
            setValue(key, "");//防止非法值
        else
            setValue(key, defaultValue);
        sync();
    }

    return QSettings::value(key, defaultValue);
}
