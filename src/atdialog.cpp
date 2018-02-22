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
#include <QTime>

AtDialog::AtDialog(ProgramOptions *programOptions, QWidget *parent):QDialog(parent)
{
    setupUi(this);
    this->programOptions=programOptions;
    spbHour->setValue(programOptions->spbAtHour_Value);
    spbMinute->setValue(programOptions->spbAtMinute_Value);
}

void AtDialog::on_btnOk_clicked()
{
    programOptions->spbAtHour_Value=spbHour->value();
    programOptions->spbAtMinute_Value=spbMinute->value();
    this->accept();
}
