#ifndef ROMLISTDIALOG_H
#define ROMLISTDIALOG_H

#include <QDialog>

namespace Ui {
class RomListDialog;
}

class RomListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RomListDialog(const QString& device, QWidget *parent = 0);
    virtual ~RomListDialog();

signals:

public slots:
    void onClickBack();

private:
    Ui::RomListDialog *ui;
    QString mActiveDevice;
};

#endif // ROMLISTDIALOG_H
