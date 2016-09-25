#ifndef CHOOSEEMOJIDLG_H
#define CHOOSEEMOJIDLG_H

#include <QDialog>

namespace Ui {
class ChooseEmojiDlg;
}

class ChooseEmojiDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseEmojiDlg(QWidget *parent = 0);
    ~ChooseEmojiDlg();
protected:
    void hideEvent(QHideEvent *event);
signals:
    void choose(const QString& emojiText);

private slots:
    void hesitate(int index);
    void choose(int index);

private:
    Ui::ChooseEmojiDlg *ui;
    QMovie* mMovie;
};

#endif // CHOOSEEMOJIDLG_H
