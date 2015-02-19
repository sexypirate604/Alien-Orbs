# OS X
ifeq ("$(shell uname)", "Darwin")
  CXXFLAGS = -I/usr/include
  LIB = -framework OpenGL -framework GLUT
#endif

# Linux
#ifeq ("$(shell uname)", "linux")
else
  CXXFLAGS = -I/usr/include -Wdeprecated-declarations
  LIB = -lGL -lGLU -lglut
endif

DEPS = common.h utils.h terrain.h objects.h input.h state.h lodepng.h
OBJ = main.o utils.o terrain.o objects.o space-craft.o orb.o input.o state.cpp laser.cpp lodepng.o

default: build

build: p4

test: build
	./p4

%.o: %.cpp $(DEPS)
	clang++ -c -g $(CXXFLAGS) -o $@ $<

p4: $(OBJ)
	g++ -g -o $@ $^ $(LIB)

clean:
	- rm -f p4 *.o
