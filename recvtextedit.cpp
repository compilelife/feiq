#include "recvtextedit.h"
#include <QDate>
#include "emoji.h"
#include <QMouseEvent>

RecvTextEdit::RecvTextEdit(QWidget *parent)
    :QTextEdit(parent)
{
    setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
}

void RecvTextEdit::mousePressEvent(QMouseEvent *e)
{
    mPressedAnchor =  (e->button() & Qt::LeftButton) ? anchorAt(e->pos()) : "";
    QTextEdit::mousePressEvent(e);
}

void RecvTextEdit::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() & Qt::LeftButton)
    {
        if (anchorAt(e->pos()) == mPressedAnchor && !mPressedAnchor.isEmpty())
            parseLink(mPressedAnchor);
    }
    QTextEdit::mouseReleaseEvent(e);
}

void RecvTextEdit::addFellowContent(const Content *content, long long msSinceEpoch)
{
    drawDaySeperatorIfNewDay(msSinceEpoch);

    QString hint = "";
    if (mFellow)
        hint = mFellow->getName().c_str();
    else
        hint = "匿名";
    hint = hint+" <font color=gray>"+ timeStr(msSinceEpoch)+"</font>";
    moveCursor(QTextCursor::End);
    insertHtml(hint);
    append("");
    showContent(content, false);
    append("\n");
    moveCursor(QTextCursor::End);
}

void RecvTextEdit::addMyContent(const Content *content, long long msSinceEpoch)
{
    drawDaySeperatorIfNewDay(msSinceEpoch);

    QString hint = "我 <font color=gray>"+timeStr(msSinceEpoch)+"</font>";
    moveCursor(QTextCursor::End);
    insertHtml(hint);
    append("");
    showContent(content, true);
    append("\n");
    moveCursor(QTextCursor::End);
}

void RecvTextEdit::setCurFellow(const Fellow *fellow)
{
    if (mFellow)
        mDocs[mFellow] = document()->clone();//document将被清除或删除了，需clone

    auto it = mDocs.find(fellow);
    if (it != mDocs.end())
    {
        setDocument((*it).second);
        moveCursor(QTextCursor::End);
    }
    else
    {
        clear();
    }

    mFellow = fellow;
}

void RecvTextEdit::addWarning(const QString &warning)
{
    auto align = alignment();
    setAlignment(Qt::AlignCenter);
    auto color = textColor();
    setTextColor(QColor(128,128,128));
    append(warning);
    append("");//结束当前段落，否则下一行恢复对齐方式时会将刚append的内容左对齐
    setAlignment(align);
    setTextColor(color);
}

const Fellow *RecvTextEdit::curFellow()
{
    return mFellow;
}

void RecvTextEdit::parseLink(const QString &link)
{
    QStringList parts = link.split("_");
    if (parts.count()<3)
        return;

    auto packetNo = parts.at(0).toLongLong();
    auto fileId = parts.at(1).toLongLong();
    bool upload = parts.at(2) == "up";

    emit navigateToFileTask(packetNo, fileId, upload);
}

QString RecvTextEdit::timeStr(long long msSinceEpoch)
{
    QDateTime time;
    time.setMSecsSinceEpoch(msSinceEpoch);
    return time.toString("MM-dd HH:mm:ss");
}

void RecvTextEdit::showContent(const Content *content, bool mySelf)
{
    switch (content->type())
    {
    case ContentType::File:
        showFile(static_cast<const FileContent*>(content), mySelf);
        break;
    case ContentType::Knock:
        showKnock(static_cast<const KnockContent*>(content), mySelf);
        break;
    case ContentType::Image:
        showImage(static_cast<const ImageContent*>(content));
        break;
    case ContentType::Text:
        showText(static_cast<const TextContent*>(content));
        break;
    default:
        showUnSupport();
        break;
    }
}

void RecvTextEdit::showFile(const FileContent *content, bool fromMySelf)
{
    if (content->fileType == IPMSG_FILE_REGULAR)
    {
        stringstream ss;
        ss<<"<a href="<<content->packetNo<<"_"<<content->fileId<<"_"<<(fromMySelf?"up":"down")<<">"
         <<content->filename<<"("<<content->size<<")"
        <<"</a>";
        insertHtml(ss.str().c_str());
    }
    else
    {
        showUnSupport("对方发来非普通文件（可能是文件夹），收不来……");
    }
}

void RecvTextEdit::showImage(const ImageContent *content)
{
    showUnSupport("对方发来图片，来图片，图片，片……额~还不支持!");
}

void RecvTextEdit::showText(const TextContent *content)
{
    insertHtml(textHtmlStr(content));
}

void RecvTextEdit::showKnock(const KnockContent *content, bool mySelf)
{
    if (mySelf)
        insertHtml("[发送了一个窗口抖动]");
    else
        insertHtml("[发来窗口抖动]");
}

void RecvTextEdit::showUnSupport(const QString& text)
{
    QString t = text;
    if (t.isEmpty())
        t = "对方发来尚未支持的内容，无法显示";

    insertHtml("<font color=\"red\">"+t+"</font>");
}

void RecvTextEdit::drawDaySeperatorIfNewDay(long long sinceEpoch)
{
    QDateTime cur;
    cur.setMSecsSinceEpoch(sinceEpoch);

    if (mLastEdit > 0)
    {
        QDateTime last;
        last.setMSecsSinceEpoch(mLastEdit);

        if (last.daysTo(cur)>0)
        {
            addWarning("-----------------------------");
        }
    }

    mLastEdit = sinceEpoch;
}

QString RecvTextEdit::textHtmlStr(const TextContent *content)
{
    auto str = QString(content->text.c_str());
    auto htmlStr = str.toHtmlEscaped();
    htmlStr.replace("\r\n", "<br>");
    htmlStr.replace("\r", "<br>");
    htmlStr.replace("\n", "<br>");

    for (auto i = 0; i < EMOJI_LEN; i++)
    {
         auto resName = QString(":/default/res/face/")+QString::number(i+1)+".gif";
         auto emojiStr = g_emojis[i];
         QString imgTag = "<img src=\""+resName+"\"/>";
         htmlStr.replace(emojiStr, imgTag);
    }

    return htmlStr;
}
