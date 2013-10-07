#include "FlashingDialog.h"
#include "ui_flashingdialog.h"

#include "../BundleManager.h"
#include "../FlashScenario.h"
#include "../util/HttpClient.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>

//----------------------------------------
FlashingDialog::FlashingDialog(BundleBuild* build, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlashingDialog), mBuildToFlash(build)
{
    ui->setupUi(this);

    mScenario = new FlashScenario();

    ui->lblSubTitle->setText("Downloading build...");

    // Download the build for a start
    HttpClient* cli = new HttpClient(this);
    connect(cli, SIGNAL(onDownloadFinished(QNetworkReply*)),
            this, SLOT(onBuildDownloaded(QNetworkReply*)));

    qDebug() << "Downloading build...";
    cli->downloadUrl(mBuildToFlash->getDownloadUrl());
}
//----------------------------------------
FlashingDialog::~FlashingDialog()
{
    delete ui;
}
//----------------------------------------
void FlashingDialog::onBuildDownloaded(QNetworkReply *reply)
{
    // We save the file to HDD; FIXME general path
    QFile writer(this);
    QString path = "C:\\Users\\guigui\\tmp_rom.zip";
    writer.setFileName(path);
    if (!writer.open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(this, tr("Unable to save the build"), tr("Unable to save the ROM installation file on your HDD."));
        return;
    }

    ui->lblSubTitle->setText("Writing build to HDD...");
    qApp->processEvents();

    qDebug() << "Writing update zip to " << path;
    writer.write(reply->readAll());
    writer.close();

    // Download the recovery now
    HttpClient* cli = new HttpClient(this);
    connect(cli, SIGNAL(onDownloadFinished(QNetworkReply*)),
            this, SLOT(onRecoveryDownloaded(QNetworkReply*)));

    ui->lblSubTitle->setText("Downloading recovery...");
    qApp->processEvents();

    mScenario->loadForDevice(mBuildToFlash->getDevice());
    qDebug() << "Downloading recovery... " << mScenario->getScenarioData()->recoveryUrl;
    cli->downloadUrl(mScenario->getScenarioData()->recoveryUrl);
}
//----------------------------------------
void FlashingDialog::onRecoveryDownloaded(QNetworkReply *reply)
{
    // We save the file to HDD; FIXME general path
    QFile writer(this);
    QString path = "C:\\Users\\guigui\\tmp_recovery.img";
    QString romPath = "C:\\Users\\guigui\\tmp_rom.zip";
    writer.setFileName(path);
    if (!writer.open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(this, tr("Unable to save the recovery"), tr("Unable to save the ROM installation file on your HDD."));
        return;
    }

    ui->lblSubTitle->setText("Writing recovery to HDD...");
    qApp->processEvents();

    qDebug() << "Writing recovery img to " << path;
    writer.write(reply->readAll());
    writer.close();

    mScenario->getScenarioData()->recoveryLocalPath = path;

    // And off we go!
    QStringList zipFiles;
    zipFiles << romPath;

    ui->lblSubTitle->setText("Flashing...");
    qApp->processEvents();

    if (!mScenario->flash(mBuildToFlash, zipFiles))
    {
        QMessageBox::critical(this, tr("Critical error"), tr("Unable to flash!"));
    }
}
//----------------------------------------
