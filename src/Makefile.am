lib_LTLIBRARIES = libeasysoap.la
lib_LIBRARIES = libeasysoap.a
THESOURCES = \
	digcalc.c \
	md5c.c \
	SOAPBase64.cpp \
	SOAPBody.cpp \
	SOAPBodyHandler.cpp \
	SOAPCGIServer.cpp \
	SOAPCGITransport.cpp \
	SOAPClientSocketImp.cpp \
	SOAPDebugger.cpp \
	SOAPEnvelope.cpp \
	SOAPEnvelopeHandler.cpp \
	SOAPFault.cpp \
	SOAPHeader.cpp \
	SOAPHeaderHandler.cpp \
	SOAPMethod.cpp \
	SOAPMethodHandler.cpp \
	SOAPNamespaces.cpp \
	SOAPParameter.cpp \
	SOAPParameterHandler.cpp \
	SOAPParse.cpp \
	SOAPProxy.cpp \
	SOAPResponse.cpp \
	SOAPSecureSocketImp.cpp \
	SOAPServerDispatch.cpp \
	SOAPSocket.cpp \
	SOAPSSLContext.cpp \
	SOAPStructHandler.cpp \
	SOAPTypeTraits.cpp \
	SOAPUrl.cpp \
	SOAPonHTTP.cpp \
	XMLComposer.cpp \
	XMLParser.cpp 

libeasysoap_la_SOURCES = $(THESOURCES)
libeasysoap_a_SOURCES = $(THESOURCES)
noinst_HEADERS = \
	digcalc.h \
	global.h \
	md5.h \
	SOAPBodyHandler.h \
	SOAPClientSocketImp.h \
	SOAPEnvelopeHandler.h \
	SOAPHeaderHandler.h \
	SOAPMethodHandler.h \
	SOAPParameterHandler.h \
	SOAPSecureSocketImp.h \
	SOAPStructHandler.h \
	es_namespaces.h

INCLUDES=-I@top_srcdir@/include 

libeasysoap_la_LDFLAGS = -no-undefined 


AM_CFLAGS=-Wall -pedantic -I/usr/kerberos/include
AM_CXXFLAGS=-Wall -pedantic -I/usr/kerberos/include

libeasysoap_a_CXXFLAGS = $(AM_CXXFLAGS)
libeasysoap_a_CFLAGS = $(AM_CFLAGS)

EXTRA_DIST = \
	digtest.c \
	mddriver.c \
	SOAPWinInetTransport.cpp \
	SOAPISAPIServer.cpp \
	SOAPHTTPServer.cpp \
	isapi.cpp \
	isapi.def

