#include "sendtextedit.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDebug>

SendTextEdit::SendTextEdit(QWidget *parent)
    :QTextEdit(parent)
{
    setAcceptDrops(true);
    installEventFilter(this);
}

void SendTextEdit::newLine()
{
    append("");
}

void SendTextEdit::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        auto urls = e->mimeData()->urls();
        for (auto url : urls)
        {
            if (QFileInfo(url.toLocalFile()).isFile())
            {
                e->accept();
                return;
            }
        }
    }
    else
    {
        QTextEdit::dragEnterEvent(e);
    }
}

void SendTextEdit::dropEvent(QDropEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        auto urls = e->mimeData()->urls();
        QList<QFileInfo> files;
        for (auto url : urls)
        {
            files.append(QFileInfo(url.toLocalFile()));
            e->accept();
        }
        emit acceptDropFiles(files);
    }   
    else
    {
        QTextEdit::dropEvent(e);
    }
}

bool SendTextEdit::eventFilter(QObject *, QEvent * e)
{
    if (e->type() == QEvent::KeyPress)
    {
        auto keyEvent = static_cast<QKeyEvent*>(e);
        auto enter = keyEvent->key() == Qt::Key_Return;
        auto ctrl  = keyEvent->modifiers() == Qt::ControlModifier;
        if (enter && ctrl)
        {
            emit ctrlEnterPressed();
            return true;
        }
        else if (enter)
        {
            emit enterPressed();
            return true;
        }
    }
    return false;
}
