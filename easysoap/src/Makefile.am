SUBDIRS = abyss
lib_LTLIBRARIES = libeasysoap.la
lib_LIBRARIES = libeasysoap.a
THESOURCES = \
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
	SOAPFaultHandler.cpp \
	SOAPHTTPServer.cpp \
	SOAPHeader.cpp \
	SOAPHeaderHandler.cpp \
	SOAPMethod.cpp \
	SOAPMethodHandler.cpp \
	SOAPNamespaces.cpp \
	SOAPPacketWriter.cpp \
	SOAPParameter.cpp \
	SOAPParameterHandler.cpp \
	SOAPParse.cpp \
	SOAPProxy.cpp \
	SOAPResponse.cpp \
	SOAPSecureSocketImp.cpp \
	SOAPServerDispatch.cpp \
	SOAPSocket.cpp \
	SOAPStructHandler.cpp \
	SOAPTypeTraits.cpp \
	SOAPUrl.cpp \
	SOAPonHTTP.cpp \
	XMLParser.cpp 

libeasysoap_la_SOURCES = $(THESOURCES)
libeasysoap_a_SOURCES = $(THESOURCES)
noinst_HEADERS = \
	SOAPBodyHandler.h \
	SOAPClientSocketImp.h \
	SOAPEnvelopeHandler.h \
	SOAPFaultHandler.h \
	SOAPHeaderHandler.h \
	SOAPMethodHandler.h \
	SOAPParameterHandler.h \
	SOAPSecureSocketImp.h \
	SOAPStructHandler.h \
	es_namespaces.h

INCLUDES=-I@top_srcdir@/include -I@top_srcdir@/src/abyss/src

libeasysoap_la_LDFLAGS = -no-undefined
libeasysoap_la_LIBADD = abyss/src/libabyss.la

libeasysoap_a_LIBADD = abyss/src/libabyss.la

EXTRA_DIST = easysoap.dsp SOAPISAPIServer.cpp isapi.cpp isapi.def

