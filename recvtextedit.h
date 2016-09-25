#ifndef RECVTEXTEDIT_H
#define RECVTEXTEDIT_H

#include "feiqlib/content.h"
#include "feiqlib/fellow.h"
#include <QObject>
#include <unordered_map>
#include <QTextEdit>

using namespace std;

class RecvTextEdit: public QTextEdit
{
    Q_OBJECT
public:
    RecvTextEdit(QWidget* parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;

public:
    void addFellowContent(const Content* content, long long msSinceEpoch);
    void addMyContent(const Content* content, long long msSinceEpoch);
    void setCurFellow(const Fellow* fellow);
    void addWarning(const QString& warning);
    const Fellow* curFellow();

signals:
    void navigateToFileTask(IdType packetNo, IdType fileId, bool upload);

private:
    QString timeStr(long long msSinceEpoch);
    void showContent(const Content* content, bool mySelf);
    void showFile(const FileContent* content, bool fromMySelf);
    void showImage(const ImageContent* content);
    void showText(const TextContent* content);
    void showKnock(const KnockContent* content, bool mySelf);
    void showUnSupport(const QString &text = "");
    void drawDaySeperatorIfNewDay(long long sinceEpoch);
    QString textHtmlStr(const TextContent* content);
    void parseLink(const QString& link);

private:
    const Fellow* mFellow = nullptr;
    unordered_map<const Fellow*, QTextDocument*> mDocs;
    long long mLastEdit=0;
    QString mPressedAnchor;

};

#endif // RECVTEXTEDIT_H
