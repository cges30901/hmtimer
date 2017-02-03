#include "newpassworddialog.h"
#include "ui_newpassworddialog.h"
#include <QMessageBox>
#include <QCryptographicHash>

NewPasswordDialog::NewPasswordDialog(ProgramOptions *programOptions, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewPasswordDialog)
{
    ui->setupUi(this);
    this->programOptions=programOptions;
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
        programOptions->password=QCryptographicHash::hash(ui->lnePassword->text().toUtf8(),QCryptographicHash::Sha1).toHex();
        QDialog::accept();
    }
}
