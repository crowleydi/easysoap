noinst_PROGRAMS = interopserver interopclient

interopserver_SOURCES = \
	interopserver.cpp \
	SOAPBuildersInteropHandler.cpp \
	SOAPBuildersInteropHandler.h \
	interopstruct.h

interopclient_SOURCES = \
	interopclient.cpp \
	interopstruct.h

LDADD = ../src/libeasysoap.la
LD_FLAGS = -no-install
INCLUDES = -I@top_srcdir@/include

EXTRA_DIST = README.TXT interopclient.dsp interopserver.dsp
