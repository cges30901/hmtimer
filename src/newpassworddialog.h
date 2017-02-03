#ifndef NEWPASSWORDDIALOG_H
#define NEWPASSWORDDIALOG_H

#include <QDialog>
#include "programoptions.h"

namespace Ui {
class NewPasswordDialog;
}

class NewPasswordDialog : public QDialog
{
    Q_OBJECT
    ProgramOptions *programOptions;

public:
    explicit NewPasswordDialog(ProgramOptions *programOptions, QWidget *parent = 0);
    ~NewPasswordDialog();

private:
    Ui::NewPasswordDialog *ui;
    void accept();
};

#endif // NEWPASSWORDDIALOG_H
