#ifndef CHOOSEEMOJIWIDGET_H
#define CHOOSEEMOJIWIDGET_H

#include <QLabel>

class ChooseEmojiWidget : public QLabel
{
    Q_OBJECT
public:
    ChooseEmojiWidget(QWidget* parent=0);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    int getIndex(int x, int y);
signals:
    void choose(int emojiIndex);
    void hesitate(int emojiIndex);
private:
    int mIndex;
};

#endif // CHOOSEEMOJIWIDGET_H
