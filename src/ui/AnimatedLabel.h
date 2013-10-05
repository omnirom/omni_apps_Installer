#ifndef ANIM_H
#define ANIM_H
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QLabel>
#include <QList>
#include <QPixmap>

class AnimatedLabel : public QLabel
{
Q_OBJECT

public:
    AnimatedLabel(QStringList images, const int time, QWidget *parent = 0);

private slots:
    void changeImage();

private:
    QList<QPixmap> mPixmaps;
    int mCurrentPixmap;
    QTimer mTimer;
};

#endif // ANIM_H
