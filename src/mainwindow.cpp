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
#include <time.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <QTime>
#ifdef Q_OS_LINUX
# include <unistd.h>
# include <linux/kd.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <QtDBus>
#endif
#ifdef Q_OS_WIN32
 #include <windows.h>
#endif
#include "mainwindow.h"
#include "selectfiledialog.h"
#include "settingsdialog.h"
#include "version.h"
#include "passworddialog.h"
#include "atdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    comboAction->addItem(tr("Do nothing")); //index 0
    comboAction->addItem(tr("Turn off monitor")); //index 1
    comboAction->addItem(tr("Shutdown")); //index 2
    comboAction->addItem(tr("Reboot")); //index 3
    comboAction->addItem(tr("Play Sound")); //index 4
    comboAction->addItem(tr("Run Program")); //index 5
    programOptions=new ProgramOptions;
    player=new QMediaPlayer;
    beepPlayer=new QMediaPlayer;
    timer=new QTimer(this);
    timer_enabled=false;
    trayIcon=new QSystemTrayIcon;
    trayIcon->setIcon(QIcon(":/hmtimer.png"));
    trayIcon->setToolTip(tr("Hsiu-Ming's Timer"));
    trayContextMenu=new QMenu;
    actShow=trayContextMenu->addAction(tr("Show"));
    actQuit=trayContextMenu->addAction(tr("Quit"));
    trayIcon->setContextMenu(trayContextMenu);
    dialog_open=false;
    connect(btnStart,&QPushButton::clicked,
            this,&MainWindow::btnStartPressed);
    connect(timer,&QTimer::timeout,
            this,&MainWindow::timer_timeout);
    connect(qApp,&QApplication::aboutToQuit,
            this,&MainWindow::writeSettings);
    connect(trayIcon,&QSystemTrayIcon::activated,
            this,&MainWindow::trayIcon_activated);
    connect(actShow,&QAction::triggered,
            this,&MainWindow::actShow_Triggered);
    connect(actQuit,&QAction::triggered,
            this,&MainWindow::actQuit_Triggered);
    connect(player,&QMediaPlayer::mediaStatusChanged,
            this,&MainWindow::playerMediaStatusChanged);

    readSettings();

    //command line parameter
    QStringList args=qApp->arguments();
    for(int i=0;i<args.size();i++){
        if(args.at(i)=="-a"){
            i++;
            if(args.at(i)=="nothing"){
                comboAction->setCurrentIndex(0);
            }
            else if(args.at(i)=="monitor"){
                comboAction->setCurrentIndex(1);
            }
            else if(args.at(i)=="shutdown"){
                comboAction->setCurrentIndex(2);
            }
            else if(args.at(i)=="reboot"){
                comboAction->setCurrentIndex(3);
            }
            else if(args.at(i)=="sound"){
                comboAction->setCurrentIndex(4);
            }
            else if(args.at(i)=="runprogram"){
                comboAction->setCurrentIndex(5);
            }
        }
        else if(args.at(i)=="-t"){
            i++;
            QString time=args.at(i);
            QStringList list=time.split(':');
            if(list.size()==1){ //time is number in second
                spbHour->setValue(time.toInt()/3600);
                spbMinute->setValue(time.toInt()/60%60);
                spbSecond->setValue(time.toInt()%60);
            }
            else if(list.size()==2){ //time is MM:SS
                int minute=list.at(0).toInt();
                int second=list.at(1).toInt();
                spbHour->setValue(minute/60);
                spbMinute->setValue(minute%60);
                spbSecond->setValue(second);
            }
            else if(list.size()==3){ //time is HH:MM:SS
                int hour=list.at(0).toInt();
                int minute=list.at(1).toInt();
                int second=list.at(2).toInt();
                spbHour->setValue(hour);
                spbMinute->setValue(minute);
                spbSecond->setValue(second);
            }
            btnStartPressed();
        }
        else if(args.at(i)=="-at"){
            i++;
            QString time=args.at(i);
            QStringList list=time.split(':');
            if(list.size()==2){
                startFromAt=true;
                programOptions->spbAtHour_Value=list.at(0).toInt();
                programOptions->spbAtMinute_Value=list.at(1).toInt();
                setAtTime();
                startTimer();
            }
        }
        else if(args.at(i)=="-m"){
            trayIcon->show();
        }
    }
}
void MainWindow::btnStartPressed()
{
    if(timer_enabled) //timer is running
    {
        //stop timer
        if(programOptions->chbPassword_Checked){
            PasswordDialog *dialog=new PasswordDialog(programOptions->password,this);
            if(dialog->exec()==QDialog::Rejected){
                delete dialog;
                return;
            }
            delete dialog;
        }
        stopTimer();
        player->stop();
    }
    else if(player->state()==QMediaPlayer::PlayingState){
        player->stop();
        btnStart->setText(tr("Start"));
    }
    else if(spbHour->value()==0 and spbMinute->value()==0 and spbSecond->value()==0)
    {
        QMessageBox::warning(this,tr("Please set the timer"),
                                 tr("Time can not be zero"));
    }
    else
    {
        startFromAt=false;
        //start timer
        startTimer();
    }
}
void MainWindow::timer_timeout()
{
    if(timeRemain==timeSet-int(difftime(time(Q_NULLPTR),timeStart))){
        return;
    }
    timeRemain=timeSet-int(difftime(time(Q_NULLPTR),timeStart));
    spbHour->setValue(timeRemain/3600);
    spbMinute->setValue(timeRemain/60%60);
    spbSecond->setValue(timeRemain%60);
    if(timeRemain<=0){
        stopTimer();
        if(startFromAt){
            setAtTime();
        }
        else{
            spbHour->setValue(timeSet/3600);
            spbMinute->setValue(timeSet/60%60);
            spbSecond->setValue(timeSet%60);
        }
        writeSettings();
        action();
    }
    else{
        //: %1 is hour, %2 is minute, %3 is second
        trayIcon->setToolTip(tr("Hsiu-Ming's Timer\n%1:%2:%3")
                             .arg(spbHour->text(),spbMinute->text(),spbSecond->text()));
        if(programOptions->chbBeep_Checked==true and timeRemain<programOptions->spbBeep_Value){
            if(programOptions->chbAudioBeep_Checked==false) //beep with pcspkr
            {
#ifdef Q_OS_LINUX
                int fd = open("/dev/console", O_RDONLY);
                ioctl(fd, KDMKTONE, (200<<16 | 1193180/750));
#endif
#ifdef Q_OS_WIN32
                Beep(750,200);
#endif
            }
            else{ //Use audio file to play beep sound
#ifdef Q_OS_LINUX
                beepPlayer->setMedia(QUrl("qrc:/beep.ogg"));
#endif
#ifdef Q_OS_WIN
                beepPlayer->setMedia(QUrl("qrc:/beep.mp3"));
#endif
                beepPlayer->setVolume(100);
                beepPlayer->play();
            }
        }
    }
}

