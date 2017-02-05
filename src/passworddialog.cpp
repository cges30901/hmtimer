#include "passworddialog.h"
#include "ui_passworddialog.h"
#include <QCryptographicHash>
#include <QMessageBox>

PasswordDialog::PasswordDialog(const QString password, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
    this->password=password;
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::on_btnOk_clicked()
{
    if(QCryptographicHash::hash(ui->lnePassword->text().toUtf8(),QCryptographicHash::Sha1).toHex()==password){
        accept();
    }
    else{
        QMessageBox::warning(this,tr("Wrong password"),tr("Wrong password"));
    }
}
