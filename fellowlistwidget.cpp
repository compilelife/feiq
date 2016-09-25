#include "fellowlistwidget.h"
#include <QDebug>

FellowListWidget::FellowListWidget()
{

}

void FellowListWidget::bindTo(QListWidget *widget)
{
    mWidget = widget;
    connect(mWidget, &QListWidget::itemClicked, this, &FellowListWidget::itemChosen);
}

void FellowListWidget::update(const Fellow &fellow)
{
    auto item = findFirstItem(fellow);
    if (item == nullptr)
    {
        mWidget->addItem(fellowText(fellow));
        item = mWidget->item(mWidget->count()-1);
    }
    else
    {
        item->setText(fellowText(fellow));
    }

    item->setData(Qt::UserRole, QVariant::fromValue((void*)&fellow));
}

void FellowListWidget::top(const Fellow &fellow)
{
    auto item = findFirstItem(fellow);
    if (item != nullptr)
    {
        mWidget->takeItem(mWidget->row(item));
        mWidget->insertItem(0, item);
        mWidget->scrollToItem(item);
        mWidget->setCurrentItem(item);
    }
}

//TODO:take->insert的方式或导致如果item是当前焦点，则移动后焦点丢失
void FellowListWidget::topSecond(const Fellow &fellow)
{
    auto item = findFirstItem(fellow);
    if (item != nullptr)
    {
        mWidget->takeItem(mWidget->row(item));
        mWidget->insertItem(1, item);
    }
}

void FellowListWidget::mark(const Fellow &fellow, const QString &info)
{
    auto item = findFirstItem(fellow);
    if (item != nullptr)
    {
        if (info.isEmpty())
            item->setText(fellowText(fellow));
        else
        {
            item->setText("("+info+")"+fellowText(fellow));
            if (mWidget->row(item)!=0)
            {
                if (mWidget->currentRow() == 0)
                    topSecond(fellow);
                else
                    top(fellow);
            }
        }
    }
}

void FellowListWidget::itemChosen(QListWidgetItem *item)
{
    if (item == nullptr)
        return;

    auto fellow = static_cast<const Fellow*>(item->data(Qt::UserRole).value<void*>());
    emit select(fellow);
}

QString FellowListWidget::fellowText(const Fellow &fellow)
{
    auto text = fellow.getName()+","+fellow.getIp();
    if (!fellow.isOnLine())
    {
        text = "[离线]"+text;
    }
    return QString(text.c_str());
}

QListWidgetItem *FellowListWidget::findFirstItem(const Fellow &fellow)
{
    auto count = mWidget->count();
    for (int i = 0; i < count; i++)
    {
        auto widget = mWidget->item(i);
        auto f = static_cast<const Fellow*>(widget->data(Qt::UserRole).value<void*>());
        if (f->getIp() == fellow.getIp())
            return widget;
    }

    return nullptr;
}
