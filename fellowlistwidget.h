#ifndef FELLOWLISTWIDGET_H
#define FELLOWLISTWIDGET_H

#include "feiqlib/feiqmodel.h"
#include <QListWidget>

//1.按“会话中”，“有新消息”、“在线”、“离线”优先级罗列好友信息
//2.支持查询好友
class FellowListWidget : public QObject
{
    Q_OBJECT

public:
    FellowListWidget();
    void bindTo(QListWidget* widget);

public:
    void update(const Fellow& fellow);
    void top(const Fellow& fellow);
    void topSecond(const Fellow& fellow);
    void mark(const Fellow& fellow, const QString &info);

signals:
    void select(const Fellow* fellow);

private slots:
    void itemChosen(QListWidgetItem *item);

private:
    QString fellowText(const Fellow& fellow);
    QListWidgetItem* findFirstItem(const Fellow& fellow);

private:
    QListWidget* mWidget;
};

#endif // FELLOWLISTWIDGET_H
