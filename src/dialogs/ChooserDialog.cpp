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

    Bundle* currBundle = BundleManager::getDefault()->getCurrentBundle();

    ui->lblBundle->setText(tr("Bundle provided by %1 (%2)")
                           .arg(currBundle->getProviderName())
                           .arg(currBundle->getPublicUrl()));
}
//----------------------------------------
ChooserDialog::~ChooserDialog()
{
    delete ui;
}
//----------------------------------------
