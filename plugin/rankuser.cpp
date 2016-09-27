#include "rankuser.h"

#define PLUGIN_NAME "rank_user"
REGISTER_PLUGIN(PLUGIN_NAME, RankUser)

RankUser::RankUser()
{
}

void RankUser::init()
{
    IPlugin::init();
    connect(mFeiq->fellowListWidget(), SIGNAL(select(const Fellow*)),  this, SLOT(onTalkTo(const Fellow*)));
    mFeiq->fellowListWidget()->setRankPredict(std::bind(&RankUser::compare, this, placeholders::_1, placeholders::_2));
}

void RankUser::unInit()
{
    IPlugin::unInit();
    mFeiq->settings()->sync();
}

void RankUser::onTalkTo(const Fellow *fellow)
{
    mFeiq->settings()->setValue(fellowKey(*fellow), weightOfFellow(*fellow)+1);
}

int RankUser::compare(const Fellow &f1, const Fellow &f2)
{
    return weightOfFellow(f1)-weightOfFellow(f2);
}

QString RankUser::fellowId(const Fellow &f)
{
    QString ip(f.getIp().c_str());
    return ip;
}

QString RankUser::fellowKey(const Fellow &f)
{
    return QString(PLUGIN_NAME)+"/"+fellowId(f);
}

int RankUser::weightOfFellow(const Fellow &f)
{
    return mFeiq->settings()->value(fellowKey(f), "0").toInt();
}
