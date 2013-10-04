#include "RomListDialog.h"
#include "ui_romlistdialog.h"

#include "../BundleManager.h"
#include "ChooserDialog.h"

#include <QDebug>

//----------------------------------------
RomListDialog::RomListDialog(const QString& device, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RomListDialog), mActiveDevice(device)
{
    ui->setupUi(this);

    Bundle* currBundle = BundleManager::getDefault()->getCurrentBundle();

    // Build the list of available ROMs
    QList<BundleRom*> roms = currBundle->getSupportedRoms(device);
    for (QList<BundleRom*>::iterator it = roms.begin(); it != roms.end(); ++it)
    {
        BundleRom* rom = (*it);

        QListWidgetItem* item = new QListWidgetItem(tr("%1 (Android %2)").arg(rom->getName())
                                                    .arg(rom->getAndroidVersion()));
        ui->listRoms->addItem(item);
    }

    connect(ui->btnBack, SIGNAL(clicked()), this, SLOT(onClickBack()));

}
//----------------------------------------
RomListDialog::~RomListDialog()
{
    delete ui;
}
//----------------------------------------
void RomListDialog::onClickBack()
{
    close();

    ChooserDialog* dialog = new ChooserDialog(mActiveDevice);
    dialog->show();
}
//----------------------------------------
