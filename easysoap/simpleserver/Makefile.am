noinst_PROGRAMS = simpleserver simpleclient

simpleserver_SOURCES = \
	simpleserver.cpp

simpleclient_SOURCES = \
	simpleclient.cpp

LDADD = ../src/libeasysoap.la
LDFLAGS = -no-install
INCLUDES = -I@top_srcdir@/include

EXTRA_DIST = README.TXT
