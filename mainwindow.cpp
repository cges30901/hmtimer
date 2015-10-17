#include "mainwindow.h"
#include <time.h>
#include <QMediaPlayer>
#include <QFileInfo>
#include <QFileDialog>
#include "selectfiledialog.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <settingsdialog.h>
#ifdef Q_OS_LINUX
 #include <linux/kd.h>
 #include <sys/ioctl.h>
 #include <fcntl.h>
#endif
#ifdef Q_OS_WIN32
 #include <windows.h>
#endif
#include <stdio.h>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    programOptions=new ProgramOptions;
    player=new QMediaPlayer;
    timer=new QTimer(this);
    timer_enabled=false;
    trayIcon=new QSystemTrayIcon;
    trayIcon->setIcon(QIcon(":/hmtimer.png"));
    trayIcon->setToolTip("Hsiu-Ming's Timer");
    trayContextMenu=new QMenu;
    actShow=trayContextMenu->addAction(tr("Show"));
    actQuit=trayContextMenu->addAction(tr("Quit"));
    trayIcon->setContextMenu(trayContextMenu);
    connect(btnStart,SIGNAL(clicked()),
            this,SLOT(buttonStartPressed()));
    connect(timer,SIGNAL(timeout()),
            this,SLOT(timer_timeout()));
    connect(spbSecond,SIGNAL(valueChanged(int)),
            this,SLOT(spbSecond_valueChanged(int)));
    connect(rbtSound,SIGNAL(clicked()),
            this,SLOT(setAudioFile()));
    connect(qApp,SIGNAL(aboutToQuit()),
            this,SLOT(writeSettings()));
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayIcon_activated(QSystemTrayIcon::ActivationReason)));
    connect(actShow,SIGNAL(triggered(bool)),
            this,SLOT(actShow_Triggered()));
    connect(actQuit,SIGNAL(triggered(bool)),
            this,SLOT(actQuit_Triggered()));
#ifdef Q_OS_LINUX
    write_fp=popen((QString("pkexec ")+qApp->applicationDirPath()+"/root").toLocal8Bit().data(),"w");
#endif
    readSettings();
    QStringList args=qApp->arguments();
    for(int i=0;i<args.size();i++){
        if(args.at(i)=="-a"){
            i++;
            if(args.at(i)=="monitor"){
                rbtMonitor->setChecked(true);
            }
            if(args.at(i)=="shutdown"){
                rbtShutdown->setChecked(true);
            }
            if(args.at(i)=="reboot"){
                rbtReboot->setChecked(true);
            }
            if(args.at(i)=="sound"){
                rbtSound->setChecked(true);
            }
            if(args.at(i)=="runprogram"){
                rbtRunprogram->setChecked(true);
            }
        }
        else if(args.at(i)=="-t"){
            i++;
            QString time=args.at(i);
            spbHour->setValue(time.toInt()/3600);
            spbMinute->setValue(time.toInt()/60%60);
            spbSecond->setValue(time.toInt()%60);
        }
        else if(args.at(i)=="-m"){
            trayIcon->show();
        }
    }
    for(int i=0;i<args.size();i++){
        if(args.at(i)=="-s"){
            buttonStartPressed();
        }
    }
}
void MainWindow::buttonStartPressed()
{
    if(timer_enabled)
    {
        //stop timer
        btnStart->setText(tr("Start"));
        timer->stop();
        spbHour->setReadOnly(false);
        spbMinute->setReadOnly(false);
        spbSecond->setReadOnly(false);
        rbtMonitor->setEnabled(true);
        rbtShutdown->setEnabled(true);
        rbtReboot->setEnabled(true);
        rbtSound->setEnabled(true);
        rbtRunprogram->setEnabled(true);
        trayIcon->setToolTip("Hsiu-Ming's Timer");
        timer_enabled=!timer_enabled;
    }
    else if(spbHour->value()==0 and spbMinute->value()==0 and spbSecond->value()==0)
    {
        QMessageBox::warning(this,tr("Please set the timer"),
                                 tr("Please set the timer"));
    }
    else
    {
        //start timer
        startTime=time(NULL);
        setTime=spbHour->text().toInt()*3600+spbMinute->text().toInt()*60+spbSecond->text().toInt();
        btnStart->setText(tr("Stop"));
        timer->start(200);
        spbHour->setReadOnly(true);
        spbMinute->setReadOnly(true);
        spbSecond->setReadOnly(true);
        rbtMonitor->setEnabled(false);
        rbtShutdown->setEnabled(false);
        rbtReboot->setEnabled(false);
        rbtSound->setEnabled(false);
        rbtRunprogram->setEnabled(false);
        timer_enabled=!timer_enabled;
    }
}
void MainWindow::timer_timeout()
{
    spbHour->setValue((setTime-int(difftime(time(NULL),startTime)))/3600);
    spbMinute->setValue((setTime-int(difftime(time(NULL),startTime)))/60%60);
    //if the value of spbSecond is changed, spbSecond_valueChanged() is called
    spbSecond->setValue((setTime-int(difftime(time(NULL),startTime)))%60);
    trayIcon->setToolTip("Hsiu-Ming's Timer\n"+spbHour->text()+':'+spbMinute->text()+':'+spbSecond->text());
}

