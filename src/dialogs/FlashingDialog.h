#ifndef FLASHINGDIALOG_H
#define FLASHINGDIALOG_H

#include <QDialog>

class BundleBuild;

namespace Ui {
class FlashingDialog;
}

class FlashingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FlashingDialog(BundleBuild* build, QWidget *parent = 0);
    ~FlashingDialog();

    void timerEvent(QTimerEvent *);

private slots:


private:
    Ui::FlashingDialog *ui;
    BundleBuild* mBuildToFlash;
    int mWarmupId;
};

#endif // FLASHINGDIALOG_H
