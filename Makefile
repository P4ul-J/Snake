#OBJS specifies which files to compile as part of the project
OBJS = main.c

#CC specifies which compiler we're using
CC = gcc

#INCLUDE_PATHS specifies the additional include paths we'll need
#INCLUDE_PATHS = -I"C:\Users\lihrinni\Documents\programs\TDM-GCC-64\x86_64-w64-mingw32\include"

#LIBRARY_PATHS specifies the additional library paths we'll need
#LIBRARY_PATHS = -L"C:\Users\lihrinni\Documents\programs\TDM-GCC-64\x86_64-w64-mingw32\lib"

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
#Compiler findet SDL_image library nicht :/
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = snake

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
