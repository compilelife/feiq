#include "sendtextedit.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>

SendTextEdit::SendTextEdit(QWidget *parent)
    :QTextEdit(parent)
{
    setAcceptDrops(true);
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
