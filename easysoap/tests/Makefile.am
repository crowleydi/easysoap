noinst_PROGRAMS = interopclient interopserver
noinst_LIBRARIES = libSOAPBuildersInteropHandler.a

interopclient_SOURCES = interopclient.cpp
interopclient_LDADD = \
    libSOAPBuildersInteropHandler.a $(STATIC_EASYSOAP_LIBS) $(SSL_LIBS)
interopclient_DEPENDENCIES = \
    libSOAPBuildersInteropHandler.a $(STATIC_EASYSOAP_LIBS)

interopserver_SOURCES = interopserver.cpp
interopserver_LDADD = \
    libSOAPBuildersInteropHandler.a $(STATIC_EASYSOAP_LIBS) $(SSL_LIBS)
interopserver_DEPENDENCIES = \
    libSOAPBuildersInteropHandler.a $(STATIC_EASYSOAP_LIBS)

libSOAPBuildersInteropHandler_a_SOURCES = SOAPBuildersInteropHandler.cpp

INCLUDES = $(EASYSOAP_INCLUDES)

EXTRA_DIST = \
    interopstruct.h interop.xsl README.TXT SOAPBuildersInteropHandler.h
