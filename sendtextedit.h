#ifndef SENDTEXTEDIT_H
#define SENDTEXTEDIT_H

#include <QTextEdit>
#include <QList>
#include <QFileInfo>

class SendTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    SendTextEdit(QWidget* parent = 0);

signals:
    void acceptDropFiles(QList<QFileInfo>);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
};

#endif // SENDTEXTEDIT_H
