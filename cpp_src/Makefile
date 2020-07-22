.DEFAULT_GOAL := all

# location of the Python header files
PYTHON_VERSION = 3.8
SERVER_PYTHON_VERSION = 3
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
SERVER_PYTHON_INCLUDE = /home/user/.pyenv/versions/3.6.10/include/python3.6m

# location of the Boost Python include files and library
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib/x86_64-linux-gnu
SERVER_BOOST_LIB = /usr/lib/x86_64-linux-gnu

LIBBOOST = libboost_python$(subst .,,$(PYTHON_VERSION)).so
SERVER_LIBBOOST = libboost_python-py35.so

PYTHON_LIB = /usr/lib/python$(PYTHON_VERSION)/config-3.8m-x86_64-linux-gnu
SERVER_PYTHON_LIB = /home/user/.pyenv/versions/3.6.10/lib

CFLAGS = --std=c++11

core.so: core.o
	g++ -shared -Wl,--export-dynamic Util.o Node.o Tree.o ConllReader.o core.o -L$(BOOST_LIB) -l:$(LIBBOOST) -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION) -o core.so $(CFLAGS)

core.o: core.cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c Util.cpp Node.cpp Tree.cpp ConllReader.cpp core.cpp $(CFLAGS)

algorithms.so: algorithms.o
	g++ -shared -Wl,--export-dynamic Util.o Node.o Tree.o algorithms.o -L$(BOOST_LIB) -l:$(LIBBOOST) -L$(PYTHON_LIB) -lpython$(PYTHON_VERSION) -o algorithms.so $(CFLAGS)

algorithms.o: algorithms.cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c algorithms.cpp $(CFLAGS)

all: core.so algorithms.so
