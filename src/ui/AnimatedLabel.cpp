#include "AnimatedLabel.h"
#include <QApplication>
#include <QImage>

AnimatedLabel::AnimatedLabel(QStringList images, const int time, QWidget *parent)
    :     QLabel(parent), mCurrentPixmap(0)
{
    // Load images
    for (QStringList::iterator it = images.begin(); it != images.end(); ++it)
    {
        QImage img;
        img.load(*it);
        mPixmaps.push_back(QPixmap::fromImage(img));
    }

    // Start timer
    connect(&mTimer, SIGNAL(timeout()), SLOT(changeImage()));
    mTimer.start(time);
    changeImage();
}

void AnimatedLabel::changeImage()
{
    if (mCurrentPixmap >= mPixmaps.length())
    {
        mCurrentPixmap = 0;
    }

    setPixmap(mPixmaps.at(mCurrentPixmap));
    mCurrentPixmap++;
}

