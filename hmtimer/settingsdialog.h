/*
Hsiu-Ming's Timer(hmtimer) A graphical shutdown timer
Copyright (C) 2014-2016 Hsiu-Ming Chang <cges30901@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <QDialog>
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
    void on_chbStartup_clicked(bool checked);
};

#endif // SETTINGSDIALOG_H
