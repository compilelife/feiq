#include "chooseemojidlg.h"
#include "ui_chooseemojidlg.h"
#include <QMouseEvent>
#include <QMovie>
#include "emoji.h"

ChooseEmojiDlg::ChooseEmojiDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseEmojiDlg)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Tool);
    setFixedSize(size());

    mMovie = new QMovie(this);
    ui->gifLabel->setMovie(mMovie);

    connect(ui->chooseWidget, SIGNAL(choose(int)), this, SLOT(choose(int)));
    connect(ui->chooseWidget, SIGNAL(choose(int)), this, SLOT(hide()));
    connect(ui->chooseWidget, SIGNAL(hesitate(int)), this, SLOT(hesitate(int)));
}

ChooseEmojiDlg::~ChooseEmojiDlg()
{
    delete ui;
}

void ChooseEmojiDlg::hideEvent(QHideEvent *event)
{
    mMovie->stop();
}

void ChooseEmojiDlg::hesitate(int index)
{
    if (index == -1)
    {
        ui->hintLabel->setText("");
        mMovie->stop();
        ui->gifLabel->setText("");
    }
    else
    {
        ui->hintLabel->setText(QString(g_emojiText[index])+"    "+QString(g_emojis[index]));

        auto gif = ":/default/res/face/"+QString::number(index+1)+".gif";

        mMovie->stop();
        mMovie->setFileName(gif);
        mMovie->start();
    }
}

void ChooseEmojiDlg::choose(int index)
{
    emit choose(g_emojis[index]);
}
