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
#ifndef ATDIALOG_H
#define ATDIALOG_H
#include <QDialog>
#include "ui_atdialog.h"

class AtDialog:public QDialog,public Ui::AtDialog
{
    Q_OBJECT
    int *timeset;
public:
    AtDialog(int *timeset,QWidget *parent=0);
private slots:
    void on_btnOk_clicked();
};

#endif // ATDIALOG_H