void MainWindow::action()
{
    if(comboAction->currentIndex()==4){ //play sound
        player->play();
        btnStart->setText(tr("Stop"));
    }
    else if(comboAction->currentIndex()==2){ //shutdown
#ifdef Q_OS_LINUX
        QDBusMessage response;
        QDBusInterface freedesktopLogin1("org.freedesktop.login1",
          "/org/freedesktop/login1",
          "org.freedesktop.login1.Manager", QDBusConnection::systemBus());
        response = freedesktopLogin1.call("PowerOff", true);
        if(response.type() == QDBusMessage::ErrorMessage){
            qDebug()<< response.errorName() << ": "
                    << response.errorMessage() << endl;
        }
#endif
#ifdef Q_OS_WIN32
        HANDLE hToken;
        TOKEN_PRIVILEGES tkp;
        OpenProcessToken(GetCurrentProcess(),
                TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
                &tkp.Privileges[0].Luid);
         tkp.PrivilegeCount = 1;  // one privilege to set
           tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

           // Get the shutdown privilege for this process.

           AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
                (PTOKEN_PRIVILEGES)NULL, 0);
        ExitWindowsEx(EWX_POWEROFF|EWX_FORCE,0);
#endif
    }
    else if(comboAction->currentIndex()==3){ //reboot
#ifdef Q_OS_LINUX
        QDBusMessage response;
        QDBusInterface freedesktopLogin1("org.freedesktop.login1",
          "/org/freedesktop/login1",
          "org.freedesktop.login1.Manager", QDBusConnection::systemBus());
        response = freedesktopLogin1.call("Reboot", true);
        if(response.type() == QDBusMessage::ErrorMessage){
            qDebug()<< response.errorName() << ": "
                    << response.errorMessage() << endl;
        }
#endif
#ifdef Q_OS_WIN32
        HANDLE hToken;
        TOKEN_PRIVILEGES tkp;
        OpenProcessToken(GetCurrentProcess(),
                TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
                &tkp.Privileges[0].Luid);
         tkp.PrivilegeCount = 1;  // one privilege to set
           tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

           // Get the shutdown privilege for this process.

           AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
                (PTOKEN_PRIVILEGES)NULL, 0);
        ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
