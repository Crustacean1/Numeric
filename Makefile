PROJNAME := $(shell pwd | xargs basename)
SOURCES := $(filter-out main.cpp, $(wildcard *.cpp))
OBJECTS := $(SOURCES:.cpp=.o)

CXX = g++
CXXFLAGS = -g -Wall -Werror -fmodules-ts -std=c++20
DEPENDS = ${OBJECTS:.o=.d} main.d

${PROJNAME}: ${SOURCES} main.cpp
	g++ ${CXXFLAGS} ${SOURCES} main.cpp -o  ${PROJNAME}

-include ${DEPENDS}

clean:
	rm -f *.o
	rm -f *.d
	rm -f */*.o
	rm -f */*.d
	rm ${PROJNAME}