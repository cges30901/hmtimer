#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <QDialog>
#include <stdio.h>
#include "ui_settingsdialog.h"
#include "programoptions.h"

class SettingsDialog:public QDialog,public Ui::SettingsDialog
{
    Q_OBJECT
    ProgramOptions *programOptions;
public:
    SettingsDialog(ProgramOptions *programOptions,QWidget *parent=0);
private slots:

    void on_btnOk_clicked();
};

#endif // SETTINGSDIALOG_H
