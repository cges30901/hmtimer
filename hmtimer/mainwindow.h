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
#include <QStringList>
#include "ui_mainwindow.h"
#include "selectfiledialog.h"
#include "settingsdialog.h"
#include "programoptions.h"
#include <QProcess>
#include <QKeyEvent>


class MainWindow : public QMainWindow,public Ui::MainWindow
{
    Q_OBJECT
    QMediaPlayer *player;
    QMediaPlayer *beepPlayer;
    QTimer *timer;
    int setTime;
    time_t startTime;
    bool timer_enabled;
    QString audioFile;
    QString filename;
    QSystemTrayIcon *trayIcon;
    QMenu *trayContextMenu;
    QAction *actQuit;
    QAction *actShow;
    ProgramOptions *programOptions;
    QProcess *process;

    void action();
    void readSettings();
public:
    MainWindow(QWidget *parent=0);
protected:
    void changeEvent(QEvent *);
    void keyPressEvent(QKeyEvent *);
private slots:
    void timer_timeout();
    void btnStartPressed();
    void spbSecond_valueChanged(int sec);
    void setAudioFile();
    void writeSettings();
    void trayIcon_activated(QSystemTrayIcon::ActivationReason reason);
    void playerError(QMediaPlayer::Error error);
    void actShow_Triggered();
    void actQuit_Triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionAbout_triggered();
    void on_actionHomepage_triggered();
    void on_actionSettings_triggered();
    void on_rbtRunprogram_clicked();
    void on_actionProject_Page_triggered();
    void on_actionDonate_triggered();
};

#endif // MAINWINDOW_H
