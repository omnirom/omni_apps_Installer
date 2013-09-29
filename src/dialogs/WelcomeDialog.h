#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

namespace Ui {
class WelcomeDialog;
}

class AdbDevice;
class ChooserDialog;

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = 0);
    ~WelcomeDialog();

    void setUiState(AdbDevice* device);

private slots:
    void onDeviceConnected(AdbDevice* device);
    void onDeviceDisconnected(AdbDevice* device);
    void onClickContinue();

private:
    Ui::WelcomeDialog *ui;
    ChooserDialog* mChooserDialog;
};

#endif // WELCOMEDIALOG_H
