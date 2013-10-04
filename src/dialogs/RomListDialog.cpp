#include "RomListDialog.h"
#include "ui_romlistdialog.h"

#include "../BundleManager.h"
#include "../util/HttpClient.h"
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
        getRomIcon(rom->getIconUrl(), item);
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
void RomListDialog::getRomIcon(const QString& url, QListWidgetItem* item)
{
    HttpClient* httpClient = new HttpClient(this);
    mPendingIcons[httpClient->downloadUrl(url)] = item;
    connect(httpClient, SIGNAL(onDownloadFinished(QNetworkReply*)),
            this, SLOT(onIconDownloadFinished(QNetworkReply*)));
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