void MainWindow::action()
{
    if(rbtSound->isChecked()){
        buttonStartPressed();
        player->setMedia(QUrl::fromLocalFile(audioFile));
        player->play();
    }
    else if(rbtShutdown->isChecked()){
        buttonStartPressed();
        writeSettings();
#ifdef Q_OS_LINUX
        fwrite("shutdown\n",sizeof(char),strlen("shutdown\n"),write_fp);
        fflush(write_fp);
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
    else if(rbtReboot->isChecked()){
        buttonStartPressed();
        writeSettings();
#ifdef Q_OS_LINUX
        fwrite("reboot\n",sizeof(char),strlen("reboot\n"),write_fp);
        fflush(write_fp);
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
    else if (rbtMonitor->isChecked()){
        buttonStartPressed();
#ifdef Q_OS_LINUX
        system("xset dpms force off");
#endif
#ifdef Q_OS_WIN32
        SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) 2);
#endif
    }
    else if (rbtRunprogram->isChecked()){
        buttonStartPressed();
        process=new QProcess;
        process->start(programOptions->lneFilename_Text,QStringList(programOptions->lneParameters_Text));
    }
    if(chbRunRepeatedly->isChecked()){
        spbHour->setValue(setTime/3600);
        spbMinute->setValue(setTime/60%60);
        spbSecond->setValue(setTime%60);
        buttonStartPressed();
    }
}

void MainWindow::spbSecond_valueChanged(int sec)
{
    if(timer_enabled){
        if(sec==0 and spbMinute->value()==0 and spbHour->value()==0){
            action();
        }
        else if(programOptions->chbBeep_Checked==true and (spbHour->value()*3600+spbMinute->value()*60+sec)<programOptions->spbBeep_Value){
            if(programOptions->chbAudioBeep_Checked==false)
            {
#ifdef Q_OS_LINUX
                fwrite("beep\n",sizeof(char),strlen("beep\n"),write_fp);
                fflush(write_fp);
#endif
#ifdef Q_OS_WIN32
                Beep(750,200);
#endif
            }
            else{
                player->setMedia(QUrl("qrc:/beep.ogg"));
                //player->setMedia(QUrl::fromLocalFile(qApp->applicationDirPath()+"/beep.ogg"));
                player->setVolume(100);
                player->play();
            }
        }
    }
}

void MainWindow::setAudioFile()
{
    QString file=QFileDialog::getOpenFileName(this,QString(),audioFile);
    if(file.length()!=0){
        audioFile=file;
    }
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}

