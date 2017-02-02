#include "newpassworddialog.h"
#include "ui_newpassworddialog.h"
#include <QMessageBox>

NewPasswordDialog::NewPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewPasswordDialog)
{
    ui->setupUi(this);
}

NewPasswordDialog::~NewPasswordDialog()
{
    delete ui;
}

void NewPasswordDialog::accept()
{
    if(ui->lnePassword->text()!=ui->lneConfirm->text()){
        QMessageBox::warning(this,tr("Error"),
                             tr("password do not match"));
    }
    else{
        QDialog::accept();
    }
}
