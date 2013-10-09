#include "FinishDialog.h"
#include "ui_finishdialog.h"

//----------------------------------------
FinishDialog::FinishDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinishDialog)
{
    ui->setupUi(this);
}
//----------------------------------------
FinishDialog::~FinishDialog()
{
    delete ui;
}
//----------------------------------------
