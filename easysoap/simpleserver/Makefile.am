noinst_PROGRAMS = simpleserver simpleclient

simpleserver_SOURCES = \
	simpleserver.cpp

simpleserver_HEADERS = \
	calchandler.h

simpleclient_SOURCES = \
	simpleclient.cpp

LDADD = ../src/libeasysoap.la
LDFLAGS = -no-install
INCLUDES = -I@top_srcdir@/include

EXTRA_DIST = README.TXT simpleclient.dsp simpleserver.dsp \
	isapi.cpp isapi.def simpleisapi.cpp
