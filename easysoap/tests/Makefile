
#
# Where Expat is installed
EXPAT = /usr/local

#

EASYDIR = ../lib
EASYSOAPLIB = $(EASYDIR)/libeasysoap.a

CXXFLAGS = -g -Wall
CPPFLAGS = -I../include
LDFLAGS = -L$(EXPAT)/lib -L../lib -leasysoap -lexpat -lssl

tests : tests.o $(EASYSOAPLIB)
	g++ -g -o $@ $< $(LDFLAGS)

$(EASYSOAPLIB) :
	cd ../src; make

clean:
	rm -f *.o tests

