#
# spec file for package hmtimer
#
# Copyright (c) 2018 Hsiu-Ming Chang <cges30901@gmail.com>
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

Summary:        A graphical shutdown timer
Name:           hmtimer
Version:        2.6.2
Release:        1
License:        GPL-3.0
Url:            https://cges30901.github.io/hmtimer-website/
Group:          Productivity/Other
Source0:        %{name}-%{version}-src.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Multimedia)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  qt5-linguist
%if 0%{?suse_version}
BuildRequires:  update-desktop-files
%endif

%description
Hsiu-Ming's Timer is a graphical shutdown timer for Linux and Windows. It enables you to shutdown, turn off monitor, reboot or play sound after a period of time.

%global debug_package %{nil}

%prep
%setup -q

%build
qmake-qt5
make
gzip hmtimer.1
lrelease-qt5 src/language/hmtimer_*.ts

%install
install -D -m0755 src/hmtimer %{buildroot}%{_bindir}/hmtimer
install -D -m0644 -t %{buildroot}%{_datadir}/hmtimer src/language/hmtimer_*.qm
install -D -m0644 hmtimer.desktop %{buildroot}%{_datadir}/applications/hmtimer.desktop
install -D -m0644 src/hmtimer.png %{buildroot}%{_datadir}/icons/hicolor/128x128/apps/hmtimer.png
install -D -m0644 src/hmtimer48.png %{buildroot}%{_datadir}/icons/hicolor/48x48/apps/hmtimer.png
install -D -m0644 hmtimer.1.gz %{buildroot}%{_mandir}/man1/hmtimer.1.gz
%if 0%{?suse_version}
%suse_update_desktop_file -r %{name} Utility DesktopUtility
%endif

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%doc LICENSE
%{_bindir}/hmtimer
%{_datadir}/applications/hmtimer.desktop
%{_datadir}/hmtimer/
%{_datadir}/icons/hicolor/
%{_mandir}/man1/hmtimer.1.gz

%changelog
