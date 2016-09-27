#ifndef RANKUSER_H
#define RANKUSER_H

#include "iplugin.h"
#include <QObject>
#include "feiqlib/fellow.h"

class RankUser : public QObject, public IPlugin
{
    Q_OBJECT

public:
    RankUser();

    void init() override;
    void unInit() override;

private slots:
    void onTalkTo(const Fellow* fellow);

private:
    int compare(const Fellow& f1, const Fellow& f2);
    QString fellowId(const Fellow& f);
    QString fellowKey(const Fellow& f);
    int weightOfFellow(const Fellow& f);
};

#endif // RANKUSER_H
