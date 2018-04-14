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
#include <QtGlobal>
#ifdef Q_OS_WIN
# include <tchar.h>
# include <windows.h>
#endif
#include <fstream>
#include <QDir>
#include "settingsdialog.h"
#include "newpassworddialog.h"
#include "passworddialog.h"

SettingsDialog::SettingsDialog(ProgramOptions *programOptions, QWidget *parent):QDialog(parent)
{
    setupUi(this);
    this->programOptions=programOptions;
    tabWidget->setCurrentIndex(0);

    //general
    chbMinimizeToTray->setChecked(programOptions->chbMinimizeToTray_Checked);
    chbBeep->setChecked(programOptions->chbBeep_Checked);
    spbBeep->setValue(programOptions->spbBeep_Value);
    chbAudioBeep->setChecked(programOptions->chbAudioBeep_Checked);
    chbAudioBeep->setEnabled(chbBeep->isChecked());
    chbPassword->setChecked(programOptions->chbPassword_Checked);

    //startup
    chbStartup->setChecked(programOptions->chbStartup_Checked);
    chbMinimizeOnStartup->setChecked(programOptions->chbMinimizeOnStartup_Checked);
    chbStartTimerOnStartup->setChecked(programOptions->chbStartTimerOnStartup_Checked);
    spbStartupHour->setValue(programOptions->spbStartupHour_Value);
    spbStartupMinute->setValue(programOptions->spbStartupMinute_Value);
    spbStartupSecond->setValue(programOptions->spbStartupSecond_Value);

    chbMinimizeOnStartup->setEnabled(programOptions->chbStartup_Checked);
    chbStartTimerOnStartup->setEnabled(programOptions->chbStartup_Checked);
    spbStartupHour->setEnabled(programOptions->chbStartup_Checked);
    spbStartupMinute->setEnabled(programOptions->chbStartup_Checked);
    spbStartupSecond->setEnabled(programOptions->chbStartup_Checked);
}

void SettingsDialog::on_btnOk_clicked()
{
    //general
    programOptions->chbMinimizeToTray_Checked=chbMinimizeToTray->isChecked();
    programOptions->chbBeep_Checked=chbBeep->isChecked();
    programOptions->spbBeep_Value=spbBeep->value();
    programOptions->chbAudioBeep_Checked=chbAudioBeep->isChecked();
    programOptions->chbPassword_Checked=chbPassword->isChecked();

    //startup
    programOptions->chbStartup_Checked=chbStartup->isChecked();
    programOptions->chbMinimizeOnStartup_Checked=chbMinimizeOnStartup->isChecked();
    programOptions->chbStartTimerOnStartup_Checked=chbStartTimerOnStartup->isChecked();
    programOptions->spbStartupHour_Value=spbStartupHour->value();
    programOptions->spbStartupMinute_Value=spbStartupMinute->value();
    programOptions->spbStartupSecond_Value=spbStartupSecond->value();

#ifdef Q_OS_LINUX
    if(chbStartup->isChecked()){
        std::ofstream file(qPrintable(QDir::homePath()+"/.config/autostart/hmtimer.desktop"));
        file<<"[Desktop Entry]\n"
            <<"Type=Application\n"
            <<"Name=hmtimer\n"
            <<"Exec="<<qPrintable(qApp->applicationFilePath());
        if(chbMinimizeOnStartup->isChecked()){
            file<<" -m";
        }
        if(chbStartTimerOnStartup->isChecked()){
            file<<" -t "<<qPrintable(QString::number(spbStartupHour->value()*3600+spbStartupMinute->value()*60+spbStartupSecond->value()));
        }
        file<<"\nTerminal=false\n"
            <<"Hidden=false";
        file.close();
    }
    else
    {
        std::remove(qPrintable(QDir::homePath()+"/.config/autostart/hmtimer.desktop"));
    }
#endif

#ifdef Q_OS_WIN
    HKEY newValue;
    if (RegOpenKey(HKEY_CURRENT_USER,
                   TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
                   &newValue) != ERROR_SUCCESS)
    {
        printf("Unable to open registry key; last error = %lu\n", GetLastError());
    }

    if(chbStartup->isChecked()){
        //create registry key
        wchar_t szPath[MAX_PATH];
        DWORD pathLen = 0;
        // GetModuleFileName returns the number of characters
        // written to the array.
        pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
        if (pathLen == 0)
        {
            printf("Unable to get module file name; last error = %lu\n", GetLastError());
        }


        // Need to pass the length of the path string in bytes,
        // which may not equal the number of characters due to
        // character set.
        QString argument=QString("\"")+QString::fromWCharArray(szPath)+QString("\"");
        argument+=" -t "+QString::number(spbStartupHour->value()*3600+spbStartupMinute->value()*60+spbStartupSecond->value());
        if(chbMinimizeOnStartup->isChecked()){
            argument+=" -m";
        }
        if(chbStartTimerOnStartup->isChecked()){
            argument+=" -s";
        }
        argument.toWCharArray(szPath);
        //pathLen=wcslen(szPath);
        DWORD pathLenInBytes = argument.length() * sizeof(*szPath);
        if (RegSetValueEx(newValue,
                          TEXT("hmtimer"),
                          0,
                          REG_SZ,
                          (LPBYTE)szPath,
                          pathLenInBytes) != ERROR_SUCCESS)
        {
            printf("Unable to set registry value; last error = %lu\n", GetLastError());
        }
        RegCloseKey(newValue);
    }
    else{
        //delete registry
        RegSetValueEx(newValue,
                      TEXT("hmtimer"),
                      0,
                      REG_SZ,
                      (LPBYTE)TEXT(""),
                      0);
        RegCloseKey(newValue);
    }
#endif
    this->accept();
}

void SettingsDialog::on_chbStartup_clicked(bool checked)
{
    chbMinimizeOnStartup->setEnabled(checked);
    chbStartTimerOnStartup->setEnabled(checked);
    spbStartupHour->setEnabled(checked);
    spbStartupMinute->setEnabled(checked);
    spbStartupSecond->setEnabled(checked);
}

void SettingsDialog::on_chbPassword_clicked(bool checked)
{
    if(checked){
        NewPasswordDialog *dialog=new NewPasswordDialog(programOptions,this);
        if(dialog->exec()==QDialog::Rejected){ //password not set
            chbPassword->setChecked(false);
        }
        delete dialog;
    }
    else{
        PasswordDialog *dialog=new PasswordDialog(programOptions->password,this);
        if(dialog->exec()==QDialog::Rejected){
            chbPassword->setChecked(true);
        }
        delete dialog;
    }
}
