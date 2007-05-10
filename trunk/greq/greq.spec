# Note that this is NOT a relocatable package
%define ver      0.9.4
%define rel      1
%define prefix   @PREFIX@

Summary:   A GTK+ shell tool
Name:      greq
Version:   %ver
Release:   %rel
Copyright: GPL
Group:     Applications/X11
Source:    greq-%{PACKAGE_VERSION}.tar.bz2
BuildRoot: /tmp/greq-%{PACKAGE_VERSION}-root
Packager:  Felix Braun <Hazzl@developer.berlios.de>

%description
A program that lets you build GTK+-dialogs from within a shell script. This
is useful if you are writing shell scripts that need to get input from a
user by way of a GUI.

%prep
%setup

%build
make CFLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{prefix}/bin
mkdir -p $RPM_BUILD_ROOT%{_mandir}/man1

make prefix=$RPM_BUILD_ROOT%{prefix} mandir=$RPM_BUILD_ROOT%{_mandir}/man1 install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)

%doc COPYING ChangeLog TODO Demo
%{prefix}/bin/*
%{_mandir}/man1/*
