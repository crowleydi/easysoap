
#
# Where Expat is installed
EXPAT = /usr/local

#
# Linux Flags
CXXFLAGS = -g -Wall
LDFLAGS = -L$(EXPAT)/lib -L../lib -leasysoap -lexpat -lssl
#
# Cygwin Flags
#CXXFLAGS = -g -Wall
#LDFLAGS = -L$(EXPAT)/lib -L../lib -leasysoap -lexpat -lssl -lcrypto -lwsock32

EASYDIR = ../lib
EASYSOAPLIB = $(EASYDIR)/libeasysoap.a


CPPFLAGS = -I../include

tests : tests.o $(EASYSOAPLIB)
	g++ -g -o $@ $< $(LDFLAGS)

$(EASYSOAPLIB) :
	cd ../src; make

clean:
	rm -f *.o tests

