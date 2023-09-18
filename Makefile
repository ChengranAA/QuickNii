# write a makefile for a program called test_opengl that uses the GLFW library and the OpenGL framework on macOS use static linking
# to compile the program, use the following command: make
# to run the program, use the following command: ./test_opengl
# to clean up the directory, use the following command: make clean
# to remove the executable, use the following command: make remove
# to remove the executable and clean up the directory, use the following command: make cleanalls

# compiler
CC = g++

# compiler flags
CFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -I./include -I./include/nifti -I/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3

# linker flags
LFLAGS = -L./lib/nifti_clib -lniftiio -lznz -lz -framework OpenGL -framework Cocoa -framework IOKit -framework GLUT

# executable name
EXE = ./bin/quicknii

# source files, everything in the src directory
SRC = $(wildcard ./src/*.cpp)

# object files
OBJ = $(SRC:.cpp=.o)

# default target
all: $(EXE)

# make a drectory bin if there isn't one
$(shell mkdir -p bin)

# link object files to create executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LFLAGS)

# compile source files to create object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# remove object files
clean:
	rm -f $(OBJ)

# remove executable
remove:
	rm -f $(EXE)

# remove object files and executable
cleanall: clean remove

# end of file


