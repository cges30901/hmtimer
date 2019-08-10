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
#ifdef FLATPAK
    tabWidget->setTabEnabled(1,false);
#endif
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
    chbStartupAt->setChecked(programOptions->chbStartupAt_Checked);
    spbStartupAtHour->setValue(programOptions->spbStartupAtHour_Value);
    spbStartupAtMinute->setValue(programOptions->spbStartupAtMinute_Value);

    chbMinimizeOnStartup->setEnabled(programOptions->chbStartup_Checked);
    chbStartTimerOnStartup->setEnabled(programOptions->chbStartup_Checked);
    spbStartupHour->setEnabled(programOptions->chbStartup_Checked);
    spbStartupMinute->setEnabled(programOptions->chbStartup_Checked);
    spbStartupSecond->setEnabled(programOptions->chbStartup_Checked);
    chbStartupAt->setEnabled(programOptions->chbStartup_Checked);
    spbStartupAtHour->setEnabled(programOptions->chbStartup_Checked);
    spbStartupAtMinute->setEnabled(programOptions->chbStartup_Checked);
}

void SettingsDialog::on_buttonBox_accepted()
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
    programOptions->chbStartupAt_Checked=chbStartupAt->isChecked();
    programOptions->spbStartupAtHour_Value=spbStartupAtHour->value();
    programOptions->spbStartupAtMinute_Value=spbStartupAtMinute->value();

    QString argument;
    if(chbMinimizeOnStartup->isChecked()){
        argument+=" -m";
    }
    if(chbStartTimerOnStartup->isChecked()){
        argument+=QString(" -t ")+QString::number(spbStartupHour->value())
                +":"+QString::number(spbStartupMinute->value())
                +":"+QString::number(spbStartupSecond->value());
    }
    if(chbStartupAt->isChecked()){
        argument+=QString(" -at ")+QString::number(spbStartupAtHour->value())
                +":"+QString::number(spbStartupAtMinute->value());
    }

#if defined(Q_OS_LINUX) && !defined(FLATPAK)
    if(chbStartup->isChecked()){
        std::ofstream file(qPrintable(QDir::homePath()+"/.config/autostart/hmtimer.desktop"));
        file<<"[Desktop Entry]\n"
            <<"Type=Application\n"
            <<"Name=hmtimer\n"
            <<"Exec="<<qPrintable(qApp->applicationFilePath())
            <<qPrintable(argument);
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
        argument=QString("\"")+QString::fromWCharArray(szPath)+QString("\"")+argument;
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
    }
    else{
        //delete registry
        RegDeleteValueA(newValue,"hmtimer");
    }
    RegCloseKey(newValue);
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
    chbStartupAt->setEnabled(checked);
    spbStartupAtHour->setEnabled(checked);
    spbStartupAtMinute->setEnabled(checked);
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

void SettingsDialog::on_chbStartTimerOnStartup_stateChanged(int state)
{
    if(state==Qt::Checked){
        spbStartupHour->setEnabled(true);
        spbStartupMinute->setEnabled(true);
        spbStartupSecond->setEnabled(true);
        chbStartupAt->setChecked(false);
    }
    else{
        spbStartupHour->setEnabled(false);
        spbStartupMinute->setEnabled(false);
        spbStartupSecond->setEnabled(false);
    }
}

void SettingsDialog::on_chbStartupAt_stateChanged(int state)
{
    if(state==Qt::Checked){
        spbStartupAtHour->setEnabled(true);
        spbStartupAtMinute->setEnabled(true);
        chbStartTimerOnStartup->setChecked(false);
    }
    else{
        spbStartupAtHour->setEnabled(false);
        spbStartupAtMinute->setEnabled(false);
    }
}
