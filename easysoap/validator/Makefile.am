noinst_PROGRAMS = validator

validator_SOURCES = \
    validator.cpp \
	UserlandValidatorHandler.cpp \
	UserlandValidatorHandler.h \
	WhichToolkitHandler.cpp  \
	WhichToolkitHandler.h 

LDADD = ../src/libeasysoap.la
INCLUDES = -I@top_srcdir@/include

AM_CFLAGS=-Wall -pedantic
AM_CXXFLAGS=-Wall -pedantic

EXTRA_DIST = README.TXT
