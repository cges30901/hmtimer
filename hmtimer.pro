######################################################################
# Automatically generated by qmake (3.0) ??? ?? 16 15:15:20 2014
######################################################################

TEMPLATE = subdirs
SUBDIRS=src

install_desktop.files=io.github.cges30901.hmtimer.desktop
CONFIG(flatpak):install_desktop.path=/app/share/applications
else:install_desktop.path=/usr/share/applications

install_appdata.files=io.github.cges30901.hmtimer.appdata.xml
CONFIG(flatpak):install_appdata.path=/app/share/appdata
else:install_appdata.path=/usr/share/appdata

INSTALLS+=install_desktop install_appdata
