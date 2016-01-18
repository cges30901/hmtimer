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