#endif
    }
    else if(comboAction->currentIndex()==1){ //turn off monitor
#ifdef Q_OS_LINUX
        QString wayland=QProcessEnvironment::systemEnvironment().value("WAYLAND_DISPLAY");
        if(wayland=="")
            system("xset dpms force off");
        else
        {
            bool response;
            QDBusInterface mutter("org.gnome.Mutter.DisplayConfig",
                                  "/org/gnome/Mutter/DisplayConfig",
                                  "org.gnome.Mutter.DisplayConfig");
            response = mutter.setProperty("PowerSaveMode", 1);
            if(response==false){
                QMessageBox::warning(this,tr("Error"),tr("Failed to turn off monitor on wayland"));
            }
        }
#endif
#ifdef Q_OS_WIN32
        SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) 2);
#endif
    }
    else if(comboAction->currentIndex()==5){ //run program
        process=new QProcess;
        process->start(programOptions->lneFilename_Text+" "+programOptions->lneParameters_Text);
    }
    if(chbRunRepeatedly->isChecked()){
        if(!chbRepeatTimes->isChecked()){
            startTimer();
        }
        else if(spbRepeatTimes->value()>1){
            spbRepeatTimes->stepDown();
            startTimer();
        }
    }
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,tr("About"),tr("<h3>Hsiu-Ming's Timer %1</h3><br>"
                                           "Author: Hsiu-Ming Chang<br>"
                                           "e-mail: cges30901@gmail.com<br>"
                                           "License: GPL v3<br><br>"
                                           "If you find any bugs or have suggestions, "
                                           "please feel free to contact me by "
                                           "sending me an e-mail.").arg(VERSION));
}

void MainWindow::on_actionWebsite_triggered()
{
    QDesktopServices::openUrl(QUrl("https://cges30901.github.io/hmtimer-website/"));
}

void MainWindow::writeSettings()
{
#ifdef Q_OS_LINUX
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                          "hmtimer", "settings");
#endif

#ifdef Q_OS_WIN
    QSettings settings(qApp->applicationDirPath()+"/settings.ini",QSettings::IniFormat);
#endif

    //mainwindow
    settings.beginGroup("mainwindow");
    settings.setValue("audioFile",audioFile);
    settings.setValue("chbRunRepeatedly",chbRunRepeatedly->isChecked());
    settings.setValue("chbRepeatTimes",chbRepeatTimes->isChecked());
    settings.setValue("action",comboAction->currentIndex());
    settings.endGroup();

    //SelectFileDialog
    settings.beginGroup("selectFileDialog");
    settings.setValue("Filename",programOptions->lneFilename_Text);
    settings.setValue("Parameters",programOptions->lneParameters_Text);
    settings.endGroup();

    //SettingsDialog
    settings.beginGroup("Settings");
    settings.setValue("minimizeToTray",programOptions->chbMinimizeToTray_Checked);
    settings.setValue("checkBoxBeep",programOptions->chbBeep_Checked);
    settings.setValue("spinBoxBeep",programOptions->spbBeep_Value);
    settings.setValue("chbAudioBeep",programOptions->chbAudioBeep_Checked);
    settings.setValue("chbPassword_Checked",programOptions->chbPassword_Checked);
    settings.setValue("password",programOptions->password);

    settings.setValue("chbStartup_Checked",programOptions->chbStartup_Checked);
    settings.setValue("chbMinimizeOnStartup_Checked",programOptions->chbMinimizeOnStartup_Checked);
    settings.setValue("chbStartTimerOnStartup_Checked",programOptions->chbStartTimerOnStartup_Checked);
    settings.setValue("spbStartupHour_Value",programOptions->spbStartupHour_Value);
    settings.setValue("spbStartupMinute_Value",programOptions->spbStartupMinute_Value);
    settings.setValue("spbStartupSecond_Value",programOptions->spbStartupSecond_Value);
    settings.endGroup();

    //AtDialog
    settings.beginGroup("At");
    settings.setValue("spbAtHour_Value",programOptions->spbAtHour_Value);
    settings.setValue("spbAtMinute_Value",programOptions->spbAtMinute_Value);
    settings.endGroup();
}

