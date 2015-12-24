QT-=core
QT-=gui

TARGET=hmtimer_root
CONFIG+=console
CONFIG-=app_bundle

TEMPLATE=app

SOURCES+=hmtimer_root.cpp

install_bin.files=hmtimer_root
install_bin.path=/usr/bin

INSTALLS+=install_bin
