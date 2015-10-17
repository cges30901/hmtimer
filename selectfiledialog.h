#ifndef SELECTFILEDIALOG_H
#define SELECTFILEDIALOG_H
#include <QDialog>
#include "ui_selectfiledialog.h"
#include "programoptions.h"

class SelectFileDialog:public QDialog,public Ui::SelectFileDialog
{
    Q_OBJECT
    ProgramOptions *programOptions;
public:
    SelectFileDialog(ProgramOptions *programOptions,QWidget *parent=0);
private slots:
    void setFile();
    void on_btnOk_clicked();
};

#endif // SELECTFILEDIALOG_H
