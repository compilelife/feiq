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
    void ctrlEnterPressed();
    void enterPressed();

public slots:
    void newLine();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
    virtual bool eventFilter(QObject *, QEvent *e) override;

private:
    bool mCtrlDown =false;
};

#endif // SENDTEXTEDIT_H
