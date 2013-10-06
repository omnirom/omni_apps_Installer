#ifndef FLASHINGDIALOG_H
#define FLASHINGDIALOG_H

#include <QDialog>
#include <QNetworkReply>

class BundleBuild;
class FlashScenario;

namespace Ui {
class FlashingDialog;
}

class FlashingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FlashingDialog(BundleBuild* build, QWidget *parent = 0);
    ~FlashingDialog();

private slots:
    void onBuildDownloaded(QNetworkReply*);
    void onRecoveryDownloaded(QNetworkReply*);

private:
    Ui::FlashingDialog *ui;
    BundleBuild* mBuildToFlash;
    FlashScenario* mScenario;
};

#endif // FLASHINGDIALOG_H
