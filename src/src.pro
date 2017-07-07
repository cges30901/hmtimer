######################################################################
# Automatically generated by qmake (3.0) ??? ?? 16 15:15:20 2014
######################################################################

TEMPLATE = app
TARGET = hmtimer
INCLUDEPATH += .
QT+=widgets
QT += multimedia
!win32{
QT += dbus
}

# Input
FORMS += mainwindow.ui \
    selectfiledialog.ui \
    settingsdialog.ui \
    newpassworddialog.ui \
    passworddialog.ui
SOURCES += main.cpp \
    mainwindow.cpp \
    selectfiledialog.cpp \
    settingsdialog.cpp \
    newpassworddialog.cpp \
    passworddialog.cpp

HEADERS += \
    mainwindow.h \
    programoptions.h \
    selectfiledialog.h \
    settingsdialog.h \
    version.h \
    newpassworddialog.h \
    passworddialog.h

TRANSLATIONS=language/hmtimer.ts \
    language/hmtimer_zh_TW.ts \
    language/hmtimer_ar.ts \
    language/hmtimer_ru.ts

RESOURCES=hmtimer.qrc

install_bin.files=hmtimer
install_bin.path=/usr/bin

install_translation.files=language/hmtimer_zh_TW.qm \
    language/hmtimer_ar.qm \
    language/hmtimer_ru.qm
install_translation.path=/usr/share/hmtimer

install_icon.files=hmtimer.png
install_icon.path=/usr/share/icons/hicolor/128x128/apps

INSTALLS+=install_bin install_translation install_icon

win32:RC_ICONS += hmtimer.ico