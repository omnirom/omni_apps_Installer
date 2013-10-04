#include "FlashingDialog.h"
#include "ui_flashingdialog.h"

#include "../BundleManager.h"
#include "../FlashScenario.h"

#include <QDebug>
#include <QMessageBox>

//----------------------------------------
FlashingDialog::FlashingDialog(BundleBuild* build, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlashingDialog), mBuildToFlash(build)
{
    ui->setupUi(this);

    // We let the window get up
    mWarmupId = startTimer(1000);
}
//----------------------------------------
FlashingDialog::~FlashingDialog()
{
    delete ui;
}
//----------------------------------------
void FlashingDialog::timerEvent(QTimerEvent* /*unused*/)
{
    killTimer(mWarmupId);

    // And off we go!
    FlashScenario* scenario = new FlashScenario();
    if (!scenario->flash(mBuildToFlash))
    {
        QMessageBox::critical(this, tr("Critical error"), tr("Unable to flash!"));
    }
}
//----------------------------------------
