Name:           qt5docgallery
Version:        5.0.0
Release:        1
License:        LGPL
Summary:        Qt5 AddOn DocGallery module
Url:            http://qt.nokia.com/
Source:         %name.tar.bz2
BuildRequires:  libqt5base-devel
BuildRequires:  libqt5declarative-devel
BuildRequires:  libqt5script-devel
BuildRequires:  qt5jsondb-devel
BuildRequires:  mt-qt5-lib-devel
BuildRoot:      %{_tmppath}/%{name}-%{version}-build


%description
The Qt 5 AddOn DocGallery module

%package -n libqt5docgallery
Summary:        Qt 5 DocGallery library
Group:          System/Libraries
%description -n libqt5docgallery
Qt 5 DocGallery libraries

%package -n libqt5docgallery-devel
Summary:        Qt 5 DocGallery development files
Group:          System/Libraries
Requires:       libqt5docgallery
%description -n libqt5docgallery-devel
Qt 5 DocGallery development files


%prep
%setup -q -n %{name}

%build
# point to the location where the mkspecs can be found
export QTDIR=/usr/share/qt5 && qmake
make %{?_smp_mflags}

%install
make install INSTALL_ROOT=%{buildroot}

# Fix wrong path in pkgconfig files
find %{buildroot}%{_libdir}/pkgconfig -type f -name '*.pc' \
-exec perl -pi -e "s, -L%{_builddir}/%{name}/?\S+,,g" {} \;

# Fix wrong path in prl files
find %{buildroot}%{_libdir} -type f -name '*.prl' \
-exec sed -i -e "/^QMAKE_PRL_BUILD_DIR/d;s/\(QMAKE_PRL_LIBS =\).*/\1/" {} \;

rm -rf %{buildroot}%{_prefix}/tests
rm -f %{buildroot}%{_libdir}/libQt*.la

%clean


%post -n libqt5docgallery -p /sbin/ldconfig

%postun -n libqt5docgallery -p /sbin/ldconfig


%files -n libqt5docgallery
%defattr(-,root,root)
%{_libdir}/libQtAddOnGallery.so.*
%{_libdir}/qt5/imports/QtAddOn/gallery

%files -n libqt5docgallery-devel
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/*.so
%{_datadir}/qt5/mkspecs/*
%{_libdir}/pkgconfig/*

%changelog
