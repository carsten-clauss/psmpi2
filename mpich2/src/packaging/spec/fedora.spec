Summary:	A high-performance implementation of MPI
Name:		mpich2
Version:	1.1.1
Release:	
License:	MIT
Group:		Development/Libraries
URL:		http://www.mcs.anl.gov/research/projects/mpich2
Source:		http://www.mcs.anl.gov/research/projects/mpich2/downloads/tarballs/%{version}%{release}/%{name}-%{version}%{release}.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:	libXt-devel, e2fsprogs-devel
BuildRequires:	java-devel-openjdk, gcc-gfortran
BuildRequires:	emacs-common, perl, python
Requires:	%{name}-libs = %{version}-%{release}
Requires:	python
Requires(post):	chkconfig
Requires(preun):chkconfig
#Requires chkconfig for /usr/sbin/alternatives

%description
MPICH2 is a high-performance and widely portable implementation of the
MPI standard. This release has all MPI-2.1 functions and features
required by the standard with the exeption of support for the
"external32" portable I/O format.

The mpich2 binaries in Fedora were configured to use the default
process manager 'MPD' using the default device 'ch3'. The ch3 device
was configured with support for the nemesis channel that allows for
shared-memory and TCP/IP sockets based communication.

The Fedora builds also include support for using '/usr/sbin/alternatives'
and/or the 'module environment' to select which MPI implementation to use
when multiple implementations are installed.

%package devel
Summary:	Development files for mpich2
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}
Requires:	pkgconfig
Requires:	gcc-gfortran
Requires(post):	/usr/sbin/alternatives
Requires(preun):/usr/sbin/alternatives
Requires(posttrans):/usr/sbin/alternatives

%description devel
Contains development headers and libraries for mpich2

%package libs
Summary:	Libraries and configuration files for mpich2
Group:		Development/Libraries

%description libs
Contains the arch dependent libraries and their build configuration for mpich2

# We only compile with gcc, but other people may want other compilers.
# Set the compiler here.
%{!?opt_cc: %global opt_cc gcc}
%{!?opt_fc: %global opt_fc gfortran}
# Optional CFLAGS to use with the specific compiler...gcc doesn't need any,
# so uncomment and undefine to NOT use
%{!?opt_cc_cflags: %global opt_cc_cflags %{optflags}}
%{!?opt_fc_fflags: %global opt_fc_fflags %{optflags}}

%ifarch %{ix86} x86_64
%global selected_channels ch3:nemesis
%else
%global selected_channels ch3:sock
%endif

%ifarch x86_64 ia64 ppc64 s390x sparc64
%global mode 64
%global priority 41
%else
%global mode 32
%global priority 40
%endif

%ifarch x86_64
%global XFLAGS -fPIC
%endif

%prep
%setup -q

%configure	\
	--enable-sharedlibs=gcc					\
	--enable-threads					\
	--with-device=%{selected_channels}			\
	--sysconfdir=%{_sysconfdir}/%{name}-%{mode}		\
	--includedir=%{_includedir}/%{name}			\
	--libdir=%{_libdir}/%{name}				\
	--datadir=%{_datadir}/%{name}				\
	--docdir=%{_docdir}/%{name}-%{version}			\
	--htmldir=%{_docdir}/%{name}-%{version}/www		\
	--with-java=%{_sysconfdir}/alternatives/java_sdk	\
	F90=%{opt_fc}						\
	F77=%{opt_fc}						\
	CFLAGS="%{?opt_cc_cflags} %{?XFLAGS}"			\
	CXXFLAGS="%{optflags} %{?XFLAGS}"			\
	F90FLAGS="%{?opt_fc_fflags} %{?XFLAGS}"			\
	FFLAGS="%{?opt_fc_fflags} %{?XFLAGS}"			\
	LDFLAGS='-Wl,-z,noexecstack'

%build
#make %{?_smp_mflags} doesn't work
make

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install

## Resolve clashes with LAM/OPENMP: mpicxx mpicc mpif77 mpif90 mpiexec mpirun
rm -f %{buildroot}%{_bindir}/{mpiexec,mpirun,mpdrun}
mv %{buildroot}%{_mandir}/man1/{,mp-}mpiexec.1
pushd  %{buildroot}%{_bindir}/
ln -s mpiexec.py mpdrun
touch mpiexec
touch mpirun
popd
for b in mpicxx mpicc mpif77 mpif90; do 
  mv %{buildroot}%{_bindir}/$b %{buildroot}%{_bindir}/mp%{mode}-$b;
  touch %{buildroot}%{_bindir}/$b;
  mv %{buildroot}%{_mandir}/man1/$b.1 %{buildroot}%{_mandir}/man1/mp-$b.1;
  touch %{buildroot}%{_mandir}/man1/$b.1;
