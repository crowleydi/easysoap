noinst_PROGRAMS = validator

validator_SOURCES = \
    validator.cpp \
	UserlandValidatorHandler.cpp \
	UserlandValidatorHandler.h \
	WhichToolkitHandler.cpp  \
	WhichToolkitHandler.h 

LDADD = ../src/libeasysoap.la
LDFLAGS = -no-install
INCLUDES = -I@top_srcdir@/include

EXTRA_DIST = README.TXT validator.dsp
