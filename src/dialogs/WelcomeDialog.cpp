#include "WelcomeDialog.h"
#include "ChooserDialog.h"
#include "ui_welcomedialog.h"
#include "../AdbMonitor.h"
#include "../AdbDevice.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDebug>"

//------------------------------------------------------
WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog), mChooserDialog(NULL)
{
    ui->setupUi(this);
    ui->containerWizardButtons->setVisible(false);

    AdbMonitor* monitor = AdbMonitor::getDefault();
    connect(monitor, SIGNAL(onDeviceConnected(AdbDevice*)),
            this, SLOT(onDeviceConnected(AdbDevice*)));
    connect(monitor, SIGNAL(onDeviceDisconnected(AdbDevice*)),
            this, SLOT(onDeviceDisconnected(AdbDevice*)));

    connect(ui->btnContinue, SIGNAL(clicked()), this, SLOT(onClickContinue()));

    setUiState(NULL);
}
//------------------------------------------------------
WelcomeDialog::~WelcomeDialog()
{
    delete ui;
    if (mChooserDialog) delete mChooserDialog;
}
//------------------------------------------------------
void WelcomeDialog::setUiState(AdbDevice* device)
{
    if (device)
    {
        ui->lblTitleAction->setText(device->getModel().replace("_"," ") + " (" + device->getDevice() + ")");
        ui->lblSummary->setText("Is this your device?");
        ui->containerWizardButtons->setVisible(true);
        ui->btnNotRecognized->setVisible(false);
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
    // We hide the current dialog, and fetch available bundles from our back-end
    close();

    if (mChooserDialog != NULL)
    {
        delete mChooserDialog;
        mChooserDialog = NULL;
    }

    mChooserDialog = new ChooserDialog();
    mChooserDialog->show();
}
//------------------------------------------------------
