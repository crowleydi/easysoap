noinst_PROGRAMS = interopserver interopclient

interopserver_SOURCES = \
	interopstruct.h \
	SOAPBuildersInteropHandler.h \
	SOAPBuildersInteropHandler.cpp \
	SOAPBuildersInteropHandlerA.cpp \
	SOAPBuildersInteropHandlerB.cpp \
	SOAPBuildersHeaderHandler.h \
	SOAPBuildersHeaderHandler.cpp \
	interopserver.cpp

interopclient_SOURCES = \
	interopclient.cpp \
	interopstruct.h

LDADD = ../src/libeasysoap.la
LD_FLAGS = -no-install
INCLUDES = -I@top_srcdir@/include

EXTRA_DIST = README.TXT interopclient.dsp interopserver.dsp