done

## Setup the executables for 'environment module'
mkdir -p %{buildroot}%{_datadir}/%{name}/bin%{mode}
for b in mpicxx mpicc mpif77 mpif90; do 
  ln -s ../../../bin/mp%{mode}-$b %{buildroot}%{_datadir}/%{name}/bin%{mode}/$b
done
for ex in mpiexec mpirun; do 
  ln -s ../../../bin/mpiexec.py %{buildroot}%{_datadir}/%{name}/bin%{mode}/$ex
done

mv %{buildroot}%{_libdir}/%{name}/pkgconfig %{buildroot}%{_libdir}/
chmod -x %{buildroot}%{_libdir}/pkgconfig/*.pc

#Install the libdir under /etc/ld.so.conf.d
mkdir -p %{buildroot}%{_sysconfdir}/ld.so.conf.d
echo "%{_libdir}/%{name}"	\
	> %{buildroot}%{_sysconfdir}/ld.so.conf.d/%{name}-%{_arch}.conf

# Adjust the default 'environment module' PATH for our changes
sed -i 's#bin#bin'%{mode}'#' %{buildroot}%{_datadir}/%{name}/%{name}.module


# Manually copy doc file here instead of the %files section to prevent the rpm
#build script from throwing the other things in there out
cp -pr CHANGES COPYRIGHT README RELEASE_NOTES %{buildroot}%{_docdir}/%{name}-%{version}/
cp -pr src/mpe2/README %{buildroot}%{_docdir}/%{name}-%{version}/README.mpe

# Silence rpmlint
sed -i '/^#! \//,1 d' %{buildroot}%{_sysconfdir}/%{name}-%{mode}/{mpi*.conf,mpe_help.*}

#Rename the MPI routine manpages, to avoid conflict with openmpi
for man in %{buildroot}%{_mandir}/man3/*; do
  manf=`basename $man .3`
  mv %{buildroot}%{_mandir}/man3/$manf.3 %{buildroot}%{_mandir}/man4/$manf.4
  sed s/3/4/1 -i %{buildroot}%{_mandir}/man4/$manf.4
done

# The uninstall script that is installed in this directory is not needed in rpm
# packaging 
rm -rf %{buildroot}%{_sbindir}/mpe*

find %{buildroot} -type f -name "*.la" -exec rm -f {} ';'

%clean
rm -rf %{buildroot}

%post
if [ $1 -eq 1 ] ; then
/usr/sbin/alternatives	\
	--install %{_bindir}/mpirun mpi-run %{_bindir}/mpiexec.py 41	\
	--slave	%{_bindir}/mpiexec mpi-exec %{_bindir}/mpiexec.py	\
	--slave	%{_mandir}/man1/mpiexec.1.gz mpi-exec-man		\
			%{_mandir}/man1/mp-mpiexec.1.gz		\
	--slave	%{_mandir}/man1/mpif90.1.gz mpif90-man		\
			%{_mandir}/man1/mp-mpif90.1.gz		\
	--slave	%{_mandir}/man1/mpif77.1.gz mpif77-man		\
			%{_mandir}/man1/mp-mpif77.1.gz		\
	--slave	%{_mandir}/man1/mpicc.1.gz mpicc-man		\
			%{_mandir}/man1/mp-mpicc.1.gz		\
	--slave	%{_mandir}/man1/mpic++.1.gz mpic++-man		\
			%{_mandir}/man1/mp-mpicxx.1.gz
fi

%posttrans
if [ $1 -eq 0 ] ; then
/usr/sbin/alternatives	\
	--install %{_bindir}/mpirun mpi-run %{_bindir}/mpiexec.py 41	\
	--slave	%{_bindir}/mpiexec mpi-exec %{_bindir}/mpiexec.py	\
	--slave	%{_mandir}/man1/mpiexec.1.gz mpi-exec-man		\
			%{_mandir}/man1/mp-mpiexec.1.gz		\
	--slave	%{_mandir}/man1/mpif90.1.gz mpif90-man		\
			%{_mandir}/man1/mp-mpif90.1.gz		\
	--slave	%{_mandir}/man1/mpif77.1.gz mpif77-man		\
			%{_mandir}/man1/mp-mpif77.1.gz		\
	--slave	%{_mandir}/man1/mpicc.1.gz mpicc-man		\
			%{_mandir}/man1/mp-mpicc.1.gz		\
	--slave	%{_mandir}/man1/mpic++.1.gz mpic++-man		\
			%{_mandir}/man1/mp-mpicxx.1.gz
fi

%preun
if [ $1 -eq 0 ] ; then
/usr/sbin/alternatives --remove mpi-run %{_bindir}/mpiexec.py
fi

%post devel
if [ $1 -eq 1 ] ; then
/usr/sbin/alternatives	\
	--install %{_bindir}/mpicc mpicc %{_bindir}/mp%{mode}-mpicc %{priority}\
	--slave	%{_bindir}/mpicxx  mpicxx  %{_bindir}/mp%{mode}-mpicxx	\
	--slave	%{_bindir}/mpif90  mpif90  %{_bindir}/mp%{mode}-mpif90	\
	--slave	%{_bindir}/mpif77  mpif77  %{_bindir}/mp%{mode}-mpif77
fi

%posttrans devel
if [ $1 -eq 0 ] ; then
/usr/sbin/alternatives	\
	--install %{_bindir}/mpicc mpicc %{_bindir}/mp%{mode}-mpicc %{priority}\
	--slave	%{_bindir}/mpicxx  mpicxx  %{_bindir}/mp%{mode}-mpicxx	\
	--slave	%{_bindir}/mpif90  mpif90  %{_bindir}/mp%{mode}-mpif90	\
	--slave	%{_bindir}/mpif77  mpif77  %{_bindir}/mp%{mode}-mpif77
fi

%preun devel
if [ $1 -eq 0 ] ; then
/usr/sbin/alternatives --remove mpicc %{_bindir}/mp%{mode}-mpicc
fi

%post libs -p /sbin/ldconfig

%postun libs -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_bindir}/*
%dir %{_datadir}/%{name}
%dir %{_datadir}/%{name}/bin%{mode}
%{_datadir}/%{name}/bin%{mode}/mpiexec
%{_datadir}/%{name}/bin%{mode}/mpirun
%exclude %{_bindir}/mpiexec
%exclude %{_bindir}/mpirun
%exclude %{_bindir}/*mpicc
%exclude %{_bindir}/*mpicxx
%exclude %{_bindir}/*mpif90
%exclude %{_bindir}/*mpif77
%doc %{_docdir}/%{name}-%{version}/
%{_mandir}/man1/mp-*.1.gz
%if 0%{?fedora} < 11
%exclude %{_bindir}/mp*.pyc
%exclude %{_bindir}/mp*.pyo
%endif
%ghost %{_bindir}/mpiexec
%ghost %{_bindir}/mpirun
%ghost %{_mandir}/man1/mpi*.1.gz
%ghost %{_mandir}/man1/MPI.1.gz

%files libs
%defattr(-,root,root,-)
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/*.jar
%{_libdir}/%{name}/mpe*.o
%{_libdir}/%{name}/*.so.*
%{_sysconfdir}/ld.so.conf.d/%{name}-%{_arch}.conf
%config %{_sysconfdir}/%{name}-%{mode}/

%files devel
%defattr(-,root,root,-)
%{_bindir}/mp%{mode}*
%ghost %{_bindir}/mpicc
%ghost %{_bindir}/mpicxx
%ghost %{_bindir}/mpif90
%ghost %{_bindir}/mpif77
%{_includedir}/*
%{_libdir}/%{name}/*.a
%{_libdir}/%{name}/*.so
%{_libdir}/%{name}/trace_rlog/libTraceInput.so
%{_libdir}/pkgconfig/%{name}-ch3.pc
%{_datadir}/%{name}/%{name}.module
%{_datadir}/%{name}/bin%{mode}/mpicc
%{_datadir}/%{name}/bin%{mode}/mpicxx
%{_datadir}/%{name}/bin%{mode}/mpif90
%{_datadir}/%{name}/bin%{mode}/mpif77
%{_datadir}/%{name}/examples*
%{_datadir}/%{name}/logfiles/
%{_mandir}/man4/*.gz

%changelog
* Wed May 20 2009 Deji Akingunola <dakingun@gmail.com> - 1.1-1
- Update to 1.1

* Wed May 20 2009 Deji Akingunola <dakingun@gmail.com> - 1.1-0.4.rc1
- Install the libdir under /etc/ld.so.conf.d

* Mon May 18 2009 Deji Akingunola <dakingun@gmail.com> - 1.1-0.3.rc1
- Update to 1.1rc1
- Update spec to follow the proposed packaging guildelines wrt using alternatives
- Also change to use the global macro instead of define.

* Sun Mar 29 2009 Deji Akingunola <dakingun@gmail.com> - 1.1-0.2.b1
- Specifically build with openjdk Java, so Jumpshot works (Anthony Chan)

* Wed Mar 18 2009 Deji Akingunola <dakingun@gmail.com> - 1.1-0.1.b1
- Update for the 1.1 (beta) release
- Stop building with dllchan, it is not fully supported
- Fix un-owned directory (#490270)
- Add Posttrans scriplets to work around 1.0.8-3 scriplet brokenness

* Mon Mar 09 2009 Deji Akingunola <dakingun@gmail.com> - 1.0.8-3
- Drop the ssm channel from ppc* archs, it fails to build
- Python scripts in bindir and sbindir are no longer bytecompiled (F-11+)
- Enhance the spec file to support ia64 and sparc
- Include mpiexec and mpirun (symlinks) in the environment module bindir 

* Fri Mar 06 2009 Deji Akingunola <dakingun@gmail.com> - 1.0.8-2
- Fix the source url, pointed out from package review
- Finally accepted to go into Fedora

* Sat Oct 24 2008 Deji Akingunola <dakingun@gmail.com> - 1.0.8-1
- Update to the 1.0.8
- Configure with the default nemesis channel

* Fri May 16 2008 Deji Akingunola <dakingun@gmail.com> - 1.0.7-5
- Update the alternate compiler/compiler flags macro to allow overriding it
  from command-line

* Wed Apr 16 2008 Deji Akingunola <dakingun@gmail.com> - 1.0.7-4
- Apply patch from Orion Poplawski to silence rpmlint

* Tue Apr 15 2008 Deji Akingunola <dakingun@gmail.com> - 1.0.7-3
- Add a note on the device/channels configuration options used, and
- Fix logfile listings as suggested by Orion Poplawski (Package review, 171993)

* Tue Apr 15 2008 Deji Akingunola <dakingun@gmail.com> - 1.0.7-2
- Fix the source url

* Sat Apr 05 2008 Deji Akingunola <dakingun@gmail.com> - 1.0.7-1
- Update to 1.0.7

* Mon Oct 15 2007 Deji Akingunola <dakingun@gmail.com> - 1.0.6p1-1
- Update to 1.0.6p1

* Mon Oct 15 2007 Deji Akingunola <dakingun@gmail.com> - 1.0.6-1
- New version upgrade

* Mon Jul 31 2007 Deji Akingunola <dakingun@gmail.com> - 1.0.5p4-4
- Create a -mpi-manpages subpackage for the MPI routines manuals

* Fri Jul 27 2007 Deji Akingunola <dakingun@gmail.com> - 1.0.5p4-3
- Fix java-gcj-compat BR
- Handle upgrades in the post scripts

* Mon Jun 12 2007 Deji Akingunola <dakingun@gmail.com> - 1.0.5p4-2
- Fix typos and make other adjustments arising from Fedora package reviews

* Mon Jun 12 2007 Deji Akingunola <dakingun@gmail.com> - 1.0.5p4-1
- Patch #4 release

* Mon Feb 12 2007 Deji Akingunola <dakingun@gmail.com> - 1.0.5p2-1
- Patch #2 release

* Tue Jan 09 2007 Deji Akingunola <dakingun@gmail.com> - 1.0.5p1-1
- New release with manpages
- Create a -libs subpackage as it's done in Fedora's openmpi to help with
  multi-libs packaging
- Disable modules support (until I can properly figure it out)

* Wed Dec 27 2006 Deji Akingunola <dakingun@gmail.com> - 1.0.5-1
- New release

* Sat Nov 18 2006 Deji Akingunola <dakingun@gmail.com> - 1.0.4p1-2
- Set the java_sdk directory so all java bit work  

* Sat Sep 02 2006 Deji Akingunola <dakingun@gmail.com> - 1.0.4p1-1
- Update to version 1.0.4p1
- Cleanup up spec file to use alternatives similarly to FC's openmpi

* Wed Aug 02 2006 Deji Akingunola <dakingun@gmail.com> - 1.0.4-1
- Update to version 1.0.4

* Thu May 18 2006 Deji Akingunola <dakingun@gmail.com> - 1.0.3-3
- Add missing BRs (Orion Polawski)

* Mon Apr 10 2006 Deji Akingunola <dakingun@gmail.com> - 1.0.3-2
- Rewrite the spec, borrowing extensively from openmpi's spec by Jason Vas Dias
- Allows use of environment modules and alternatives

* Fri Nov 25 2005 Deji Akingunola <dakingun@gmail.com> - 1.0.3-1
- Update to new version

* Sat Oct 15 2005 Deji Akingunola <deji.aking@gmail.com> - 1.0.2p1-1
- Initial package
