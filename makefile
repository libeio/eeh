

SRCS        = bic.cpp eehandler.cpp eemodule.cpp eeclient.cpp eehelper.cpp eelog.cpp sha1.cpp eefunc.cpp
OBJS        = $(SRCS:.cpp=.o)
LIB         = libeeh.a
CXXFLAGS    += -g -O0 -Wall -W -std=c++11 -lpthread
CXXFLAGS    += -I.

.PHONY: all
all:
	$(CXX) $(CXXFLAGS) -c -o bic.o bic.cpp
	$(CXX) $(CXXFLAGS) -c -o eehandler.o eehandler.cpp
	$(CXX) $(CXXFLAGS) -c -o eemodule.o eemodule.cpp
	$(CXX) $(CXXFLAGS) -c -o eeclient.o eeclient.cpp
	$(CXX) $(CXXFLAGS) -c -o eehelper.o eehelper.cpp
	$(CXX) $(CXXFLAGS) -c -o eelog.o eelog.cpp
	$(CXX) $(CXXFLAGS) -c -o sha1.o sha1.cpp
	$(CXX) $(CXXFLAGS) -c -o eefunc.o eefunc.cpp
	$(AR) cr $(LIB) $(OBJS)
	$(MAKE) -C test

.PHONY: clean
clean:
	rm -rf $(OBJS) $(LIB)
	$(MAKE) -C test clean
