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
#include "atdialog.h"
#include <QFileDialog>
#include <QTime>

AtDialog::AtDialog(int *timeset, QWidget *parent):QDialog(parent)
{
    setupUi(this);
    this->timeset=timeset;
}

void AtDialog::on_btnOk_clicked()
{
    int atTime=spbHour->value()*3600+spbMinute->value()*60;
    int currentTime=QTime::currentTime().msecsSinceStartOfDay()/1000;
    if(atTime<currentTime)
        *timeset=86400-(currentTime-atTime);
    else
        *timeset=atTime-currentTime;
    this->accept();
}
