CCX = gcc

CPP_SOURCES = $(wildcard *.cpp)
CPP_OBJECTS = $(patsubst %.cpp,%.o,$(CPP_SOURCES))
C_HEADERS = $(wildcard *.h)


ifeq ($(OS),Windows_NT)
	LDFLAGS = -L./SDL2-2.32.4/i686-w64-mingw32/lib -L./SDL2-2.32.4/x86_64-w64-mingw32/lib -L./SDL2_ttf-2.24.0/i686-w64-mingw32/lib -L./SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib
	INC = -I.  -I./SDL2-2.32.4/i686-w64-mingw32/include -I./SDL2-2.32.4/x86_64-w64-mingw32/include 
	INC += -I./SDL2_ttf-2.24.0/i686-w64-mingw32/include -I./SDL2_ttf-2.24.0/x86_64-w64-mingw32/include
	INC += -I./SDL2-2.32.4/i686-w64-mingw32/include/SDL2 -I./SDL2-2.32.4/x86_64-w64-mingw32/include/SDL2
	LDLIBS = -lm -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	CFLAGS = -Wall -O3 -std=c++17
else 
	LDFLAGS = -L. -L/usr/lib
	INC = -I/usr/include/SDL2
	LDLIBS = -lm -lSDL2 -lSDL2_ttf
	CFLAGS = -Wall -O3 
endif

ifeq ($(DEBUG),1)
	CFLAGS += -g -fsanitize=address
endif

ifeq ($(OS),Windows_NT)
	EXECUTABLE = main.exe
else 
	EXECUTABLE = main
endif

ifeq ($(OS),Windows_NT)

copy-dlls: $(EXECUTABLE)
	move.bat

endif

$(EXECUTABLE) : $(CPP_OBJECTS)
	$(CXX)  $(CFLAGS) -o $(EXECUTABLE) $(CPP_OBJECTS)  $(LDFLAGS) $(LDLIBS)

ifeq ($(OS),Windows_NT)

$(CPP_OBJECTS) : %.o : %.cpp
	if not exist "SDL2_ttf-2.24.0" (install_sdl_ttf.bat)
	if not exist "SDL2-2.32.4" (install_sdl.bat)
	$(CXX) $(INC) $(CFLAGS) -c $^ -o $@
else

$(CPP_OBJECTS) : %.o : %.cpp
	$(CXX) $(INC) $(CFLAGS) -c $^ -o $@

endif

ifeq ($(OS),Windows_NT)
clean:
	del *.o $(EXECUTABLE)
else 
clean:
	rm *.o $(EXECUTABLE)
endif
