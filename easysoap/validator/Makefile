
#
# Where Expat is installed
EXPAT := /usr/local

#
# Linux Flags
CXXFLAGS := -g -Wall
LDFLAGS := -L$(EXPAT)/lib -L../lib -leasysoap -lexpat -lssl
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


CPPFLAGS := -I../include
OBJS := validator.o UserlandValidatorHandler.o WhichToolkitHandler.o 

validator : $(OBJS) $(EASYSOAPLIB)
	g++ $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(EASYSOAPLIB) :
	cd ../src; make

clean:
	rm -f *.o validator

