#include "RomListDialog.h"
#include "ui_romlistdialog.h"

#include "../BundleManager.h"
#include "../util/HttpClient.h"
#include "ChooserDialog.h"
#include "FlashingDialog.h"

#include <QDebug>
#include <QMessageBox>

//----------------------------------------
RomListDialog::RomListDialog(const QString& device, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RomListDialog), mActiveDevice(device)
{
    ui->setupUi(this);

    // Change the UI default to change the build type default
    updateList((BuildType) ui->cbBuildType->currentIndex());

    connect(ui->btnBack, SIGNAL(clicked()), this, SLOT(onClickBack()));
    connect(ui->btnInstall, SIGNAL(clicked()), this, SLOT(onClickInstall()));
    connect(ui->listRoms, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onClickListItem(QListWidgetItem*)));
    connect(ui->cbBuildType, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeBuildType(int)));
}
//----------------------------------------
RomListDialog::~RomListDialog()
{
    delete ui;
}
//----------------------------------------
void RomListDialog::updateList(BuildType type)
{
    Bundle* currBundle = BundleManager::getDefault()->getCurrentBundle();

    // Build the list of available ROMs based on the type selected
    QList<BundleRom*> roms = currBundle->getSupportedRoms(mActiveDevice);
    for (QList<BundleRom*>::iterator it = roms.begin(); it != roms.end(); ++it)
    {
        BundleRom* rom = (*it);
        QList<BundleBuild*> builds = rom->getBuilds(type, mActiveDevice);

        for (QList<BundleBuild*>::iterator buildIt = builds.begin();
             buildIt != builds.end(); ++buildIt)
        {
            BundleBuild* build = (*buildIt);
            QListWidgetItem* item = new QListWidgetItem(tr("%1 (Android %2)\n%3").arg(rom->getName())
                                                        .arg(rom->getAndroidVersion()).arg(build->getDate().toString()));
            getRomIcon(rom->getIconUrl(), item);
            mRomItems[item] = build;

            ui->listRoms->addItem(item);
        }
    }
}
//----------------------------------------
void RomListDialog::getRomIcon(const QString& url, QListWidgetItem* item)
{
    HttpClient* httpClient = new HttpClient(this);
    mPendingIcons[httpClient->downloadUrl(url)] = item;
    connect(httpClient, SIGNAL(onDownloadFinished(QNetworkReply*)),
            this, SLOT(onIconDownloadFinished(QNetworkReply*)));
}
//----------------------------------------
void RomListDialog::onClickBack()
{
    close();

    ChooserDialog* dialog = new ChooserDialog(mActiveDevice);
    dialog->show();
}
//----------------------------------------
void RomListDialog::onClickInstall()
{
    if (ui->listRoms->selectedItems().size() == 0)
    {
        QMessageBox::critical(this, tr("Missing selection"), tr("Please select the ROM to flash"));
        return;
    }

    if (QMessageBox::question(this, tr("About to flash"),
                              tr("You are about to update your device. It will reboot multiple times in the process, and must NOT be unplugged from USB!\nContinue?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
        // Ok :(
        return;
    }

    close();

    FlashingDialog* dialog = new FlashingDialog(mRomItems[ui->listRoms->currentItem()]);
    dialog->show();
}
//----------------------------------------
void RomListDialog::onIconDownloadFinished(QNetworkReply* reply)
{
    QListWidgetItem* item = mPendingIcons[reply];
    if (!item)
    {
        qDebug() << "No matching widget item for the received network reply!";
        return;
    }

    QByteArray iconData = reply->readAll();
    item->setIcon(QIcon(QPixmap::fromImage(QImage::fromData(iconData))));
}
//----------------------------------------
void RomListDialog::onClickListItem(QListWidgetItem* item)
{
    // Get more info from that ROM
    BundleBuild* rom = mRomItems[item];

    if (!rom)
    {
        qDebug() << "Can't find the associated build!";
        return;
    }

    ui->lblTitleValue->setText(rom->getTitle());
    ui->lblDescriptionValue->setText(rom->getDescription());
    ui->lblDateValue->setText(rom->getDate().toString());

}
//----------------------------------------
void RomListDialog::onChangeBuildType(int type)
{
    updateList((BuildType) type);
}
//----------------------------------------
