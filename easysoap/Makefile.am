SUBDIRS = proj include src tests validator simpleserver google
EXTRA_DIST = \
	README \
	README.borland \
	easysoap.spec \
	conftools/config.guess \
	conftools/config.sub \
	conftools/install-sh \
	conftools/missing \
	conftools/mkinstalldirs \
	conftools/ltmain.sh

tests: src
validator: src
simpleserver: src
google: src

AM_CFLAGS=-Wall
AM_CXXFLAGS=-Wall
