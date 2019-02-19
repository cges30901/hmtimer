#
# spec file for package hmtimer
#
# Copyright (c) 2019 Hsiu-Ming Chang <cges30901@gmail.com>
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
Version:        3.0.2
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
lrelease-qt5 src/language/hmtimer_*.ts
qmake-qt5
make
gzip hmtimer.1

%install
make INSTALL_ROOT="%{buildroot}" install
install -D -m0644 hmtimer.1.gz %{buildroot}%{_mandir}/man1/hmtimer.1.gz
%if 0%{?suse_version}
%suse_update_desktop_file -r io.github.cges30901.hmtimer Utility DesktopUtility
%endif

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%doc LICENSE
%{_bindir}/hmtimer
%{_datadir}/appdata/io.github.cges30901.hmtimer.appdata.xml
%{_datadir}/applications/io.github.cges30901.hmtimer.desktop
%{_datadir}/hmtimer/
%{_datadir}/icons/hicolor/
%{_mandir}/man1/hmtimer.1.gz

%changelog
