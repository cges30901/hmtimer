#ifdef Q_OS_WIN
# include <tchar.h>
# include <windows.h>
#endif
#include <fstream>
#include <QDir>
#include <cstdio>
#include "settingsdialog.h"

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

    //startup
#ifdef Q_OS_LINUX
    chbMinimizeOnStartup->hide();
#endif
    chbStartup->setChecked(programOptions->chbStartup_Checked);
    chbMinimizeOnStartup->setChecked(programOptions->chbMinimizeOnStartup_Checked);
    chbStartTimerOnStartup->setChecked(programOptions->chbStartTimerOnStartup_Checked);
    spbStartupHour->setValue(programOptions->spbStartupHour_Value);
    spbStartupMinute->setValue(programOptions->spbStartupMinute_Value);
    spbStartupSecond->setValue(programOptions->spbStartupSecond_Value);
}

void SettingsDialog::on_btnOk_clicked()
{
    //general
    programOptions->chbMinimizeToTray_Checked=chbMinimizeToTray->isChecked();
    programOptions->chbBeep_Checked=chbBeep->isChecked();
    programOptions->spbBeep_Value=spbBeep->value();
    programOptions->chbAudioBeep_Checked=chbAudioBeep->isChecked();

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
            <<"Name=timer\n"
            <<"Exec="<<qPrintable(qApp->applicationFilePath());
        file<<" -t "<<qPrintable(QString::number(spbStartupHour->value()*3600+spbStartupMinute->value()*60+spbStartupSecond->value()));
        if(chbMinimizeOnStartup->isChecked()){
            file<<" -m";
        }
        if(chbStartTimerOnStartup->isChecked()){
            file<<" -s";
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
    this->close();
}
