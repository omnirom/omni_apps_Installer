#ifndef ROMLISTDIALOG_H
#define ROMLISTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QNetworkReply>
#include <QListWidgetItem>

#include "../BundleManager.h"

namespace Ui {
class RomListDialog;
}

class RomListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RomListDialog(const QString& device, QWidget *parent = 0);
    virtual ~RomListDialog();

protected:
    void updateList(BuildType type);
    void getRomIcon(const QString& url, QListWidgetItem* item);

signals:

public slots:
    void onClickBack();
    void onClickInstall();
    void onIconDownloadFinished(QNetworkReply*);
    void onClickListItem(QListWidgetItem*);
    void onChangeBuildType(int);

private:
    Ui::RomListDialog *ui;
    QString mActiveDevice;
    QMap<QNetworkReply*, QListWidgetItem*> mPendingIcons;
    QMap<QListWidgetItem*, BundleBuild*> mRomItems;
};

#endif // ROMLISTDIALOG_H
