Summary:        A graphical shutdown timer
Name:           hmtimer
Version:        2.1
Release:        1
License:        GPL-3.0
Url:            https://sites.google.com/site/hsiumingstimer/
Group:          Productivity/Other
Source0:        %{name}-%{version}-src.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  update-desktop-files
BuildRequires:  pkgconfig(Qt5Multimedia)

%description
Hsiu-Ming's Timer is a graphical shutdown timer for Linux and Windows. It enables you to shutdown, turn off monitor, reboot or play sound after a period of time.

%prep
%setup -q

%build
qmake-qt5
make
g++ -o hmtimer_root hmtimer_root.cpp

%install
install -D -m0755 hmtimer %{buildroot}%{_bindir}/hmtimer
install -D -m0755 hmtimer_root %{buildroot}%{_bindir}/hmtimer_root
install -D -m0644 language/hmtimer_zh_TW.qm %{buildroot}%{_datadir}/hmtimer/hmtimer_zh_TW.qm
install -D -m0644 hmtimer.desktop %{buildroot}%{_datadir}/applications/hmtimer.desktop
install -D -m0644 hmtimer.png %{buildroot}%{_datadir}/icons/hicolor/128x128/apps/hmtimer.png
%suse_update_desktop_file %{name}

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}/hmtimer
%{_bindir}/hmtimer_root
%{_datadir}/hmtimer/hmtimer_zh_TW.qm
%{_datadir}/applications/hmtimer.desktop
%{_datadir}/icons/hicolor/128x128/apps/hmtimer.png
/usr/share/hmtimer
/usr/share/icons/hicolor
/usr/share/icons/hicolor/128x128
/usr/share/icons/hicolor/128x128/apps

%changelog
