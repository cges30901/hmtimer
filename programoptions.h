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
    bool chbStartup_Checked;
    bool chbMinimizeOnStartup_Checked;
    bool chbStartTimerOnStartup_Checked;
    int spbStartupHour_Value;
    int spbStartupMinute_Value;
    int spbStartupSecond_Value;

    //SelectFileDialog
    QString lneFilename_Text;
    QString lneParameters_Text;
};

#endif // PROGRAMOPTIONS_H
