#include "ChooserDialog.h"
#include "RomListDialog.h"
#include "ui_chooserdialog.h"

#include "../BundleManager.h"

#include <QDebug>

//----------------------------------------
ChooserDialog::ChooserDialog(const QString& device, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooserDialog)
{
    ui->setupUi(this);
    mActiveDevice = device;

    Bundle* currBundle = BundleManager::getDefault()->getCurrentBundle();

    ui->lblBundle->setText(tr("Bundle provided by %1 (%2)")
                           .arg(currBundle->getProviderName())
                           .arg(currBundle->getPublicUrl()));

    connect(ui->btnAutoInstall, SIGNAL(clicked()), this, SLOT(onClickAutoInstall()));
}
//----------------------------------------
ChooserDialog::~ChooserDialog()
{
    delete ui;
}
//----------------------------------------
void ChooserDialog::onClickAutoInstall()
{
    close();

    RomListDialog* dialog = new RomListDialog(mActiveDevice);
    dialog->show();
}
//----------------------------------------
