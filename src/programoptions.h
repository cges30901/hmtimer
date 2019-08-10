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
#ifndef PROGRAMOPTIONS_H
#define PROGRAMOPTIONS_H

#include <QString>
class ProgramOptions
{
public:
    //SettingsDialog
    bool chbMinimizeToTray_Checked;
    bool chbBeep_Checked;
    int spbBeep_Value;
    bool chbAudioBeep_Checked;
    bool chbPassword_Checked;
    QString password;

    bool chbStartup_Checked;
    bool chbMinimizeOnStartup_Checked;
    bool chbStartTimerOnStartup_Checked;
    int spbStartupHour_Value;
    int spbStartupMinute_Value;
    int spbStartupSecond_Value;
    bool chbStartupAt_Checked;
    int spbStartupAtHour_Value;
    int spbStartupAtMinute_Value;

    //SelectFileDialog
    QString lneFilename_Text;
    QString lneParameters_Text;

    //AtDialog
    int spbAtHour_Value;
    int spbAtMinute_Value;
};

#endif // PROGRAMOPTIONS_H
