#include <QApplication>
#include <QTranslator>
#include <QStringList>
#include <iostream>
#include "mainwindow.h"
#include "selectfiledialog.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    QStringList args=app.arguments();
    for(int i=0;i<args.size();i++){
        if(args.at(i)=="--help"){
            std::cout<<"Usage: hmtimer [-t time] [-s] [-m] [-a ACTION]\n"
                     <<"options:\n"
                     <<"  -t SECONDS   set seconds\n"
                     <<"  -s           start timer\n"
                     <<"  -m           minimize to tray(may not work on Linux)\n"
                     <<"  -a ACTION    set action when time is up\n"
                     <<"    ACTION:monitor, shutdown, reboot, sound, runprogram\n";
            exit(0);
        }
    }
    QTranslator translator;
    translator.load("hmtimer_"+QLocale::system().name(),app.applicationDirPath()+"/language");
    app.installTranslator(&translator);
    MainWindow *window=new MainWindow;
    window->show();
    for(int i=0;i<args.size();i++){
        if(args.at(i)=="-m"){
            window->hide();
        }
    }
    return app.exec();
}