void MainWindow::readSettings()
{
#ifdef Q_OS_LINUX
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                          "hmtimer", "settings");
#endif

#ifdef Q_OS_WIN
    QSettings settings(qApp->applicationDirPath()+"/settings.ini",QSettings::IniFormat);
#endif

    //mainwindow
    settings.beginGroup("mainwindow");
    audioFile=settings.value("audioFile").toString();
    chbRepeatTimes->setChecked(settings.value("chbRepeatTimes",false).toBool());
    chbRunRepeatedly->setChecked(settings.value("chbRunRepeatedly",false).toBool());
    on_chbRunRepeatedly_toggled(chbRunRepeatedly->isChecked());
    comboAction->setCurrentIndex(settings.value("action").toInt());
    settings.endGroup();

    //SelectFileDialog
    settings.beginGroup("selectFileDialog");
    programOptions->lneFilename_Text=settings.value("Filename").toString();
    programOptions->lneParameters_Text=settings.value("Parameters").toString();
    settings.endGroup();

    //SettingsDialog
    settings.beginGroup("Settings");
    programOptions->chbMinimizeToTray_Checked=settings.value("minimizeToTray",true).toBool();
    programOptions->chbBeep_Checked=settings.value("checkBoxBeep",true).toBool();
    programOptions->spbBeep_Value=settings.value("spinBoxBeep",60).toInt();
    programOptions->chbAudioBeep_Checked=settings.value("chbAudioBeep",true).toBool();
    programOptions->chbPassword_Checked=settings.value("chbPassword_Checked",false).toBool();
    programOptions->password=settings.value("password","").toString();

    programOptions->chbStartup_Checked=settings.value("chbStartup_Checked",false).toBool();
    programOptions->chbMinimizeOnStartup_Checked=settings.value("chbMinimizeOnStartup_Checked",false).toBool();
    programOptions->chbStartTimerOnStartup_Checked=settings.value("chbStartTimerOnStartup_Checked",false).toBool();
    programOptions->spbStartupHour_Value=settings.value("spbStartupHour_Value",0).toInt();
    programOptions->spbStartupMinute_Value=settings.value("spbStartupMinute_Value",0).toInt();
    programOptions->spbStartupSecond_Value=settings.value("spbStartupSecond_Value",0).toInt();
    settings.endGroup();

    //AtDialog
    settings.beginGroup("At");
    programOptions->spbAtHour_Value=settings.value("spbAtHour_Value",0).toInt();
    programOptions->spbAtMinute_Value=settings.value("spbAtMinute_Value",0).toInt();
    settings.endGroup();
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::WindowStateChange){
        if(isMinimized() and programOptions->chbMinimizeToTray_Checked
                and dialog_open==false){ //minimize to tray
            this->hide();
            trayIcon->show();
        }
    }
    else{
        QMainWindow::changeEvent(event);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Enter or event->key()==Qt::Key_Return){ //start or stop timer by pressing Enter or Return key
        btnStartPressed();
    }
    else{
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //prevent hmtimer from closing when quit from tray icon
    this->show();
    trayIcon->hide();

    if(timer_enabled){
        if(QMessageBox::question(this,tr("Close hmtimer?"),
                                 tr("Timer is running. Do you really want to quit?"))==QMessageBox::No){
            event->ignore();
        }
        else{ //want to quit
            if(programOptions->chbPassword_Checked){
                PasswordDialog *dialog=new PasswordDialog(programOptions->password,this);
                if(dialog->exec()) event->accept(); //correct password
                else event->ignore();
                delete dialog;
            }
            else event->accept(); //don't need to enter password
        }
    }
    else{
        event->accept();
    }
}


void MainWindow::on_actionSettings_triggered()
{
    dialog_open=true;
    SettingsDialog *dlgSettings=new SettingsDialog(programOptions,this);
    dlgSettings->exec();
    delete dlgSettings;
    dialog_open=false;
}

void MainWindow::trayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason==QSystemTrayIcon::Trigger){
        this->showNormal();
        trayIcon->hide();
    }
}

void MainWindow::playerError(QMediaPlayer::Error error)
{
    stopTimer();
    //prevent multiple messageboxes
    disconnect(player,static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),0,0);
    qDebug()<<"player error: "<<error;
    QMessageBox::warning(this,tr("Audio Error"),
                         player->errorString());
}

