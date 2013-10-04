#include "WelcomeDialog.h"
#include "ChooserDialog.h"
#include "ui_welcomedialog.h"

#include "../AdbMonitor.h"
#include "../AdbDevice.h"
#include "../BundleManager.h"
#include "../FlashScenario.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDebug>

//------------------------------------------------------
WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog), mChooserDialog(NULL), mDownloadProgress(NULL)
{
    ui->setupUi(this);
    ui->containerWizardButtons->setVisible(false);

    AdbMonitor* monitor = AdbMonitor::getDefault();
    connect(monitor, SIGNAL(onDeviceConnected(AdbDevice*)),
            this, SLOT(onDeviceConnected(AdbDevice*)));
    connect(monitor, SIGNAL(onDeviceDisconnected(AdbDevice*)),
            this, SLOT(onDeviceDisconnected(AdbDevice*)));
    connect(monitor, SIGNAL(onAdbReady()), this, SLOT(onAdbReady()));

    connect(ui->btnContinue, SIGNAL(clicked()), this, SLOT(onClickContinue()));

    ui->btnNotRecognized->setVisible(false);

    setUiState(NULL);
}
//------------------------------------------------------
WelcomeDialog::~WelcomeDialog()
{
    delete ui;
    if (mChooserDialog) delete mChooserDialog;
}
//------------------------------------------------------
void WelcomeDialog::onAdbReady()
{
    setUiState(NULL);
}
//------------------------------------------------------
void WelcomeDialog::setUiState(AdbDevice* device)
{
    if (device)
    {
        switch (device->getState())
        {
        case ADB_DEVICE_STATE_ONLINE:
            mActiveDevice = device->getDevice();
            mActiveDeviceFullname = device->getModel().replace("_"," ");

            ui->lblTitleAction->setText(tr("%1 (%2)").arg(mActiveDeviceFullname).arg(mActiveDevice));
            ui->lblSummary->setText(tr("Is this your device?"));
            ui->containerWizardButtons->setVisible(true);
            ui->btnNotRecognized->setVisible(false);

            break;

        case ADB_DEVICE_STATE_UNAUTHORIZED:
        case ADB_DEVICE_STATE_OFFLINE:
            ui->lblTitleAction->setText(tr("Please allow access"));
            ui->lblSummary->setText(tr("Unlock your phone screen, and accept the dialog asking you your permission to allow a computer to run commands on your device"));
            ui->containerWizardButtons->setVisible(false);
            ui->btnNotRecognized->setVisible(false);
            break;

        case ADB_DEVICE_STATE_RECOVERY:
            ui->lblTitleAction->setText(tr("Recovery device detected"));
            ui->lblSummary->setText(tr("Your device has been detected in recovery mode. To restore your device to a working state, press the button below. (TODO)"));
            ui->containerWizardButtons->setVisible(false);
            ui->btnNotRecognized->setVisible(false);
            break;
        }

    }
    else
    {
        ui->lblTitleAction->setText(tr("Plug your device now"));
        ui->lblSummary->setText(tr("Once connected, your computer will install required drivers "
                                       "automatically, and you'll be able to choose further options."));
        ui->containerWizardButtons->setVisible(false);
        ui->btnNotRecognized->setVisible(true);

        if (mChooserDialog && mChooserDialog->isVisible())
        {
            // The device has been disconnected, and user was on the Chooser dialog. We close the
            // chooser and get back here.
            delete mChooserDialog;
            mChooserDialog = NULL;
            show();
        }
    }
}
//------------------------------------------------------
void WelcomeDialog::onDeviceConnected(AdbDevice* device)
{
    setUiState(device);
}
//------------------------------------------------------
void WelcomeDialog::onDeviceDisconnected(AdbDevice* device)
{
    Q_UNUSED(device);
    setUiState(NULL);
}
//------------------------------------------------------
void WelcomeDialog::onClickContinue()
{
    // We first check if the device is supported (ie. we have a scenario for it)
    if (!FlashScenario::existsForDevice(mActiveDevice))
    {
        QMessageBox::critical(this, tr("Unsupported device"),
                              tr("Unfortunately, we don't have information yet on how to flash your %1 (%2)")
                              .arg(mActiveDeviceFullname)
                              .arg(mActiveDevice));
        return;
    }

    // We hide the current dialog, and fetch available bundles from our back-end
    close();

    // Show a nice dialog
    if (mDownloadProgress) delete mDownloadProgress;
    mDownloadProgress = new QProgressDialog();
    mDownloadProgress->setCancelButton(NULL);
    mDownloadProgress->setLabelText(tr("Downloading device support..."));
    mDownloadProgress->setValue(50); //TODO: Real progress callback
    mDownloadProgress->show();

    qApp->processEvents();

    // And on we fetch!
    BundleManager* bm = BundleManager::getDefault();
    connect(bm, SIGNAL(bundleReady()), this, SLOT(onBundleReady()));

    bm->fetchBundle("http://xplod.fr/test_oi/bundle.json");
}
//------------------------------------------------------
void WelcomeDialog::onBundleReady()
{
    BundleManager* bm = BundleManager::getDefault();
    disconnect(bm, SIGNAL(bundleReady()), this, SLOT(onBundleReady()));

    delete mDownloadProgress;
    mDownloadProgress = NULL;

    if (!bm->getCurrentBundle()->isDeviceSupported(mActiveDevice))
    {
        QMessageBox::critical(this, tr("Your device is not supported"),
                              tr("Oops, it seems like your %1 isn't supported by %2.")
                              .arg(mActiveDeviceFullname).arg(bm->getCurrentBundle()->getProviderName()));
        show();
        return;
    }

    // We open the chooser dialog if everything went fine so far
    if (mChooserDialog != NULL)
    {
        delete mChooserDialog;
        mChooserDialog = NULL;
    }

    mChooserDialog = new ChooserDialog(mActiveDevice);
    mChooserDialog->show();
}
//------------------------------------------------------
