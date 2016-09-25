#include "chooseemojiwidget.h"
#include <QMouseEvent>
#include "emoji.h"

ChooseEmojiWidget::ChooseEmojiWidget(QWidget* parent)
    :QLabel(parent), mIndex(-1)
{
    setMouseTracking(true);
}

void ChooseEmojiWidget::mousePressEvent(QMouseEvent *event)
{
    mIndex = getIndex(event->x(), event->y());
}

void ChooseEmojiWidget::mouseMoveEvent(QMouseEvent *event)
{
    auto index = getIndex(event->x(), event->y());
    if (index >= 0  && index < EMOJI_LEN)
    {
        emit hesitate(index);
    }
    else
    {
        emit hesitate(-1);
    }
}

void ChooseEmojiWidget::mouseReleaseEvent(QMouseEvent *event)
{
    auto index = getIndex(event->x(), event->y());
    if (mIndex == index && mIndex >= 0 && mIndex < EMOJI_LEN)
    {
        emit choose(mIndex);
    }

    mIndex = -1;
}

int ChooseEmojiWidget::getIndex(int x, int y)
{
    int col = x/25;
    int row = y/25;
    auto index = row * EMOJI_BMP_COL + col;
    if (index >= 0 && index < EMOJI_LEN)
        return index;

    return -1;
}
