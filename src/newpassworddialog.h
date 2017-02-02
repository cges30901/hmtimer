#ifndef NEWPASSWORDDIALOG_H
#define NEWPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class NewPasswordDialog;
}

class NewPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewPasswordDialog(QWidget *parent = 0);
    ~NewPasswordDialog();

private:
    Ui::NewPasswordDialog *ui;
    void accept();
};

#endif // NEWPASSWORDDIALOG_H
