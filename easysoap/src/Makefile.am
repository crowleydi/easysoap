lib_LTLIBRARIES = libeasysoap.la
lib_LIBRARIES = libeasysoap.a
THESOURCES = \
    SOAPArrayHandler.cpp SOAPBase64.cpp SOAPBody.cpp \
    SOAPBodyHandler.cpp SOAPCGITransport.cpp SOAPClientSocketImp.cpp \
    SOAPDebugger.cpp SOAPEnvelope.cpp SOAPEnvelopeHandler.cpp \
    SOAPFault.cpp SOAPFaultHandler.cpp SOAPHeader.cpp \
    SOAPHeaderHandler.cpp SOAPMethod.cpp SOAPMethodHandler.cpp \
    SOAPPacketWriter.cpp SOAPParameter.cpp SOAPParameterHandler.cpp \
    SOAPParse.cpp SOAPProxy.cpp SOAPQName.cpp \
    SOAPResponse.cpp SOAPSecureSocketImp.cpp SOAPServerDispatch.cpp \
    SOAPSocket.cpp SOAPStructHandler.cpp SOAPUrl.cpp \
    SOAPonHTTP.cpp XMLParser.cpp
libeasysoap_la_SOURCES = $(THESOURCES)
libeasysoap_a_SOURCES = $(THESOURCES)
noinst_HEADERS = \
    SOAPArrayHandler.h SOAPBodyHandler.h SOAPClientSocketImp.h \
    SOAPEnvelopeHandler.h SOAPFaultHandler.h SOAPHeaderHandler.h \
    SOAPMethodHandler.h SOAPParameterHandler.h \
    SOAPSecureSocketImp.h SOAPStructHandler.h 
INCLUDES=$(EASYSOAP_INCLUDES) $(SSL_DEFINES)
