CCX = gcc
CFLAGS = -Wall -g

CPP_SOURCES = $(wildcard *.cpp)
CPP_OBJECTS = $(patsubst %.cpp,%.o,$(CPP_SOURCES))
C_HEADERS = $(wildcard *.h)


LDFLAGS = -L. -L/usr/lib
LDLIBS = -lm -lSDL2 -lSDL2_ttf -lOpenCL
INC = -I. -I/usr/include/SDL2


EXECUTABLE = main

$(EXECUTABLE) : $(CPP_OBJECTS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(EXECUTABLE) $(CPP_OBJECTS)

$(CPP_OBJECTS) : %.o : %.cpp
	$(CXX) $(INC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *o $(EXECUTABLE)
