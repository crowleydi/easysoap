
#
#
# Where Expat is installed
EXPAT := /usr/local

SOAPLIBDIR := ../lib
STATICLIB  := $(SOAPLIBDIR)/libeasysoap.a
SHAREDLIB  := $(SOAPLIBDIR)/libeasysoap.so

#
# flags for Linux
CXXFLAGS := -g -fPIC -Wall

#
# flags for cygwin
#CXXFLAGS := -g -Wall

CPPFLAGS := -I../include -I$(EXPAT)/include \
	 -DEASYSOAP_SSL # if you want https

SRCS := SOAPArrayHandler.cpp SOAPBase64.cpp SOAPBody.cpp \
	SOAPBodyHandler.cpp SOAPCGITransport.cpp SOAPClientSocketImp.cpp \
	SOAPDebugger.cpp SOAPEnvelope.cpp SOAPEnvelopeHandler.cpp \
	SOAPFault.cpp SOAPFaultHandler.cpp SOAPHeader.cpp \
	SOAPHeaderHandler.cpp SOAPMethod.cpp SOAPMethodHandler.cpp \
	SOAPPacketWriter.cpp SOAPParameter.cpp SOAPParameterHandler.cpp \
	SOAPParse.cpp SOAPProxy.cpp SOAPQName.cpp \
	SOAPResponse.cpp SOAPSecureSocketImp.cpp SOAPServerDispatch.cpp \
	SOAPSocket.cpp SOAPStructHandler.cpp SOAPUrl.cpp \
	SOAPonHTTP.cpp XMLParser.cpp

OBJS := $(SRCS:%.cpp=%.o)

all : $(STATICLIB)

$(STATICLIB) : $(OBJS)
	mkdir -p $(SOAPLIBDIR)
	$(AR) ru $@ $(OBJS)


$(SHAREDLIB) : $(OBJS)
	mkdir -p $(SOAPLIBDIR)
	g++ -shared -o $@ $(OBJS)


clean :
	rm -f core *.o $(SOAPLIB)

