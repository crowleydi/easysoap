noinst_PROGRAMS = simpleserver simpleclient

simpleserver_SOURCES = \
	simpleserver.cpp \
	calchandler.h

simpleclient_SOURCES = \
	simpleclient.cpp \
	calchandler.h

LDADD = ../src/libeasysoap.la
INCLUDES = -I@top_srcdir@/include

AM_CFLAGS=-Wall -pedantic
AM_CXXFLAGS=-Wall -pedantic

EXTRA_DIST = README.TXT \
	simpleisapi.cpp
