#ifndef CHOOSERDIALOG_H
#define CHOOSERDIALOG_H

#include <QDialog>

namespace Ui {
class ChooserDialog;
}

class ChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooserDialog(const QString& device, QWidget *parent = 0);
    ~ChooserDialog();

private slots:
    void onClickAutoInstall();

private:
    Ui::ChooserDialog *ui;
    QString mActiveDevice;
};

#endif // CHOOSERDIALOG_H
