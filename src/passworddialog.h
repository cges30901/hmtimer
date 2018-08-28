#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT
    QString password;

public:
    explicit PasswordDialog(const QString password, QWidget *parent = Q_NULLPTR);
    ~PasswordDialog();

private slots:
    void on_btnOk_clicked();

private:
    Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
