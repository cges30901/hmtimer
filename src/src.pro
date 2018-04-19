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
    passworddialog.ui \
    atdialog.ui
SOURCES += main.cpp \
    mainwindow.cpp \
    selectfiledialog.cpp \
    settingsdialog.cpp \
    newpassworddialog.cpp \
    passworddialog.cpp \
    atdialog.cpp \

HEADERS += \
    mainwindow.h \
    programoptions.h \
    selectfiledialog.h \
    settingsdialog.h \
    version.h \
    newpassworddialog.h \
    passworddialog.h \
    atdialog.h \

TRANSLATIONS=language/hmtimer.ts \
    language/hmtimer_zh_TW.ts \
    language/hmtimer_ar.ts \
    language/hmtimer_ru.ts \
    language/hmtimer_be.ts \
    language/hmtimer_cs.ts \
    language/hmtimer_de.ts

RESOURCES=hmtimer.qrc

install_bin.files=hmtimer
install_bin.path=/usr/bin

install_translation.files=language/hmtimer_*.qm
install_translation.path=/usr/share/hmtimer

install_icon128.files=icon/128x128/hmtimer.png
install_icon128.path=/usr/share/icons/hicolor/128x128/apps

install_icon64.files=icon/64x64/hmtimer.png
install_icon64.path=/usr/share/icons/hicolor/64x64/apps

install_icon48.files=icon/48x48/hmtimer.png
install_icon48.path=/usr/share/icons/hicolor/48x48/apps

INSTALLS+=install_bin install_translation install_icon128 install_icon64 install_icon48

win32:RC_ICONS += hmtimer.ico