void MainWindow::stopTimer()
{
    btnStart->setText(tr("Start"));
    timer->stop();
    spbHour->setReadOnly(false);
    spbMinute->setReadOnly(false);
    spbSecond->setReadOnly(false);
    comboAction->setEnabled(true);
    chbRunRepeatedly->setEnabled(true);
    on_chbRunRepeatedly_toggled(chbRunRepeatedly->isChecked());
    btnAt->setEnabled(true);
    trayIcon->setToolTip(tr("Hsiu-Ming's Timer"));
    timer_enabled=false;
}

void MainWindow::startTimer()
{
    timeStart=time(Q_NULLPTR);
    timeSet=spbHour->text().toInt()*3600+spbMinute->text().toInt()*60+spbSecond->text().toInt();
    timeRemain=timeSet;
    btnStart->setText(tr("Stop"));
    timer->start(200);
    spbHour->setReadOnly(true);
    spbMinute->setReadOnly(true);
    spbSecond->setReadOnly(true);
    comboAction->setEnabled(false);
    chbRunRepeatedly->setEnabled(false);
    chbRepeatTimes->setEnabled(false);
    spbRepeatTimes->setEnabled(false);
    labelRepeatTimes->setEnabled(false);
    btnAt->setEnabled(false);
    timer_enabled=true;
    //checking player state is required to play sound and run repeatedly
    if(comboAction->currentIndex()==4 and player->state()!=QMediaPlayer::PlayingState){
        player->setMedia(QUrl::fromLocalFile(audioFile));
        connect(player,static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
                this,&MainWindow::playerError);
    }
}

void MainWindow::playerMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status==QMediaPlayer::EndOfMedia and !timer_enabled){
        btnStart->setText(tr("Start"));
    }
}

void MainWindow::actShow_Triggered()
{
    this->showNormal();
    trayIcon->hide();
}

void MainWindow::actQuit_Triggered()
{
    this->close();
}

void MainWindow::on_actionProject_Page_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/cges30901/hmtimer"));
}

void MainWindow::on_actionDonate_triggered()
{
    QDesktopServices::openUrl(QUrl("https://cges30901.github.io/hmtimer-website/donate.html"));
}

void MainWindow::on_btnAt_clicked()
{
    dialog_open=true;
    AtDialog *dlgAt =new AtDialog(programOptions,this);
    if(dlgAt->exec()==QDialog::Accepted){
        startFromAt=true;
        setAtTime();
        startTimer();
    }
    delete dlgAt;
    dialog_open=false;
}

void MainWindow::setAtTime()
{
    int atTime=programOptions->spbAtHour_Value*3600+programOptions->spbAtMinute_Value*60;
    int currentTime=QTime::currentTime().msecsSinceStartOfDay()/1000;
    if(atTime<=currentTime)
        timeSet=86400-(currentTime-atTime);
    else
        timeSet=atTime-currentTime;
    spbHour->setValue(timeSet/3600);
    spbMinute->setValue(timeSet/60%60);
    spbSecond->setValue(timeSet%60);
}

void MainWindow::on_chbRepeatTimes_toggled(bool checked)
{
    spbRepeatTimes->setEnabled(checked);
    labelRepeatTimes->setEnabled(checked);
}

void MainWindow::on_chbRunRepeatedly_toggled(bool checked)
{
    if(!checked){
        chbRepeatTimes->setEnabled(false);
        spbRepeatTimes->setEnabled(false);
        labelRepeatTimes->setEnabled(false);
    }
    else{
        chbRepeatTimes->setEnabled(true);
        on_chbRepeatTimes_toggled(chbRepeatTimes->isChecked());
    }
}

void MainWindow::on_actionBlog_triggered()
{
    QDesktopServices::openUrl(QUrl("https://hsiu-ming.blogspot.com/"));
}

void MainWindow::on_comboAction_activated(int index)
{
    if(index==4){ //play sound
        dialog_open=true;
        QString file=QFileDialog::getOpenFileName(this,QString(),audioFile);
        if(file.length()!=0){
            audioFile=file;
        }
        dialog_open=false;
    }
    else if(index==5){ //run program
        dialog_open=true;
        SelectFileDialog *dlgSelectFile =new SelectFileDialog(programOptions,this);
        dlgSelectFile->exec();
        delete dlgSelectFile;
        dialog_open=false;
    }
}
