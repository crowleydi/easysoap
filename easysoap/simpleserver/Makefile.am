noinst_PROGRAMS = simpleserver simpleclient

simpleserver_SOURCES = simpleserver.cpp
simpleserver_DEPENDENCIES = $(STATIC_EASYSOAP_LIBS)
simpleserver_LDADD = $(STATIC_EASYSOAP_LIBS)

simpleclient_SOURCES = simpleclient.cpp
simpleclient_DEPENDENCIES = $(STATIC_EASYSOAP_LIBS)
simpleclient_LDADD = $(STATIC_EASYSOAP_LIBS)

INCLUDES = $(EASYSOAP_INCLUDES)

EXTRA_DIST = README.TXT
