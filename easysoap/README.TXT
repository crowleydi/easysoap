

Pre-Alpha release. 0.1.1
=====================================

The Expat XML parsing library 1.95.1 is required.  You can download it
from expat.sourceforge.net. See below.

To run the tests program you will need to install  SOAP::Lite.
The soap.daemon must be running on localhost. You may have to edit
soap.daemon so you can access the example SOAP classes in /My/Examples.
On linux, I had to install the ActiveState 5.6 perl distribution from
www.activestate.com.

Please submit bug reports and comments to:
dcrowley@users.sourceforge.net.


HOW TO BUILD EASYSOAP
=====================================

1) Install Expat (see below)
2) If you're on Linux:
	cd easysoap
	make
3) If you're on Windows, VC6:
	Load easysoap.dsw.  Build All.


HOW TO DOWNLOAD AND BUILD EXPAT
=====================================

Download and untar source from:
	http://download.sourceforge.net/expat/expat-1.95.1.tar.gz

For Linux :
	cd expat
	./configure
	(SEE NOTE BELOW FOR GCC USERS)
	make
	make install (must be root to install into /usr/local)


For Win32 :
	Load .dsw and build.
	Copy expat.lib to easysoap/win32/Debug
	Copy expat.dll to easysoap/win32/Debug


COMPLILING EXPAT WITH GCC
=========================

Because EasySoap++ uses exceptions, the Expat library must be compiled
to handle exceptions correctly or crashes will occur when a C++ exception
is thrown.  After you run the configure command you must edit lib/Makefile
and add the flag "-fexceptions" to the CFLAGS proeprty.


