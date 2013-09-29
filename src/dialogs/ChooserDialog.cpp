#include "ChooserDialog.h"
#include "ui_chooserdialog.h"

#include "../BundleManager.h"

#include <QDebug>

//----------------------------------------
ChooserDialog::ChooserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooserDialog)
{
    ui->setupUi(this);

    BundleManager::getDefault()->fetchBundle("derp");
}
//----------------------------------------
ChooserDialog::~ChooserDialog()
{
    delete ui;
}
//----------------------------------------
