
#
#
# Where Expat is installed
EXPAT = /usr/local
SOAPLIBDIR = ../lib
SOAPLIB  = $(SOAPLIBDIR)/libeasysoap.a

#
# flags for Linux
CXXFLAGS = -g -fPIC -Wall

#
# flags for cygwin
#CXXFLAGS = -g -Wall

CPPFLAGS = -I../include -I$(EXPAT)/include \
	 -DEASYSOAP_SSL # if you want https

SRCS = SOAPArrayHandler.cpp SOAPBody.cpp SOAPBodyHandler.cpp \
	SOAPDebugger.cpp SOAPPacketWriter.cpp SOAPBase64.cpp \
	SOAPClientSocketImp.cpp SOAPEnvelope.cpp SOAPEnvelopeHandler.cpp \
	SOAPFault.cpp SOAPFaultHandler.cpp SOAPHeader.cpp \
	SOAPHeaderHandler.cpp SOAPMethod.cpp SOAPMethodHandler.cpp \
	SOAPParameter.cpp SOAPParameterHandler.cpp SOAPParse.cpp \
	SOAPProxy.cpp SOAPResponse.cpp SOAPQName.cpp \
	SOAPSecureSocketImp.cpp SOAPSocket.cpp SOAPStructHandler.cpp \
	SOAPUrl.cpp SOAPonHTTP.cpp XMLParser.cpp SOAPServerDispatch.cpp

OBJS = SOAPArrayHandler.o SOAPBody.o SOAPBodyHandler.o \
	SOAPDebugger.o SOAPPacketWriter.o SOAPBase64.o \
	SOAPClientSocketImp.o SOAPEnvelope.o SOAPEnvelopeHandler.o \
	SOAPFault.o SOAPFaultHandler.o SOAPHeader.o \
	SOAPHeaderHandler.o SOAPMethod.o SOAPMethodHandler.o \
	SOAPParameter.o SOAPParameterHandler.o SOAPParse.o \
	SOAPProxy.o SOAPResponse.o SOAPQName.o \
	SOAPSecureSocketImp.o SOAPSocket.o SOAPStructHandler.o \
	SOAPUrl.o SOAPonHTTP.o XMLParser.o SOAPServerDispatch.o

all : $(SOAPLIB)

$(SOAPLIB) : $(OBJS)
	mkdir -p $(SOAPLIBDIR)
	$(AR) ru $@ $(OBJS)


clean :
	rm -f core *.o $(SOAPLIB)

