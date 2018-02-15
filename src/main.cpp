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
#include <QApplication>
#include <QTranslator>
#include <QStringList>
#include <iostream>
#include "mainwindow.h"
#include "version.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    QStringList args=app.arguments();
    bool showWindow=true;
    for(int i=0;i<args.size();i++){
        if(args.at(i)=="--help"){
            std::cout<<"Usage: hmtimer [-t time] [-s] [-m] [-a ACTION] [--help] [--version]\n"
                     <<"options:\n"
                     <<"  -t SECONDS   set seconds and start timer\n"
                     <<"  -m           minimize to tray\n"
                     <<"  -a ACTION    set action when time is up.\n"
                     <<"    available action: monitor, shutdown, reboot, sound, runprogram\n"
                     <<"  --help       display help and exit\n"
                     <<"  --version    display version and exit\n";
            exit(0);
        }
        else if(args.at(i)=="--version"){
            std::cout<<VERSION<<"\n";
            exit(0);
        }
        else if(args.at(i)=="-m"){
            showWindow=false;
        }
    }
    QTranslator translator;
    if(translator.load("hmtimer_"+QLocale::system().name(),app.applicationDirPath()+"/language")!=true){
        translator.load("hmtimer_"+QLocale::system().name(),"/usr/share/hmtimer");
    }
    app.installTranslator(&translator);
    MainWindow *window=new MainWindow;
    if(showWindow==true){
        window->show();
    }
    return app.exec();
}
