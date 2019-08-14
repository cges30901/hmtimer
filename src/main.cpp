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
#include <QLibraryInfo>
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
            std::cout<<"Usage: hmtimer [-t TIME] [-at HH:MM] [-m] [-a ACTION] [--help] [--version]\n"
                     <<"options:\n"
                     <<"  -t TIME      set time from now and start timer\n"
                     <<"    Available formats for time are:\n"
                     <<"    1. seconds\n"
                     <<"    2. MM:SS, maximum value for SS is 59\n"
                     <<"    3. HH:MM:SS, maximum value for MM and SS is 59\n"
                     <<"  -at HH:MM      set absolute time and start timer\n"
                     <<"  -m           minimize to tray\n"
                     <<"  -a ACTION    set action when time is up.\n"
                     <<"    available actions: nothing, monitor, shutdown, reboot, sound, runprogram\n"
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

    QTranslator translator, qtTranslator;
#ifdef FLATPAK
    translator.load(QLocale(),"hmtimer","_","/app/share/hmtimer");
    qtTranslator.load(QLocale(),"qt","_",QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#else
    if(translator.load(QLocale(),"hmtimer","_",app.applicationDirPath()+"/language")!=true){
        translator.load(QLocale(),"hmtimer","_","/usr/share/hmtimer");
    }

    if(qtTranslator.load(QLocale(),"qt","_",app.applicationDirPath()+"/language")!=true){
        qtTranslator.load(QLocale(),"qt","_",QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    }
#endif
    app.installTranslator(&translator);
    app.installTranslator(&qtTranslator);

    MainWindow *window=new MainWindow;
    if(showWindow==true){
        window->show();
    }
    return app.exec();
}