void MainWindow::on_actionAbout_triggered()
{
    QString version="2.1";
    QMessageBox::about(this,tr("About"),tr("<h3>Hsiu-Ming's Timer %1</h3><br>"
                                           "Author: Hsiu-Ming Chang<br>"
                                           "e-mail: cges30901@gmail.com<br>"
                                           "License:GPL v3").arg(version));
}

void MainWindow::on_actionHomepage_triggered()
{
    QDesktopServices::openUrl(QUrl("https://sites.google.com/site/hsiumingstimer/"));
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
    settings.setValue("monitor",rbtMonitor->isChecked());
    settings.setValue("shutdown",rbtShutdown->isChecked());
    settings.setValue("reboot",rbtReboot->isChecked());
    settings.setValue("sound",rbtSound->isChecked());
    settings.setValue("runprogram",rbtRunprogram->isChecked());
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

    settings.setValue("chbStartup_Checked",programOptions->chbStartup_Checked);
    settings.setValue("chbMinimizeOnStartup_Checked",programOptions->chbMinimizeOnStartup_Checked);
    settings.setValue("chbStartTimerOnStartup_Checked",programOptions->chbStartTimerOnStartup_Checked);
    settings.setValue("spbStartupHour_Value",programOptions->spbStartupHour_Value);
    settings.setValue("spbStartupMinute_Value",programOptions->spbStartupMinute_Value);
    settings.setValue("spbStartupSecond_Value",programOptions->spbStartupSecond_Value);
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
    chbRunRepeatedly->setChecked(settings.value("chbRunRepeatedly",false).toBool());
    rbtMonitor->setChecked(settings.value("monitor").toBool());
    rbtShutdown->setChecked(settings.value("shutdown").toBool());
    rbtReboot->setChecked(settings.value("reboot").toBool());
    rbtSound->setChecked(settings.value("sound").toBool());
    rbtRunprogram->setChecked(settings.value("runprogram").toBool());
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
    programOptions->chbAudioBeep_Checked=settings.value("chbAudioBeep",false).toBool();

    programOptions->chbStartup_Checked=settings.value("chbStartup_Checked",false).toBool();
    programOptions->chbMinimizeOnStartup_Checked=settings.value("chbMinimizeOnStartup_Checked",false).toBool();
    programOptions->chbStartTimerOnStartup_Checked=settings.value("chbStartTimerOnStartup_Checked",false).toBool();
    programOptions->spbStartupHour_Value=settings.value("spbStartupHour_Value",false).toInt();
    programOptions->spbStartupMinute_Value=settings.value("spbStartupMinute_Value",false).toInt();
    programOptions->spbStartupSecond_Value=settings.value("spbStartupSecond_Value",false).toInt();
    settings.endGroup();
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::WindowStateChange){
        if(isMinimized() and programOptions->chbMinimizeToTray_Checked){
            this->hide();
            trayIcon->show();
        }
    }
    else{
        QMainWindow::changeEvent(event);
    }
}


void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *dlgSettings=new SettingsDialog(programOptions,this);
    dlgSettings->exec();
    delete dlgSettings;
}


void MainWindow::on_rbtRunprogram_clicked()
{
    SelectFileDialog *dlgSelectFile =new SelectFileDialog(programOptions,this);
    dlgSelectFile->exec();
    delete dlgSelectFile;
}

void MainWindow::trayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason==3){
        this->show();
        trayIcon->hide();
    }
}
void MainWindow::actShow_Triggered()
{
    this->show();
    trayIcon->hide();
}

void MainWindow::actQuit_Triggered()
{
    this->close();
}

void MainWindow::on_actionProject_Page_triggered()
{
    QDesktopServices::openUrl(QUrl("https://sourceforge.net/projects/hsiumingstimer/"));
}

void MainWindow::on_actionDonate_triggered()
{
    QDesktopServices::openUrl(QUrl("https://sites.google.com/site/hsiumingstimer/donate"));
}