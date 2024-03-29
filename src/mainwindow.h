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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QMediaPlayer>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ui_mainwindow.h"
#include "programoptions.h"
#include <QProcess>
#include <QKeyEvent>
#include <QCloseEvent>


class MainWindow : public QMainWindow,public Ui::MainWindow
{
    Q_OBJECT
    QMediaPlayer *player;
    QMediaPlayer *beepPlayer;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QAudioOutput *audioOutput;
    QAudioOutput *beepAudioOutput;
#endif

    QTimer *timer;
    int timeSet;
    int timeRemain;
    time_t timeStart;
    bool timer_enabled;
    QString audioFile;
    QString filename;
    QSystemTrayIcon *trayIcon;
    QMenu *trayContextMenu;
    QAction *actQuit;
    QAction *actShow;
    ProgramOptions *programOptions;
    QProcess *process;
    bool startFromAt;
    //If a dialog is open when I minimize to tray,
    //tray icon cannot be activated. So I
    //disable minimize to tray if a dialog is open.
    bool dialog_open;

    void action();
    void readSettings();
    void setAtTime();
public:
    MainWindow(QWidget *parent=Q_NULLPTR);
protected:
    void changeEvent(QEvent *);
    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *event);
private slots:
    void timer_timeout();
    void btnStartPressed();
    void writeSettings();
    void trayIcon_activated(QSystemTrayIcon::ActivationReason reason);
    void playerError(QMediaPlayer::Error error);
    void stopTimer();
    void startTimer();
    void playerMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void actShow_Triggered();
    void actQuit_Triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionAbout_triggered();
    void on_actionWebsite_triggered();
    void on_actionSettings_triggered();
    void on_actionProject_Page_triggered();
    void on_actionDonate_triggered();
    void on_btnAt_clicked();
    void on_chbRepeatTimes_toggled(bool checked);
    void on_chbRunRepeatedly_toggled(bool checked);
    void on_actionBlog_triggered();
    void on_comboAction_activated(int index);
};

#endif // MAINWINDOW_H
