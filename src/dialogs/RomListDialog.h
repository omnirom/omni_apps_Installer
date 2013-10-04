#ifndef ROMLISTDIALOG_H
#define ROMLISTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QNetworkReply>
#include <QListWidgetItem>

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
    void getRomIcon(const QString& url, QListWidgetItem* item);

signals:

public slots:
    void onClickBack();
    void onIconDownloadFinished(QNetworkReply*);

private:
    Ui::RomListDialog *ui;
    QString mActiveDevice;
    QMap<QNetworkReply*, QListWidgetItem*> mPendingIcons;
};

#endif // ROMLISTDIALOG_H
