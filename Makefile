PROJNAME := $(shell pwd | xargs basename)
SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

CXX := g++
CXXFLAGS := -g -Wall -O3 -Wextra -MMD -fmodules-ts -std=c++20 -c
DEPENDS := ${OBJECTS:.o=.d} main.d

${PROJNAME}: ${OBJECTS}
	g++ ${LDFLAGS}  ${OBJECTS} -o  ${PROJNAME}


-include ${DEPENDS}

clean:
	rm -f *.o
	rm -f *.d
	rm -f */*.o
	rm -f */*.d
	rm ${PROJNAME}
