CCX = gcc

CPP_SOURCES = $(wildcard *.cpp)
CPP_OBJECTS = $(patsubst %.cpp,%.o,$(CPP_SOURCES))
C_HEADERS = $(wildcard *.h)


ifeq ($(OS),Windows_NT)
	LDFLAGS = -L./SDL2/lib
	INC = -I.  -I./SDL2/include
	LDLIBS = -lm -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	CFLAGS = -Wall -O3 -std=c++17 -m32
else 
	LDFLAGS = -L. -L/usr/lib
	INC = -I/usr/include/SDL2 
	LDLIBS = -lm -lSDL2 -lSDL2_ttf -lOpenCL
	CFLAGS = -Wall -O3 
endif

ifeq ($(DEBUG),1)
	CFLAGS += -g #-fsanitize=address
endif

ifeq ($(OS),Windows_NT)
	EXECUTABLE = main.exe
else 
	EXECUTABLE = main
endif

$(EXECUTABLE) : $(CPP_OBJECTS)
	$(CXX)  $(CFLAGS) -o $(EXECUTABLE) $(CPP_OBJECTS)  $(LDFLAGS) $(LDLIBS)

$(CPP_OBJECTS) : %.o : %.cpp
	$(CXX) $(INC) $(CFLAGS) -c $^ -o $@

clean:
	rm *.o $(EXECUTABLE)

