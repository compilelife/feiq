#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QSettings
{
public:
    Settings(const QString &fileName, Format format, QObject *parent = Q_NULLPTR);
public:
    //QSettings::value不是虚函数，这里并非多态重载，使用时需注意以Settings指针调用，而非QSettings调用
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant(), bool cacheDefault=true);

};

#endif // SETTINGS_H
