
#
# Where Expat is installed
EXPAT := /usr/local

#
# Linux Flags
CXXFLAGS := -g -Wall
LDFLAGS := -L$(EXPAT)/lib -L../lib -leasysoap -lssl -lexpat
#
# SunOS Flags
#CXXFLAGS := -g -Wall
#LDFLAGS := -L$(EXPAT)/lib -L../lib -leasysoap -lexpat -lsocket -lnsl
#
# Cygwin Flags
#CXXFLAGS := -g -Wall
#LDFLAGS := -L$(EXPAT)/lib -L../lib -leasysoap -lexpat -lwsock32 -lssl -lcrypto

EASYDIR := ../lib
EASYSOAPLIB := $(EASYDIR)/libeasysoap.a


CLIENTOBJS := interopclient.o
SERVEROBJS := interopserver.o SOAPBuildersInteropHandler.o

CPPFLAGS := -I../include

all: interopserver interopclient

interopclient : $(CLIENTOBJS) $(EASYSOAPLIB)
	g++ $(CXXFLAGS) -o $@ $(CLIENTOBJS) $(LDFLAGS)

interopserver : $(SERVEROBJS) $(EASYSOAPLIB)
	g++ $(CXXFLAGS) -o $@ $(SERVEROBJS) $(LDFLAGS)

$(EASYSOAPLIB) :
	cd ../src; make

interop.html: interop.xsl interop.xml
	sabcmd interop.xsl interop.xml interop.html

clean:
	rm -f core *.o interopclient interopserver interop.html

